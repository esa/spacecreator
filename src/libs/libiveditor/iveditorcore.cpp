/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "iveditorcore.h"

#include "commandlineparser.h"
#include "commands/cmdchangeasn1file.h"
#include "commands/cmdconnectionitemcreate.h"
#include "commands/cmdentityattributeschange.h"
#include "commands/cmdfunctionitemcreate.h"
#include "commands/cmdifaceattrchange.h"
#include "commands/cmdinterfaceitemcreate.h"
#include "commandsstack.h"
#include "connectioncreationvalidator.h"
#include "context/action/actionsmanager.h"
#include "graphicsviewutils.h"
#include "interfacedocument.h"
#include "itemeditor/common/ivutils.h"
#include "itemeditor/graphicsview.h"
#include "ivappwidget.h"
#include "ivconnection.h"
#include "ivcore/abstractsystemchecks.h"
#include "ivcreatortool.h"
#include "ivexporter.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "ivpropertytemplateconfig.h"

#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QImageWriter>
#include <QMainWindow>
#include <QMenu>
#include <QRectF>
#include <QToolBar>
#include <QUndoStack>
#include <QUrl>

namespace ive {

/*!
 * \namespace ive
 * \brief Has most of the code for the Interface View Editor UI
 */

IVEditorCore::IVEditorCore(QObject *parent)
    : shared::EditorCore(parent)
    , m_document(new ive::InterfaceDocument(this))
{
    m_document->init();

    if (ivm::IVModel *model = document()->objectsModel()) {
        connect(model, &ivm::IVModel::objectsAdded, this, &ive::IVEditorCore::updateIVItems);
        connect(model, &ivm::IVModel::objectRemoved, this, &ive::IVEditorCore::updateIVItems);
        connect(model, &ivm::IVModel::rootObjectChanged, this, &ive::IVEditorCore::updateIVItems);
    }
}

IVEditorCore::~IVEditorCore() { }

/*!
   Returns the interface document
 */
InterfaceDocument *IVEditorCore::document() const
{
    return m_document;
}

shared::ui::GraphicsViewBase *IVEditorCore::chartView()
{
    mainwidget();
    return m_mainWidget->graphicsView();
}

QWidget *IVEditorCore::mainwidget()
{
    if (!m_mainWidget) {
        m_mainWidget = new IVAppWidget(this);
    }
    return m_mainWidget;
}

void IVEditorCore::addToolBars(QMainWindow *window)
{
    window->addToolBar(mainToolBar());
}

void IVEditorCore::registerBasicActions()
{
    ive::ActionsManager::registerAction(Q_FUNC_INFO, actionUndo(), "Undo", "Undo the last operation");
    ive::ActionsManager::registerAction(Q_FUNC_INFO, actionRedo(), "Redo", "Redo the last undone operation");
    ive::ActionsManager::registerAction(
            Q_FUNC_INFO, actionExportFunctions(), "Export Functions", "Export selected objects");
    ive::ActionsManager::registerAction(
            Q_FUNC_INFO, actionExportType(), "Export Type", "Export selected component type");
}

void IVEditorCore::populateCommandLineArguments(shared::CommandLineParser *parser) const
{
    parser->handlePositional(shared::CommandLineParser::Positional::OpenXMLFile);
    parser->handlePositional(shared::CommandLineParser::Positional::OpenStringTemplateFile);
    parser->handlePositional(shared::CommandLineParser::Positional::ExportToFile);
    parser->handlePositional(shared::CommandLineParser::Positional::ListScriptableActions);
    parser->handlePositional(shared::CommandLineParser::Positional::DropUnsavedChangesSilently);
}

QAction *IVEditorCore::actionExportFunctions()
{
    if (m_actionExportFunctions == nullptr) {
        m_actionExportFunctions =
                new QAction(QIcon(":/toolbar/icns/export_selected.svg"), tr("Export selected entity"), this);
        connect(m_actionExportFunctions, &QAction::triggered, m_document, &InterfaceDocument::exportSelectedFunctions);
    }
    return m_actionExportFunctions;
}

QAction *IVEditorCore::actionExportType()
{
    if (m_actionExportType == nullptr) {
        m_actionExportType =
                new QAction(QIcon(":/toolbar/icns/export_component_type.svg"), tr("Export component type"), this);
        connect(m_actionExportType, &QAction::triggered, m_document, &InterfaceDocument::exportSelectedType);
    }
    return m_actionExportType;
}

QAction *IVEditorCore::actionToggleE2EView()
{
    if (m_actionToggleE2EView == nullptr) {
        m_actionToggleE2EView =
                new QAction(QIcon(QIcon(":/toolbar/icns/end_to_end.png")), tr("Show end to end dataflow"), this);
        m_actionToggleE2EView->setCheckable(true);
    }
    return m_actionToggleE2EView;
}

/*!
   Adds an iv function with the given \p name
   @return Returns the newly created function. Or nullptr if the creation failed.
 */
ivm::IVFunction *IVEditorCore::addFunction(const QString &name, ivm::IVFunction *parent)
{
    auto cmd = new cmd::CmdFunctionItemCreate(m_document->objectsModel(), parent,
            QRectF(QPointF(10., 10.), shared::graphicsviewutils::kDefaultGraphicsItemSize), name);
    bool ok = commandsStack()->push(cmd);
    if (ok) {
        Q_EMIT editedExternally(this);
        return cmd->createdFunction();
    }

    return nullptr;
}

/*!
   Adds the connection and the needed interfaces between the given instances
 */
bool IVEditorCore::addConnection(QString name, const QString &fromInstanceName, const QString &toInstanceName)
{
    if (fromInstanceName == toInstanceName) {
        return false;
    }

    if (fromInstanceName.isEmpty()) {
        return addInterface(name, toInstanceName) != nullptr;
    }

    if (toInstanceName.isEmpty()) {
        return addInterface(name, fromInstanceName) != nullptr;
    }

    ivm::IVModel *ivModel = m_document->objectsModel();
    if (ivModel->getConnection(name, fromInstanceName, toInstanceName, m_caseCheck) != nullptr) {
        // The connection exists already
        return false;
    }

    ivm::IVFunction *fromFunc = ivModel->getFunction(fromInstanceName, m_caseCheck);
    ivm::IVFunction *toFunc = ivModel->getFunction(toInstanceName, m_caseCheck);

    if (!fromFunc || !toFunc) {
        qDebug() << Q_FUNC_INFO << "No function for the connection" << name;
        return false;
    }

    ivm::IVInterface *fromInterface = getInterface(name, ivm::IVInterface::InterfaceType::Required, fromFunc);
    ivm::IVInterface *toInterface = getInterface(name, ivm::IVInterface::InterfaceType::Provided, toFunc);

    if (fromInterface && toInterface) {
        cmd::CommandsStack::Macro cmdMacro(commandsStack(), tr("Create connection"));

        auto createConnection = [ivModel, this](
                                        ivm::IVFunction *parent, ivm::IVInterface *inIf, ivm::IVInterface *outIf) {
            QVector<QPointF> points;
            points.append(shared::graphicsviewutils::pos(inIf->coordinates()));
            points.append(shared::graphicsviewutils::pos(outIf->coordinates()));
            auto command = new cmd::CmdConnectionItemCreate(ivModel, parent, inIf->id(), outIf->id(), points);
            commandsStack()->push(command);
        };

        // find all (nested/parent) functions for "source" and "target" connections
        QList<ivm::IVFunction *> fromFunctions = fromInterface->functionsStack();
        QList<ivm::IVFunction *> toFunctions = toInterface->functionsStack();
        ivm::IVFunction *firstCommonFunction = nullptr;
        while (!fromFunctions.isEmpty() && !toFunctions.isEmpty() && fromFunctions.last() == toFunctions.last()) {
            firstCommonFunction = fromFunctions.last();
            fromFunctions.takeLast();
            toFunctions.takeLast();
        }

        // Create all connections on the "source" side
        while (fromFunctions.size() > 1) {
            ivm::IVFunction *parentFunc = fromFunctions[1];
            ivm::IVInterface *sourceIf = fromInterface;
            ivm::IVInterface *targetIf = getInterface(name, ivm::IVInterface::InterfaceType::Required, parentFunc);

            createConnection(parentFunc, sourceIf, targetIf);

            fromFunctions.takeFirst();
            fromInterface = targetIf;
        }

        // Create all connections on the "target" side
        while (toFunctions.size() > 1) {
            ivm::IVFunction *parentFunc = toFunctions[1];
            ivm::IVInterface *sourceIf = toInterface;
            ivm::IVInterface *targetIf = getInterface(name, ivm::IVInterface::InterfaceType::Provided, parentFunc);

            createConnection(parentFunc, sourceIf, targetIf);

            toFunctions.takeFirst();
            toInterface = targetIf;
        }

        // Create the connection that connects "source" to "target"
        createConnection(firstCommonFunction, fromInterface, toInterface);

        cmdMacro.setComplete(true);
    }

    Q_EMIT editedExternally(this);
    return true;
}

/*!
   Adds an interface with name \p name to the function \p functionName to the model.
   Returns the newly returned interface, or nullptr in case of an error.
 */
ivm::IVInterface *IVEditorCore::addInterface(QString name, const QString &functionName)
{
    ivm::IVModel *ivModel = m_document->objectsModel();
    ivm::IVFunction *func = ivModel->getFunction(functionName, m_caseCheck);
    if (!func) {
        qDebug() << Q_FUNC_INFO << "No function" << functionName << "for the interface" << name;
        return nullptr;
    }

    if (func->hasInterface(name, m_caseCheck)) {
        qDebug() << Q_FUNC_INFO << "Function" << functionName << "already has an interface named" << name;
        return nullptr;
    }

    ivm::IVInterface::CreationInfo ci;
    ci.kind = ivm::IVInterface::OperationKind::Cyclic;
    ci.type = ivm::IVInterface::InterfaceType::Provided;
    ci.function = func;
    ci.model = ivModel;
    ci.name = name;

    auto command = new cmd::CmdInterfaceItemCreate(ci);
    commandsStack()->push(command);

    Q_EMIT editedExternally(this);

    return command->createdInterface();
}

/*!
   Renames the function with current name \p oldName to \p newName.
   If \p updateSystem is true, the user is asked to check the .msc models to be updated as well
 */
bool IVEditorCore::renameIVFunction(const QString &oldName, const QString &newName)
{
    ivm::IVModel *ivModel = m_document->objectsModel();
    ivm::IVFunction *ivFunc = ivModel->getFunction(oldName, m_caseCheck);
    if (!ivFunc) {
        return false;
    }

    const QList<EntityAttribute> attributess = { EntityAttribute {
            ivm::meta::Props::token(ivm::meta::Props::Token::name), newName, EntityAttribute::Type::Attribute } };
    auto cmd =
            new shared::cmd::CmdEntityAttributesChange(ivm::IVPropertyTemplateConfig::instance(), ivFunc, attributess);
    commandsStack()->push(cmd);

    Q_EMIT editedExternally(this);
    return true;
}

/*!
   \brief IVEditorCore::renameIVConnection
   \param oldName
   \param newName
   \param fromInstanceName
   \param toInstanceName
   \return
 */
bool IVEditorCore::renameIVConnection(
        const QString &oldName, const QString &newName, const QString &fromInstanceName, const QString &toInstanceName)
{
    if (fromInstanceName.isEmpty()) {
        return renameCyclicInterface(oldName, newName, toInstanceName);
    }
    if (toInstanceName.isEmpty()) {
        return renameCyclicInterface(oldName, newName, fromInstanceName);
    }

    ivm::IVModel *ivModel = m_document->objectsModel();
    ivm::IVConnection *ivConnect = ivModel->getConnection(oldName, fromInstanceName, toInstanceName, m_caseCheck);
    if (!ivConnect) {
        return false;
    }

    auto cmd = new cmd::CmdIfaceAttrChange(ivm::IVPropertyTemplateConfig::instance(), ivConnect->targetInterface(),
            ivm::meta::Props::token(ivm::meta::Props::Token::name), QVariant::fromValue(newName));
    commandsStack()->push(cmd);

    Q_EMIT editedExternally(this);
    return true;
}

bool IVEditorCore::renameCyclicInterface(const QString &oldName, const QString &newName, const QString &functionName)
{
    ivm::IVModel *ivModel = m_document->objectsModel();
    ivm::IVFunction *ivFunc = ivModel->getFunction(functionName, m_caseCheck);
    if (!ivFunc) {
        qDebug() << Q_FUNC_INFO << "No function" << functionName << "to rename the interface from" << newName << "to"
                 << oldName;
        return false;
    }

    ivm::IVInterface *interface =
            ivModel->getIfaceByName(oldName, ivm::IVInterface::InterfaceType::Required, ivFunc, m_caseCheck);
    if (!interface) {
        interface = ivModel->getIfaceByName(oldName, ivm::IVInterface::InterfaceType::Provided, ivFunc, m_caseCheck);
    }
    if (!interface) {
        qDebug() << Q_FUNC_INFO << "No interface" << oldName << "to rname for function" << functionName;
        return false;
    }

    auto cmd = new cmd::CmdIfaceAttrChange(ivm::IVPropertyTemplateConfig::instance(), interface,
            ivm::meta::Props::token(ivm::meta::Props::Token::name), QVariant::fromValue(newName));
    commandsStack()->push(cmd);

    Q_EMIT editedExternally(this);
    return true;
}

QUndoStack *IVEditorCore::undoStack() const
{
    Q_ASSERT(m_document && m_document->undoStack());
    return m_document->undoStack();
}

cmd::CommandsStack *IVEditorCore::commandsStack() const
{
    Q_ASSERT(m_document && m_document->commandsStack());
    return m_document->commandsStack();
}

/*!
   Changes the asn1 reference to \p newName if the existing one is pointing to \p oldName
 */
bool IVEditorCore::renameAsnFile(const QString &oldName, const QString &newName)
{
    QFileInfo oldFile(oldName);
    const QString oldFileName = oldFile.fileName();

    if (document()->asn1FileName() == oldFileName) {
        auto command = new cmd::CmdChangeAsn1File(document(), newName);
        commandsStack()->push(command);
        Q_EMIT editedExternally(this);
        return true;
    }

    return false;
}

QString IVEditorCore::filePath() const
{
    return m_document->path();
}

bool IVEditorCore::save()
{
    return m_document->exporter()->exportDocSilently(m_document);
}

QVector<ivm::IVFunction *> IVEditorCore::allIVFunctions() const
{
    return m_ivFunctions;
}

QVector<ivm::IVConnection *> IVEditorCore::allIVConnections() const
{
    return m_ivConnections;
}

/*!
   Returns a list of the names of all functions in the iv model
 */
QStringList IVEditorCore::ivFunctionsNames() const
{
    QStringList functionNames;
    for (const ivm::IVFunction *ivFunction : m_ivFunctions) {
        if (ivFunction && !ivFunction->title().isEmpty()) {
            functionNames << ivFunction->title();
        }
    }
    return functionNames;
}

/*!
   Returns a list of the names of all connections in the iv model
 */
QStringList IVEditorCore::ivConnectionNames() const
{
    QStringList connectionNames;
    for (const ivm::IVConnection *ivConnection : m_ivConnections) {
        if (ivConnection && !ivConnection->name().isEmpty()) {
            connectionNames << ivConnection->name();
        }
    }
    connectionNames.removeDuplicates();
    return connectionNames;
}

void IVEditorCore::setDvChecks(ivm::AbstractSystemChecks *checks)
{
    m_checks = checks;
}

ivm::AbstractSystemChecks *IVEditorCore::dvChecks() const
{
    return m_checks;
}

void IVEditorCore::centerOnView()
{
    mainwidget();
    m_mainWidget->centerView();
}

/*!
 * \brief Return the list of image formats which the Qt is available to write.
 */
static QStringList supportedImgFileExtensions()
{
    QStringList extensions;
    const QList<QByteArray> &formats = QImageWriter::supportedImageFormats();
    for (const QByteArray &format : formats)
        extensions << ("*." + format.toLower());
    return extensions;
}

/*!
 * \brief Handler for render scene request.
 */
void IVEditorCore::onSaveRenderRequested()
{
    const QString defaultFileName = QString("%1.png").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy_HH-mm-ss"));
    const QStringList &extensions = supportedImgFileExtensions();

    QFileDialog dialog(nullptr, tr("Save screenshot..."), defaultFileName, extensions.join(";; "));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(".png");
    if (dialog.exec() == QDialog::Accepted) {
        const QString fileName = dialog.selectedUrls().value(0).toLocalFile();
        saveSceneRender(fileName);
    }
}

void IVEditorCore::saveSceneRender(const QString &filePath) const
{
    if (filePath.isEmpty())
        return;

    if (QGraphicsScene *scene = document()->scene()) {
        QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
        img.fill(Qt::transparent);
        QPainter p(&img);
        scene->render(&p);
        p.end();
        img.save(filePath);
    }
}

/*!
   Get the interface, or creates it if it does not exist. In case the \p parentFunction is a nullptr, a nullptr is
   returned
 */
ivm::IVInterface *IVEditorCore::getInterface(
        const QString &ifName, ivm::IVInterface::InterfaceType ifType, ivm::IVFunction *parentFunction)
{
    if (parentFunction == nullptr) {
        return {};
    }

    ivm::IVModel *ivModel = m_document->objectsModel();
    if (!ivModel) {
        return {};
    }

    ivm::IVInterface *interface = ivModel->getIfaceByName(ifName, ifType, parentFunction, m_caseCheck);
    if (!interface) {
        // create the interface
        ivm::IVInterface::CreationInfo createInfo(ivModel, parentFunction);
        createInfo.name = ifName;
        createInfo.type = ifType;
        QRectF funcRect = shared::graphicsviewutils::rect(parentFunction->coordinates());
        QPointF ifPos(funcRect.left(), funcRect.center().y());
        if (ifType == ivm::IVInterface::InterfaceType::Required) {
            ifPos.setX(funcRect.right());
        }
        createInfo.position = ifPos;
        auto *command = new cmd::CmdInterfaceItemCreate(createInfo);
        commandsStack()->push(command);
        interface = ivModel->getIfaceByName(ifName, ifType, parentFunction, m_caseCheck);
    }
    return interface;
}

/*!
   Updates the list of functions and connections from the iv model
 */
void IVEditorCore::updateIVItems()
{
    m_ivFunctions.clear();
    m_ivConnections.clear();

    ivm::IVModel *ivModel = m_document->objectsModel();
    if (!ivModel) {
        return;
    }

    m_ivFunctions = ivModel->allObjectsByType<ivm::IVFunction>();
    m_ivConnections = ivModel->allObjectsByType<ivm::IVConnection>();
}

QUrl IVEditorCore::helpPage() const
{
    return QUrl("https://taste.tuxfamily.org/wiki/index.php?title=Interface_view_tutorial");
}

}
