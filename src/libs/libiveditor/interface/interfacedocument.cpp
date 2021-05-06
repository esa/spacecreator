/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "interfacedocument.h"

#include "actionsbar.h"
#include "asn1modelstorage.h"
#include "baseitems/common/ivutils.h"
#include "baseitems/graphicsview.h"
#include "colors/colormanagerdialog.h"
#include "commands/cmdentitiesimport.h"
#include "commands/cmdentitiesinstantiate.h"
#include "commandsstack.h"
#include "commonvisualizationmodel.h"
#include "context/action/actionsmanager.h"
#include "context/action/editor/dynactioneditor.h"
#include "creatortool.h"
#include "file.h"
#include "graphicsitemhelpers.h"
#include "interface/objectstreeview.h"
#include "interface/properties/propertiesdialog.h"
#include "interface/properties/propertytemplatemanager.h"
#include "interface/properties/propertytemplatewidget.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivfunctiongraphicsitem.h"
#include "ivitemmodel.h"
#include "ivmodel.h"
#include "ivxmlreader.h"
#include "propertytemplateconfig.h"
#include "xmldocexporter.h"

#include <QAction>
#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDir>
#include <QDirIterator>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QSplitter>
#include <QStandardPaths>
#include <QToolBar>
#include <QUndoStack>
#include <QVBoxLayout>
#include <algorithm>

namespace ive {

struct InterfaceDocument::InterfaceDocumentPrivate {
    cmd::CommandsStack *commandsStack { nullptr };

    QString filePath;

    QPointer<QWidget> view;
    ive::GraphicsView *graphicsView { nullptr };
    ivm::PropertyTemplateConfig *dynPropConfig { nullptr };
    QTreeView *objectsView { nullptr };
    IVItemModel *itemsModel { nullptr };
    CommonVisualizationModel *objectsVisualizationModel { nullptr };
    QItemSelectionModel *objectsSelectionModel { nullptr };
    ivm::IVModel *objectsModel { nullptr };
    ObjectsTreeView *importView { nullptr };
    ivm::IVModel *importModel { nullptr };
    ObjectsTreeView *sharedView { nullptr };
    ivm::IVModel *sharedModel { nullptr };

    QAction *actCreateConnectionGroup { nullptr };
    QAction *actRemove { nullptr };
    QAction *actZoomIn { nullptr };
    QAction *actZoomOut { nullptr };
    QAction *actExitToRoot { nullptr };
    QAction *actExitToParent { nullptr };
    QVector<QAction *> m_toolbarActions;

    CreatorTool *tool { nullptr };

    Asn1Acn::Asn1ModelStorage *asnModelStorage { nullptr };
    QString mscFileName;
    QString asnFileName;
};

/*!
\class ive::InterfaceDocument
\brief ive::InterfaceDocument is the document for graphical IV data (loaded from the XML).
*/

InterfaceDocument::InterfaceDocument(QObject *parent)
    : QObject(parent)
    , d(new InterfaceDocumentPrivate)
{
    setAsn1ModelStorage(new Asn1Acn::Asn1ModelStorage(this));

    d->commandsStack = new cmd::CommandsStack(this);
    connect(d->commandsStack, &cmd::CommandsStack::cleanChanged, this,
            [this](bool clean) { Q_EMIT dirtyChanged(!clean); });

    d->dynPropConfig = ivm::PropertyTemplateConfig::instance();
    d->dynPropConfig->init(ive::dynamicPropertiesFilePath());

    d->importModel = new ivm::IVModel(d->dynPropConfig, this);
    d->sharedModel = new ivm::IVModel(d->dynPropConfig, this);
    d->objectsModel = new ivm::IVModel(d->dynPropConfig, this);
    d->objectsModel->setSharedTypesModel(d->sharedModel);
}

InterfaceDocument::~InterfaceDocument()
{
    delete d->view;
    delete d;
}

void InterfaceDocument::init()
{
    if (!d->view.isNull()) {
        return;
    }

    d->itemsModel = new IVItemModel(d->objectsModel, d->commandsStack, this);
    connect(d->itemsModel, &IVItemModel::itemClicked, this, &InterfaceDocument::onItemClicked);
    connect(d->itemsModel, &IVItemModel::itemDoubleClicked, this, &InterfaceDocument::onItemDoubleClicked);
    connect(d->itemsModel, &IVItemModel::itemsSelected, this, &InterfaceDocument::onSceneSelectionChanged);

    d->view = new QWidget;
    auto rootLayout = new QVBoxLayout;
    rootLayout->setMargin(0);
    d->view->setLayout(rootLayout);

    QWidget *panelWidget = new QWidget;
    QVBoxLayout *panelLayout = new QVBoxLayout;
    panelLayout->setMargin(0);
    panelWidget->setLayout(panelLayout);

    QSplitter *panelSplitter = new QSplitter(Qt::Vertical, panelWidget);
    panelSplitter->setHandleWidth(1);
    panelSplitter->addWidget(createModelView());
    panelSplitter->addWidget(createImportView());
    panelSplitter->addWidget(createSharedView());
    panelLayout->addWidget(panelSplitter);

    QWidget *editorView = createGraphicsView();
    auto editorWidget = new QWidget;
    auto editorLayout = new QHBoxLayout;
    editorLayout->setMargin(0);
    editorLayout->setSpacing(0);
    editorWidget->setLayout(editorLayout);
    auto ivToolBar = new shared::ActionsBar(editorWidget);
    initActions();
    for (QAction *action : qAsConst(d->m_toolbarActions)) {
        ivToolBar->addAction(action);
    }
    editorLayout->addWidget(ivToolBar);
    editorLayout->addWidget(editorView);

    auto splitter = new QSplitter(Qt::Horizontal, d->view);
    splitter->setHandleWidth(1);
    splitter->addWidget(panelWidget);
    splitter->addWidget(editorWidget);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    rootLayout->addWidget(splitter);

    QTimer::singleShot(0, this, &InterfaceDocument::loadAvailableComponents);
}

void InterfaceDocument::fillToolBar(QToolBar *toolBar)
{
    if (toolBar == nullptr) {
        return;
    }

    toolBar->setUpdatesEnabled(false);
    toolBar->clear();

    const QVector<QAction *> &docActions = initActions();
    for (QAction *action : docActions) {
        toolBar->addAction(action);
    }

    toolBar->setUpdatesEnabled(true);
}

QGraphicsScene *InterfaceDocument::scene() const
{
    return d->itemsModel->scene();
}

shared::ui::GraphicsViewBase *InterfaceDocument::graphicsView() const
{
    return d->graphicsView;
}

QWidget *InterfaceDocument::view() const
{
    return d->view;
}

QUndoStack *InterfaceDocument::undoStack() const
{
    Q_ASSERT(d->commandsStack);
    return d->commandsStack->undoStack();
}

cmd::CommandsStack *InterfaceDocument::commandsStack() const
{
    return d->commandsStack;
}

bool InterfaceDocument::create(const QString &path)
{
    bool created = false;
    if (!path.isEmpty()) {
        created = loadImpl(path);
    }
    if (created) {
        d->commandsStack->clear();
    }
    return created;
}

bool InterfaceDocument::load(const QString &path)
{
    const QString oldPath = d->filePath = path;
    setPath(path);

    const bool loaded = loadImpl(path);

    if (loaded) {
        d->commandsStack->clear();
    } else {
        setPath(oldPath);
    }

    return loaded;
}

bool InterfaceDocument::loadAvailableComponents()
{
    bool result = true;

    d->importModel->clear();
    QDirIterator importableIt(ive::componentsLibraryPath(), QDir::Dirs | QDir::NoDotAndDotDot);
    while (importableIt.hasNext()) {
        result |= loadComponentModel(d->importModel, importableIt.next() + QDir::separator() + kDefaultFilename);
    }

    d->sharedModel->clear();
    QDirIterator instantiatableIt(ive::sharedTypesPath(), QDir::Dirs | QDir::NoDotAndDotDot);
    while (instantiatableIt.hasNext()) {
        result |= loadComponentModel(d->sharedModel, instantiatableIt.next() + QDir::separator() + kDefaultFilename);
    }

    return result;
}

QString InterfaceDocument::getComponentName(const QStringList &exportNames)
{
    QString name = exportNames.join(QLatin1Char('_'));
    auto dialog = new QDialog(qobject_cast<QWidget *>(window()));
    dialog->setWindowTitle(tr("Export"));
    auto layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("Enter the name for exporting component:"), dialog));
    auto lineEdit = new QLineEdit(dialog);
    lineEdit->setText(name);
    lineEdit->selectAll();
    auto box = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    connect(box, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
    layout->addWidget(lineEdit);
    layout->addWidget(box);
    dialog->setLayout(layout);
    dialog->adjustSize();
    if (dialog->exec() == QDialog::Accepted) {
        name = lineEdit->text();
    }
    dialog->deleteLater();
    return name;
}

QList<ivm::IVObject *> InterfaceDocument::prepareSelectedObjectsForExport(QString &name, bool silent)
{
    QList<ivm::IVObject *> objects;
    QStringList exportNames;
    for (const auto id : d->objectsSelectionModel->selection().indexes()) {
        const int role = static_cast<int>(ive::CommonVisualizationModel::IdRole);
        if (ivm::IVObject *object = d->objectsModel->getObject(id.data(role).toUuid())) {
            if (object->isFunction() && object->parentObject() == nullptr) {
                exportNames.append(object->entityAttributeValue<QString>(QLatin1String("name")));
            }
            objects.append(object);
        }
    }

    name = silent ? exportNames.join(QLatin1Char('_')) : getComponentName(exportNames);
    if (exportNames.size() > 1) {
        ivm::IVFunction *dummyFunction = new ivm::IVFunction(name);
        for (auto object : objects) {
            if (!object->parentObject()) {
                dummyFunction->addChild(object);
            }
        }
        objects.prepend(dummyFunction);
    }
    return objects;
}

bool InterfaceDocument::exportSelectedFunctions()
{
    QString name;
    const QList<ivm::IVObject *> objects = prepareSelectedObjectsForExport(name);
    d->objectsSelectionModel->clearSelection();
    const QString path = componentsLibraryPath() + name;
    if (exportImpl(path, objects)) {
        return loadComponentModel(d->importModel, path + QDir::separator() + kDefaultFilename);
    }
    return false;
}

bool InterfaceDocument::exportSelectedType()
{
    const auto indexes = d->objectsSelectionModel->selection().indexes();
    if (indexes.isEmpty()) {
        return false;
    }
    static const int role = static_cast<int>(ive::CommonVisualizationModel::IdRole);
    ivm::IVObject *rootType = nullptr;
    for (const auto index : indexes) {
        if (ivm::IVObject *object = d->objectsModel->getObject(index.data(role).toUuid())) {
            if (object->isFunctionType() && object->parentObject() == nullptr) {
                if (rootType) {
                    return false;
                }
                rootType = object;
            }
        }
    }
    if (!rootType) {
        return false;
    }
    d->objectsSelectionModel->clearSelection();
    const QString path = sharedTypesPath() + QDir::separator() + rootType->title();
    if (exportImpl(path, { rootType })) {
        return loadComponentModel(d->sharedModel, path + QDir::separator() + kDefaultFilename);
    }
    return false;
}

bool InterfaceDocument::loadComponentModel(ivm::IVModel *model, const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        qWarning() << Q_FUNC_INFO << "Invalid path";
        return false;
    }

    ivm::IVXMLReader parser;
    if (!parser.readFile(path)) {
        qWarning() << parser.errorString();
        return false;
    }

    model->addObjects(parser.parsedObjects());
    return true;
}

void InterfaceDocument::close()
{
    d->objectsModel->clear();
    setPath(QString());
    d->commandsStack->clear();
}

QString InterfaceDocument::path() const
{
    return d->filePath;
}

void InterfaceDocument::setPath(const QString &path)
{
    if (path != d->filePath) {
        d->filePath = path;
        Q_EMIT titleChanged();
    }
}

/*!
  Sets the filename of the used asn1 file. This is only the file name without a path. The File is expected to be next
   to the iv file
 */
void InterfaceDocument::setAsn1FileName(const QString &asnfile)
{
    if (d->asnFileName == asnfile) {
        return;
    }

    d->asnFileName = asnfile;
    Q_EMIT asn1FileNameChanged(d->asnFileName);

    if (d->asnModelStorage) {
        if (d->asnModelStorage->contains(asn1FilePath())) {
            checkAllInterfacesForAsn1Compliance();
        } else {
            // does load the data
            d->asnModelStorage->asn1DataTypes(asn1FilePath());
        }
    }
}

/*!
   Returns the filename of the used asn1 file. It does not contain any path.
 */
QString InterfaceDocument::asn1FileName() const
{
    return d->asnFileName;
}

/*!
   Returns the ASN.1 file including it's full path
 */
QString InterfaceDocument::asn1FilePath() const
{
    if (path().isEmpty()) {
        return {};
    }

    QFileInfo fi(path());
    fi.setFile(fi.absolutePath() + QDir::separator() + d->asnFileName);
    return fi.absoluteFilePath();
}

/*!
   Sets the name of the associated msc file. This is only the file name without a path. The File is expected to be next
   to the iv file
 */
void InterfaceDocument::setMscFileName(const QString &mscFile)
{
    if (mscFile == d->mscFileName) {
        return;
    }

    d->mscFileName = mscFile;
    Q_EMIT mscFileNameChanged(mscFile);
}

/*!
   Returns the name of the associated msc file. This is only the file name without a path. The File is expected to be
   next to the iv file
 */
const QString &InterfaceDocument::mscFileName() const
{
    return d->mscFileName;
}

bool InterfaceDocument::isDirty() const
{
    return d->commandsStack && !d->commandsStack->isClean();
}

QString InterfaceDocument::title() const
{
    const QString fileName = QFileInfo(path()).fileName();
    if (fileName.isEmpty()) {
        return tr("Untitled");
    }
    return fileName;
}

QList<QAction *> InterfaceDocument::customActions() const
{
    QList<QAction *> actions;
    QAction *actCommonProps = new QAction(tr("Common Properties..."));
    actions.append(actCommonProps);
    connect(actCommonProps, &QAction::triggered, this, &InterfaceDocument::onAttributesManagerRequested);
    //    QAction *actDataTypes = root->addAction(tr("Data types"));
    //    connect(actDataTypes, &QAction::triggered, this, &InterfaceDocument::onDataTypesMenuInvoked);
    QAction *actColorScheme = new QAction(tr("Color Scheme..."));
    actions.append(actColorScheme);
    connect(actColorScheme, &QAction::triggered, this, &InterfaceDocument::onColorSchemeMenuInvoked);
    QAction *actDynContext = new QAction(tr("Context Actions..."));
    actions.append(actDynContext);
    connect(actDynContext, &QAction::triggered, this, &InterfaceDocument::onDynContextEditorMenuInvoked);

    ActionsManager::registerAction(Q_FUNC_INFO, actCommonProps, "Edit Properties", "Show editor for common Properties");
    //    ActionsManager::registerAction(Q_FUNC_INFO, actDataTypes, "Edit Datatypes", "Show editor for common
    //    Datatypes");
    ActionsManager::registerAction(
            Q_FUNC_INFO, actColorScheme, "Edit Color scheme", "Show editor for common Color schemes");
    ActionsManager::registerAction(
            Q_FUNC_INFO, actDynContext, "Context actions", "Show editor for common custom context menu actions");

    return actions;
}

const QHash<shared::Id, ivm::IVObject *> &InterfaceDocument::objects() const
{
    return d->objectsModel->objects();
}

ivm::IVModel *InterfaceDocument::objectsModel() const
{
    return d->objectsModel;
}

ivm::IVModel *InterfaceDocument::importModel() const
{
    return d->importModel;
}

IVItemModel *InterfaceDocument::itemsModel() const
{
    return d->itemsModel;
}

Asn1Acn::Asn1ModelStorage *InterfaceDocument::asn1ModelStorage() const
{
    return d->asnModelStorage;
}

void InterfaceDocument::setAsn1ModelStorage(Asn1Acn::Asn1ModelStorage *asn1Storage)
{
    if (!asn1Storage) {
        return;
    }

    if (d->asnModelStorage) {
        disconnect(d->asnModelStorage, nullptr, this, nullptr);
        if (d->asnModelStorage->parent() == this) {
            d->asnModelStorage->deleteLater();
        }
    }

    d->asnModelStorage = asn1Storage;
    connect(d->asnModelStorage, &Asn1Acn::Asn1ModelStorage::dataTypesChanged, this, [&](const QString &fileName) {
        if (fileName == asn1FilePath()) {
            checkAllInterfacesForAsn1Compliance();
        }
    });
}

QString InterfaceDocument::supportedFileExtensions() const
{
    return QStringLiteral("*.xml");
}

/*!
   \brief InterfaceDocument::checkInterfaceAsn1Compliance
   \param interface
   \return
 */
bool InterfaceDocument::checkInterfaceAsn1Compliance(const ivm::IVInterface *interface) const
{
    if (!d->asnModelStorage) {
        return true;
    }
    const QSharedPointer<Asn1Acn::File> dataTypes = d->asnModelStorage->asn1DataTypes(asn1FilePath());
    if (dataTypes.isNull()) {
        return true;
    }

    const QVector<ivm::InterfaceParameter> &params = interface->params();
    if (params.isEmpty()) {
        return true;
    }
    return std::any_of(params.cbegin(), params.cend(),
            [dataTypes](const ivm::InterfaceParameter &param) { return dataTypes->hasType(param.paramTypeName()); });
}

/*!
   \brief InterfaceDocument::checkAllInterfacesForAsn1Compliance
   \param faultyInterface
   \return
 */
bool InterfaceDocument::checkAllInterfacesForAsn1Compliance()
{
    QStringList faultyInterfaces;

    bool ok = true;
    for (ivm::IVInterface *interface : d->objectsModel->allObjectsByType<ivm::IVInterface>()) {
        if (!checkInterfaceAsn1Compliance(interface)) {
            ok = false;
            const QString id = QString("%1.%2").arg(
                    interface->function() ? interface->function()->title() : "", interface->title());
            QString parameters;
            for (const ivm::InterfaceParameter &param : interface->params()) {
                if (!parameters.isEmpty()) {
                    parameters += ", ";
                }
                parameters += param.toString();
            }
            faultyInterfaces << QString("%1(%2)").arg(id, parameters);
        }
    }

    if (!ok) {
        Q_EMIT asn1ParameterErrorDetected(faultyInterfaces);
    }

    return ok;
}

void InterfaceDocument::onSavedExternally(const QString &filePath, bool saved)
{
    if (saved) {
        setPath(filePath);
        d->commandsStack->setClean();
    }
}

/*!
   \brief InterfaceDocument::setObjects
 */
void InterfaceDocument::setObjects(const QVector<ivm::IVObject *> &objects)
{
    d->objectsModel->initFromObjects(objects);
    d->objectsModel->setRootObject({});
}

void InterfaceDocument::onItemClicked(const shared::Id &id)
{
    Q_UNUSED(id)
}

void InterfaceDocument::onItemDoubleClicked(const shared::Id &id)
{
    if (id.isNull()) {
        return;
    }

    showPropertyEditor(id);
}

void InterfaceDocument::onItemCreated(const shared::Id &id)
{
    if (id.isNull()) {
        return;
    }

    showPropertyEditor(id);
}

void InterfaceDocument::onAttributesManagerRequested()
{
    auto dialog = new ive::PropertyTemplateManager(window());
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceDocument::onDataTypesMenuInvoked()
{
    if (auto act = qobject_cast<QAction *>(sender()))
        showNIYGUI(act->text());
}

void InterfaceDocument::prepareEntityNameForEditing(const shared::Id &id)
{
    if (auto entity = d->itemsModel->getItem(id)) {
        if (auto iObj = qobject_cast<InteractiveObject *>(entity->toGraphicsObject())) {
            iObj->enableEditMode();
        }
    }
}

void InterfaceDocument::onColorSchemeMenuInvoked()
{
    shared::ColorManagerDialog *dialog = new shared::ColorManagerDialog(window());
    dialog->setFilterGroup("IVE");
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceDocument::onDynContextEditorMenuInvoked()
{
    auto dialog = new DynActionEditor(window());
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    dialog->open();
    if (!dialog->init()) {
        dialog->close();
    }
}

void InterfaceDocument::showPropertyEditor(const shared::Id &id)
{
    Q_ASSERT(d->asnModelStorage);
    Q_ASSERT(d->commandsStack);
    Q_ASSERT(d->graphicsView);
    if (id.isNull()) {
        return;
    }

    ivm::IVObject *obj = d->objectsModel->getObject(id);
    if (!obj || obj->type() == ivm::IVObject::Type::InterfaceGroup || obj->type() == ivm::IVObject::Type::Connection) {
        return;
    }

    ive::PropertiesDialog dialog(d->dynPropConfig, obj, d->asnModelStorage->asn1DataTypes(asn1FilePath()),
            d->commandsStack, d->graphicsView);
    dialog.exec();
}

void InterfaceDocument::showInfoMessage(const QString &title, const QString &message)
{
    QMessageBox::information(qobject_cast<QWidget *>(parent()), title, message);
}

void InterfaceDocument::importEntity(const shared::Id &id, const QPointF &sceneDropPoint)
{
    const auto obj = d->importModel->getObject(id);
    if (!obj) {
        return;
    }
    const auto existingFunctionNames = d->objectsModel->nestedFunctionNames();
    const auto intersectedNames = d->importModel->nestedFunctionNames(obj->as<const ivm::IVFunctionType *>())
                                          .intersect(existingFunctionNames);
    if (!intersectedNames.isEmpty()) {
        QMessageBox::critical(view()->window(), tr("Entity importing"),
                tr("Chosen entity [%1] couldn't be imported because of Function names conflict(s): %2")
                        .arg(obj->titleUI())
                        .arg(intersectedNames.toList().join(QLatin1Char('\n'))));
        return;
    }
    QGraphicsItem *itemAtScenePos = scene()->itemAt(sceneDropPoint, graphicsView()->transform());
    while (itemAtScenePos && itemAtScenePos->type() != IVFunctionGraphicsItem::Type) {
        itemAtScenePos = itemAtScenePos->parentItem();
    }

    QBuffer buffer;
    if (!buffer.open(QIODevice::WriteOnly)) {
        qWarning() << "Can't open buffer for exporting:" << buffer.errorString();
        return;
    }

    if (!XmlDocExporter::exportObjects({ obj }, &buffer)) {
        qWarning() << "Error during component export";
        return;
    }
    buffer.close();

    ivm::IVFunctionType *parentObject = gi::functionObject(itemAtScenePos);
    auto cmdImport = new cmd::CmdEntitiesImport(
            buffer.data(), parentObject, d->objectsModel, sceneDropPoint, QFileInfo(path()).absolutePath());
    d->commandsStack->push(cmdImport);
}

void InterfaceDocument::instantiateEntity(const shared::Id &id, const QPointF &sceneDropPoint)
{
    const auto obj = d->sharedModel->getObject(id);
    if (!obj || obj->type() != ivm::IVObject::Type::FunctionType) {
        return;
    }
    QGraphicsItem *itemAtScenePos = scene()->itemAt(sceneDropPoint, graphicsView()->transform());
    while (itemAtScenePos && itemAtScenePos->type() != IVFunctionGraphicsItem::Type) {
        itemAtScenePos = itemAtScenePos->parentItem();
    }
    ivm::IVFunctionType *parentObject = gi::functionObject(itemAtScenePos);
    auto cmdInstantiate = new cmd::CmdEntitiesInstantiate(
            obj->as<ivm::IVFunctionType *>(), parentObject, d->objectsModel, sceneDropPoint);
    d->commandsStack->push(cmdInstantiate);
}

void InterfaceDocument::copyItems()
{
    QBuffer buffer;
    if (!buffer.open(QIODevice::WriteOnly)) {
        qWarning() << "Can't open buffer for exporting:" << buffer.errorString();
        return;
    }

    QString name;
    const QList<ivm::IVObject *> objects = prepareSelectedObjectsForExport(name, true);
    if (!XmlDocExporter::exportObjects(objects, &buffer)) {
        qWarning() << "Error during component export";
        return;
    }
    buffer.close();
    QApplication::clipboard()->setText(QString::fromUtf8(buffer.data()));
}

void InterfaceDocument::cutItems()
{
    copyItems();
    d->tool->removeSelectedItems();
}

void InterfaceDocument::pasteItems(const QPointF &sceneDropPoint)
{
    const QByteArray data = QApplication::clipboard()->text().toUtf8();
    if (data.isEmpty()) {
        return;
    }

    QGraphicsItem *itemAtScenePos = scene()->itemAt(sceneDropPoint, graphicsView()->transform());
    while (itemAtScenePos && itemAtScenePos->type() != IVFunctionGraphicsItem::Type) {
        itemAtScenePos = itemAtScenePos->parentItem();
    }
    ivm::IVFunctionType *parentObject = gi::functionObject(itemAtScenePos);
    auto cmdImport = new cmd::CmdEntitiesImport(
            data, parentObject, d->objectsModel, sceneDropPoint, QFileInfo(path()).absolutePath());
    d->commandsStack->push(cmdImport);
}

void InterfaceDocument::pasteItems()
{
    const QPoint viewportCursorPos = graphicsView()->viewport()->mapFromGlobal(QCursor::pos());
    QPointF sceneDropPoint;
    if (graphicsView()->viewport()->rect().marginsRemoved(kRootMargins.toMargins()).contains(viewportCursorPos)) {
        sceneDropPoint = graphicsView()->mapToScene(viewportCursorPos);
    }
    pasteItems(sceneDropPoint);
}

void InterfaceDocument::showContextMenuForIVModel(const QPoint &pos)
{
    const QModelIndex idx = d->objectsView->indexAt(pos);
    if (!idx.isValid()) {
        return;
    }

    const auto obj =
            d->objectsModel->getObject(idx.data(static_cast<int>(ive::CommonVisualizationModel::IdRole)).toUuid());
    if (!obj) {
        return;
    }
    QList<QAction *> actions;

    if (obj->type() == ivm::IVObject::Type::Function || obj->type() == ivm::IVObject::Type::FunctionType) {
        QAction *actExportSelectedEntities = new QAction(tr("Export selected entities"));
        connect(actExportSelectedEntities, &QAction::triggered, this, &InterfaceDocument::exportSelectedFunctions);
        actions.append(actExportSelectedEntities);
        ActionsManager::registerAction(
                Q_FUNC_INFO, actExportSelectedEntities, "Export selected entities", "Export selected entities");
    }

    if (obj->type() == ivm::IVObject::Type::FunctionType) {
        QAction *actExportSelectedSharedType = new QAction(tr("Export component type"));
        connect(actExportSelectedSharedType, &QAction::triggered, this, &InterfaceDocument::exportSelectedType);
        actions.append(actExportSelectedSharedType);
        ActionsManager::registerAction(
                Q_FUNC_INFO, actExportSelectedSharedType, "Export component type", "Export component type");
    }

    QMenu *menu = new QMenu;
    menu->addActions(actions);
    menu->exec(d->objectsView->mapToGlobal(pos));
}

bool InterfaceDocument::exportImpl(const QString &targetDir, const QList<ivm::IVObject *> &objects)
{
    const bool ok = shared::ensureDirExists(targetDir);
    if (!ok) {
        qWarning() << "Unable to create directory" << targetDir;
        return {};
    }

    const QString exportFilePath = QString("%1/%2").arg(targetDir, kDefaultFilename);

    const QFileInfo exportFileInfo(exportFilePath);
    if (exportFileInfo.exists()) {
        qWarning() << "Current object already exported: " << exportFilePath;
        return false;
    }

    QBuffer buffer;
    if (!buffer.open(QIODevice::WriteOnly)) {
        qWarning() << "Can't open buffer for exporting:" << buffer.errorString();
        return false;
    }
    if (!XmlDocExporter::exportObjects(objects, &buffer)) {
        qWarning() << "Error during component export";
        return false;
    }
    buffer.close();

    QFile file(exportFileInfo.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Can't export file: " << file.errorString();
        return false;
    }
    file.write(buffer.buffer());
    file.close();
    if (QFile::copy(asn1FilePath(), targetDir + QDir::separator() + asn1FileName())) {
        qWarning() << "Error during ASN.1 file copying:" << asn1FilePath();
    }

    const QFileInfo ivPath(path());
    for (ivm::IVObject *object : objects) {
        if (object->type() == ivm::IVObject::Type::Function) {
            const QString subPath = QStringLiteral("work/%1").arg(object->title()).toLower();
            const QString targetExportDir { componentsLibraryPath() + QDir::separator() + subPath };
            const QString sourceExportDir { ivPath.absolutePath() + QDir::separator() + subPath };
            shared::copyDir(sourceExportDir, targetExportDir);
        }
    }

    return true;
}

bool InterfaceDocument::loadImpl(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        qWarning() << Q_FUNC_INFO << "Invalid path";
        return false;
    }

    ivm::IVXMLReader parser;
    if (!parser.readFile(path)) {
        qWarning() << parser.errorString();
        return false;
    }

    setObjects(parser.parsedObjects());
    const QVariantMap metadata = parser.metaData();
    setAsn1FileName(metadata["asn1file"].toString());
    setMscFileName(metadata["mscfile"].toString());
    return true;
}

QVector<QAction *> InterfaceDocument::initActions()
{
    if (d->tool != nullptr) {
        return d->m_toolbarActions;
    }

    auto actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);

    d->tool = new CreatorTool(this);
    connect(d->tool, &CreatorTool::created, this, [this, actionGroup]() {
        if (QAction *currentAction = actionGroup->checkedAction())
            currentAction->setChecked(false);
        d->tool->setCurrentToolType(CreatorTool::ToolType::Pointer);
    });
    connect(d->tool, &CreatorTool::functionCreated, this, &InterfaceDocument::prepareEntityNameForEditing,
            Qt::QueuedConnection);
    connect(d->tool, &CreatorTool::propertyEditorRequest, this, &InterfaceDocument::showPropertyEditor,
            Qt::QueuedConnection);
    connect(d->tool, &CreatorTool::informUser, this, &InterfaceDocument::showInfoMessage);
    connect(d->tool, &CreatorTool::copyActionTriggered, this, &InterfaceDocument::copyItems);
    connect(d->tool, &CreatorTool::cutActionTriggered, this, &InterfaceDocument::cutItems);
    connect(d->tool, &CreatorTool::pasteActionTriggered, this,
            qOverload<const QPointF &>(&InterfaceDocument::pasteItems));

    auto actCreateFunctionType = new QAction(tr("Function Type"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateFunctionType, "Function Type", "Create FunctionType object");
    actCreateFunctionType->setCheckable(true);
    actCreateFunctionType->setActionGroup(actionGroup);
    connect(actCreateFunctionType, &QAction::triggered, this, [this]() {
        d->tool->setCurrentToolType(CreatorTool::ToolType::FunctionType);
        qWarning() << Q_FUNC_INFO << "Not implemented yet.";
    });
    actCreateFunctionType->setIcon(QIcon(":/tab_interface/toolbar/icns/function_type.svg"));

    auto actCreateFunction = new QAction(tr("Function"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateFunction, "Function", "Create Function object");
    actCreateFunction->setCheckable(true);
    actCreateFunction->setActionGroup(actionGroup);
    connect(actCreateFunction, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(CreatorTool::ToolType::Function); });
    actCreateFunction->setIcon(QIcon(":/tab_interface/toolbar/icns/function.svg"));

    auto actCreateProvidedInterface = new QAction(tr("Provided Interface"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, actCreateProvidedInterface, "Provided Interface", "Create Provided Interface object");
    actCreateProvidedInterface->setCheckable(true);
    actCreateProvidedInterface->setActionGroup(actionGroup);
    connect(actCreateProvidedInterface, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(CreatorTool::ToolType::ProvidedInterface); });
    actCreateProvidedInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/pi.svg"));

    auto actCreateRequiredInterface = new QAction(tr("Required Interface"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, actCreateRequiredInterface, "Required Interface", "Create Required Interface object");
    actCreateRequiredInterface->setCheckable(true);
    actCreateRequiredInterface->setActionGroup(actionGroup);
    connect(actCreateRequiredInterface, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(CreatorTool::ToolType::RequiredInterface); });
    actCreateRequiredInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/ri.svg"));

    auto actCreateComment = new QAction(tr("Comment"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateComment, "Comment", "Create Comment object");
    actCreateComment->setCheckable(true);
    actCreateComment->setActionGroup(actionGroup);
    connect(actCreateComment, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(CreatorTool::ToolType::Comment); });
    actCreateComment->setIcon(QIcon(":/tab_interface/toolbar/icns/comment.svg"));

    auto actCreateConnection = new QAction(tr("Connection"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateConnection, "Connection", "Create Connection object");
    actCreateConnection->setCheckable(true);
    actCreateConnection->setActionGroup(actionGroup);
    connect(actCreateConnection, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(CreatorTool::ToolType::MultiPointConnection); });
    actCreateConnection->setIcon(QIcon(":/tab_interface/toolbar/icns/connection.svg"));

    d->actCreateConnectionGroup = new QAction(tr("Create Connection Group"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, d->actCreateConnectionGroup, "Connection Group", "Create Connection group");
    d->actCreateConnectionGroup->setActionGroup(actionGroup);
    connect(d->actCreateConnectionGroup, &QAction::triggered, this, [this]() { d->tool->groupSelectedItems(); });
    d->actCreateConnectionGroup->setIcon(QIcon(":/tab_interface/toolbar/icns/connection_group.svg"));

    d->actRemove = new QAction(tr("Remove"));
    ActionsManager::registerAction(Q_FUNC_INFO, d->actRemove, "Remove", "Remove selected object");
    d->actRemove->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/delete.svg")));
    d->actRemove->setEnabled(false);
    d->actRemove->setShortcut(QKeySequence::Delete);
    connect(d->actRemove, &QAction::triggered, this, [this]() { d->tool->removeSelectedItems(); });

    d->actZoomIn = new QAction(tr("Zoom In"));
    ActionsManager::registerAction(Q_FUNC_INFO, d->actZoomIn, "Zoom In", "Scale up the current scene");
    d->actZoomIn->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/zoom_in.svg")));
    d->actZoomIn->setShortcut(QKeySequence::ZoomIn);
    connect(d->actZoomIn, &QAction::triggered, this,
            [this]() { d->graphicsView->setZoom(d->graphicsView->zoom() + d->graphicsView->zoomStepPercent()); });

    d->actZoomOut = new QAction(tr("Zoom Out"));
    ActionsManager::registerAction(Q_FUNC_INFO, d->actZoomOut, "Zoom Out", "Scale down the current scene");
    d->actZoomOut->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/zoom_out.svg")));
    d->actZoomOut->setShortcut(QKeySequence::ZoomOut);
    connect(d->actZoomOut, &QAction::triggered, this,
            [this]() { d->graphicsView->setZoom(d->graphicsView->zoom() - d->graphicsView->zoomStepPercent()); });

    d->actExitToRoot = new QAction(tr("Exit to root function"));
    d->actExitToRoot->setActionGroup(actionGroup);
    d->actExitToRoot->setEnabled(false);
    connect(d->actExitToRoot, &QAction::triggered, this, [this]() { d->itemsModel->changeRootItem({}); });
    d->actExitToRoot->setIcon(QIcon(":/tab_interface/toolbar/icns/exit.svg"));

    d->actExitToParent = new QAction(tr("Exit to parent function"));
    d->actExitToParent->setActionGroup(actionGroup);
    d->actExitToParent->setEnabled(false);
    connect(d->actExitToParent, &QAction::triggered, this, [this]() {
        ivm::IVObject *parentObject =
                d->objectsModel->rootObject() ? d->objectsModel->rootObject()->parentObject() : nullptr;
        d->itemsModel->changeRootItem(parentObject ? parentObject->id() : shared::InvalidId);
    });
    d->actExitToParent->setIcon(QIcon(":/tab_interface/toolbar/icns/exit_parent.svg"));

    d->m_toolbarActions = { actCreateFunctionType, actCreateFunction, actCreateProvidedInterface,
        actCreateRequiredInterface, actCreateComment, actCreateConnection, d->actCreateConnectionGroup, d->actRemove,
        d->actZoomIn, d->actZoomOut, d->actExitToRoot, d->actExitToParent };

    connect(d->objectsModel, &ivm::IVModel::rootObjectChanged, this, [this](const shared::Id &rootId) {
        if (d->actExitToRoot) {
            d->actExitToRoot->setEnabled(nullptr != d->objectsModel->rootObject());
        }
        if (d->actExitToParent) {
            d->actExitToParent->setEnabled(nullptr != d->objectsModel->rootObject());
        }

        if (const QGraphicsItem *item = d->itemsModel->getItem(rootId)) {
            d->graphicsView->centerOn(item->sceneBoundingRect().center());
        }
    });
    connect(d->objectsSelectionModel, &QItemSelectionModel::selectionChanged, this,
            [this](const QItemSelection &selected, const QItemSelection & /*deselected*/) {
                d->actRemove->setEnabled(!selected.isEmpty());
                const QModelIndexList idxs = selected.indexes();
                auto it = std::find_if(idxs.cbegin(), idxs.cend(), [](const QModelIndex &index) {
                    return index.data(static_cast<int>(ive::CommonVisualizationModel::TypeRole)).toInt()
                            == static_cast<int>(ivm::IVObject::Type::Connection);
                });
                d->actCreateConnectionGroup->setEnabled(it != std::cend(idxs));
            });
    return d->m_toolbarActions;
}

QWidget *InterfaceDocument::createGraphicsView()
{
    if (!d->graphicsView) {
        d->graphicsView = new ive::GraphicsView;
        connect(d->graphicsView, &ive::GraphicsView::zoomChanged, this, [this](qreal percent) {
            d->itemsModel->zoomChanged();
            d->actZoomIn->setEnabled(!qFuzzyCompare(percent, d->graphicsView->maxZoomPercent()));
            d->actZoomOut->setEnabled(!qFuzzyCompare(percent, d->graphicsView->minZoomPercent()));
        });

        connect(d->graphicsView, &GraphicsView::importEntity, this, &InterfaceDocument::importEntity);
        connect(d->graphicsView, &GraphicsView::instantiateEntity, this, &InterfaceDocument::instantiateEntity);
        connect(d->graphicsView, &GraphicsView::copyItems, this, &InterfaceDocument::copyItems);
        connect(d->graphicsView, &GraphicsView::cutItems, this, &InterfaceDocument::cutItems);
        connect(d->graphicsView, &GraphicsView::pasteItems, this, qOverload<>(&InterfaceDocument::pasteItems));
    }
    d->graphicsView->setScene(d->itemsModel->scene());
    d->graphicsView->setUpdatesEnabled(false);
    d->itemsModel->updateSceneRect();
    d->graphicsView->setUpdatesEnabled(true);
    return d->graphicsView;
}

QTreeView *InterfaceDocument::createModelView()
{
    if (d->objectsView)
        return d->objectsView;

    d->objectsView = new QTreeView;
    d->objectsView->setObjectName(QLatin1String("IVModelView"));
    d->objectsView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    d->objectsView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    d->objectsView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(d->objectsView, &QTreeView::customContextMenuRequested, this,
            &InterfaceDocument::showContextMenuForIVModel);

    d->objectsVisualizationModel = new VisualizationModel(d->objectsModel, d->commandsStack, d->objectsView);
    auto headerItem = new QStandardItem(tr("IV Structure"));
    headerItem->setTextAlignment(Qt::AlignCenter);
    d->objectsVisualizationModel->setHorizontalHeaderItem(0, headerItem);
    d->objectsView->setModel(d->objectsVisualizationModel);

    d->objectsSelectionModel = new QItemSelectionModel(d->objectsVisualizationModel, d->objectsView);
    connect(d->objectsSelectionModel, &QItemSelectionModel::selectionChanged, this,
            &InterfaceDocument::onViewSelectionChanged);
    d->objectsView->setSelectionModel(d->objectsSelectionModel);

    return d->objectsView;
}

QTreeView *InterfaceDocument::createImportView()
{
    if (d->importView)
        return d->importView;

    d->importView = new ObjectsTreeView(shared::DropType::ImportableType);
    d->importView->setObjectName(QLatin1String("ImportView"));
    d->importView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    d->importView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    auto sourceModel = new CommonVisualizationModel(d->importModel, d->commandsStack, d->importView);
    auto headerItem = new QStandardItem(tr("Import Component"));
    headerItem->setTextAlignment(Qt::AlignCenter);
    sourceModel->setHorizontalHeaderItem(0, headerItem);
    d->importView->setModel(sourceModel);

    return d->importView;
}

QTreeView *InterfaceDocument::createSharedView()
{
    if (d->sharedView)
        return d->sharedView;

    d->sharedView = new ObjectsTreeView(shared::DropType::InstantiatableType);
    d->sharedView->setObjectName(QLatin1String("SharedView"));
    d->sharedView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    d->sharedView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    auto sourceModel = new CommonVisualizationModel(d->sharedModel, d->commandsStack, d->sharedView);
    auto headerItem = new QStandardItem(tr("Shared Types"));
    headerItem->setTextAlignment(Qt::AlignCenter);
    sourceModel->setHorizontalHeaderItem(0, headerItem);
    d->sharedView->setModel(sourceModel);

    return d->sharedView;
}

void InterfaceDocument::showNIYGUI(const QString &title)
{
    QString header = title.isEmpty() ? "NIY" : title;
    QMessageBox::information(window(), header, "Not implemented yet!");
}

QWidget *InterfaceDocument::window()
{
    if (d->view) {
        return d->view->window();
    }
    return nullptr;
}

void InterfaceDocument::onSceneSelectionChanged(const QList<shared::Id> &selectedObjects)
{
    QItemSelection itemSelection;
    for (auto id : selectedObjects) {
        const QModelIndex idx = d->objectsVisualizationModel->indexFromItem(d->objectsVisualizationModel->getItem(id));
        if (itemSelection.isEmpty()) {
            itemSelection.select(idx, idx);
        } else {
            itemSelection.merge(QItemSelection(idx, idx), QItemSelectionModel::SelectCurrent);
        }
    }
    d->objectsSelectionModel->select(itemSelection,
            QItemSelectionModel::Rows | QItemSelectionModel::Current | QItemSelectionModel::ClearAndSelect);
}

void InterfaceDocument::onViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    auto updateSelection = [this](const QItemSelection &selection, bool value) {
        for (const QModelIndex &idx : selection.indexes()) {
            if (auto graphicsItem = d->itemsModel->getItem(idx.data(CommonVisualizationModel::IdRole).toUuid())) {
                graphicsItem->setSelected(value);
            }
        }
    };

    updateSelection(deselected, false);
    updateSelection(selected, true);
}

}
