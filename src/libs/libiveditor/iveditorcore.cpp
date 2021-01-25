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

#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"
#include "baseitems/common/aadlutils.h"
#include "commandlineparser.h"
#include "commandsstack.h"
#include "connectioncreationvalidator.h"
#include "context/action/actionsmanager.h"
#include "interface/commands/cmdconnectionitemcreate.h"
#include "interface/commands/cmdentityattributechange.h"
#include "interface/commands/cmdfunctionitemcreate.h"
#include "interface/commands/cmdifaceattrchange.h"
#include "interface/commands/cmdinterfaceitemcreate.h"
#include "interface/commands/commandsfactory.h"
#include "interface/creatortool.h"
#include "interface/interfacedocument.h"
#include "xmldocexporter.h"

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

namespace ive {

/*!
 * \namespace ive
 * \brief Has most of the code for the Interface View Editor UI
 */

IVEditorCore::IVEditorCore(QObject *parent)
    : shared::EditorCore(parent)
    , m_document(new ive::InterfaceDocument(this))
    , m_docToolBar(new QToolBar)
{
    m_document->init();

    m_docToolBar->setObjectName("Document ToolBar");
    m_docToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    m_docToolBar->setMovable(true);

    if (ivm::AADLObjectsModel *model = document()->objectsModel()) {
        connect(model, &ivm::AADLObjectsModel::aadlObjectsAdded, this, &ive::IVEditorCore::updateAadlItems);
        connect(model, &ivm::AADLObjectsModel::aadlObjectRemoved, this, &ive::IVEditorCore::updateAadlItems);
        connect(model, &ivm::AADLObjectsModel::rootObjectChanged, this, &ive::IVEditorCore::updateAadlItems);
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
    return m_document->graphicsView();
}

void IVEditorCore::addToolBars(QMainWindow *window)
{
    window->addToolBar(mainToolBar());
    window->addToolBar(m_docToolBar);
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
    parser->handlePositional(shared::CommandLineParser::Positional::OpenAADLXMLFile);
    parser->handlePositional(shared::CommandLineParser::Positional::OpenStringTemplateFile);
    parser->handlePositional(shared::CommandLineParser::Positional::ExportToFile);
    parser->handlePositional(shared::CommandLineParser::Positional::ListScriptableActions);
    parser->handlePositional(shared::CommandLineParser::Positional::DropUnsavedChangesSilently);
}

QAction *IVEditorCore::actionExportFunctions()
{
    if (m_actionExportFunctions == nullptr) {
        m_actionExportFunctions = new QAction(tr("Export selected entity"), this);
    }
    return m_actionExportFunctions;
}

QAction *IVEditorCore::actionExportType()
{
    if (m_actionExportType == nullptr) {
        m_actionExportType = new QAction(tr("Export component type"), this);
    }
    return m_actionExportType;
}

QAction *IVEditorCore::actionToggleE2EView()
{
    if (m_actionToggleE2EView == nullptr) {
        m_actionToggleE2EView = new QAction(tr("&Show end to end dataflow"), this);
        m_actionToggleE2EView->setCheckable(true);
    }
    return m_actionToggleE2EView;
}

/*!
   Adds an aadl function with the given \p name
   @return Returns the newly created function. Or nullptr if the creation failed.
 */
ivm::AADLObjectFunction *IVEditorCore::addFunction(const QString &name, ivm::AADLObjectFunction *parent)
{
    auto cmd = new cmd::CmdFunctionItemCreate(
            m_document->objectsModel(), parent, QRectF(QPointF(10., 10.), DefaultGraphicsItemSize), name);
    bool ok = cmd::CommandsStack::push(cmd);
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

    ivm::AADLObjectsModel *aadlModel = m_document->objectsModel();
    if (aadlModel->getConnection(name, fromInstanceName, toInstanceName, m_caseCheck) != nullptr) {
        // The connection exists already
        return false;
    }

    ivm::AADLObjectFunction *fromFunc = aadlModel->getFunction(fromInstanceName, m_caseCheck);
    ivm::AADLObjectFunction *toFunc = aadlModel->getFunction(toInstanceName, m_caseCheck);

    if (!fromFunc || !toFunc) {
        qDebug() << Q_FUNC_INFO << "No function for the connection" << name;
        return false;
    }

    ivm::AADLObjectIface *fromInterface = getInterface(name, ivm::AADLObjectIface::IfaceType::Required, fromFunc);
    ivm::AADLObjectIface *toInterface = getInterface(name, ivm::AADLObjectIface::IfaceType::Provided, toFunc);

    if (fromInterface && toInterface) {
        cmd::CommandsStack::Macro cmdMacro(tr("Create connection"));

        auto createConnection = [aadlModel](ivm::AADLObjectFunction *parent, ivm::AADLObjectIface *inIf,
                                        ivm::AADLObjectIface *outIf) {
            QVector<QPointF> points;
            points.append(ive::pos(inIf->coordinates()));
            points.append(ive::pos(outIf->coordinates()));
            auto command = new cmd::CmdConnectionItemCreate(aadlModel, parent, inIf->id(), outIf->id(), points);
            cmd::CommandsStack::push(command);
        };

        // find all (nested/parent) functions for "source" and "target" connections
        QList<ivm::AADLObjectFunction *> fromFunctions = fromInterface->functionsStack();
        QList<ivm::AADLObjectFunction *> toFunctions = toInterface->functionsStack();
        ivm::AADLObjectFunction *firstCommonFunction = nullptr;
        while (!fromFunctions.isEmpty() && !toFunctions.isEmpty() && fromFunctions.last() == toFunctions.last()) {
            firstCommonFunction = fromFunctions.last();
            fromFunctions.takeLast();
            toFunctions.takeLast();
        }

        // Create all connections on the "source" side
        while (fromFunctions.size() > 1) {
            ivm::AADLObjectFunction *parentFunc = fromFunctions[1];
            ivm::AADLObjectIface *sourceIf = fromInterface;
            ivm::AADLObjectIface *targetIf =
                    getInterface(name, ivm::AADLObjectIface::IfaceType::Required, parentFunc);

            createConnection(parentFunc, sourceIf, targetIf);

            fromFunctions.takeFirst();
            fromInterface = targetIf;
        }

        // Create all connections on the "target" side
        while (toFunctions.size() > 1) {
            ivm::AADLObjectFunction *parentFunc = toFunctions[1];
            ivm::AADLObjectIface *sourceIf = toInterface;
            ivm::AADLObjectIface *targetIf =
                    getInterface(name, ivm::AADLObjectIface::IfaceType::Provided, parentFunc);

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
ivm::AADLObjectIface *IVEditorCore::addInterface(QString name, const QString &functionName)
{
    ivm::AADLObjectsModel *aadlModel = m_document->objectsModel();
    ivm::AADLObjectFunction *func = aadlModel->getFunction(functionName, m_caseCheck);
    if (!func) {
        qDebug() << Q_FUNC_INFO << "No function" << functionName << "for the interface" << name;
        return nullptr;
    }

    if (func->hasInterface(name, m_caseCheck)) {
        qDebug() << Q_FUNC_INFO << "Function" << functionName << "already has an interface named" << name;
        return nullptr;
    }

    ivm::AADLObjectIface::CreationInfo ci;
    ci.kind = ivm::AADLObjectIface::OperationKind::Cyclic;
    ci.type = ivm::AADLObjectIface::IfaceType::Provided;
    ci.function = func;
    ci.model = aadlModel;
    ci.name = name;

    auto command = new cmd::CmdInterfaceItemCreate(ci);
    cmd::CommandsStack::push(command);

    Q_EMIT editedExternally(this);

    return command->createdInterface();
}

/*!
   Renames the function with current name \p oldName to \p newName.
   If \p updateSystem is true, the user is asked to check the .msc models to be updated as well
 */
bool IVEditorCore::renameAadlFunction(const QString &oldName, const QString &newName)
{
    ivm::AADLObjectsModel *aadlModel = m_document->objectsModel();
    ivm::AADLObjectFunction *aadlFunc = aadlModel->getFunction(oldName, m_caseCheck);
    if (!aadlFunc) {
        return false;
    }

    const QVariantHash attributess = { { ivm::meta::Props::token(ivm::meta::Props::Token::name), newName } };
    auto cmd = new cmd::CmdEntityAttributeChange(aadlFunc, attributess);
    cmd::CommandsStack::push(cmd);

    Q_EMIT editedExternally(this);
    return true;
}

/*!
   \brief IVEditorCore::addAadlConnection
   \param oldName
   \param newName
   \param fromInstanceName
   \param toInstanceName
   \return
 */
bool IVEditorCore::renameAadlConnection(
        const QString &oldName, const QString &newName, const QString &fromInstanceName, const QString &toInstanceName)
{
    if (fromInstanceName.isEmpty()) {
        return renameCyclicInterface(oldName, newName, toInstanceName);
    }
    if (toInstanceName.isEmpty()) {
        return renameCyclicInterface(oldName, newName, fromInstanceName);
    }

    ivm::AADLObjectsModel *aadlModel = m_document->objectsModel();
    ivm::AADLObjectConnection *aadlConnect =
            aadlModel->getConnection(oldName, fromInstanceName, toInstanceName, m_caseCheck);
    if (!aadlConnect) {
        return false;
    }

    auto cmd = new cmd::CmdIfaceAttrChange(aadlConnect->targetInterface(),
            ivm::meta::Props::token(ivm::meta::Props::Token::name), QVariant::fromValue(newName));
    cmd::CommandsStack::push(cmd);

    Q_EMIT editedExternally(this);
    return true;
}

bool IVEditorCore::renameCyclicInterface(const QString &oldName, const QString &newName, const QString &functionName)
{
    ivm::AADLObjectsModel *aadlModel = m_document->objectsModel();
    ivm::AADLObjectFunction *func = aadlModel->getFunction(functionName, m_caseCheck);
    if (!func) {
        qDebug() << Q_FUNC_INFO << "No function" << functionName << "to rename the interface from" << newName << "to"
                 << oldName;
        return false;
    }

    ivm::AADLObjectIface *interface =
            aadlModel->getIfaceByName(oldName, ivm::AADLObjectIface::IfaceType::Required, func, m_caseCheck);
    if (!interface) {
        interface = aadlModel->getIfaceByName(oldName, ivm::AADLObjectIface::IfaceType::Provided, func, m_caseCheck);
    }
    if (!interface) {
        qDebug() << Q_FUNC_INFO << "No interface" << oldName << "to rname for function" << functionName;
        return false;
    }

    auto cmd = new cmd::CmdIfaceAttrChange(
            interface, ivm::meta::Props::token(ivm::meta::Props::Token::name), QVariant::fromValue(newName));
    cmd::CommandsStack::push(cmd);

    Q_EMIT editedExternally(this);
    return true;
}

QUndoStack *IVEditorCore::undoStack() const
{
    return m_document->commandsStack();
}

cmd::CommandsStack *IVEditorCore::commandsStack() const
{
    return static_cast<cmd::CommandsStack *>(cmd::CommandsStack::instance());
}

/*!
   Changes the asn1 reference to \p newName if the existing one is pointing to \p oldName
 */
bool IVEditorCore::renameAsnFile(const QString &oldName, const QString &newName)
{
    QFileInfo oldFile(oldName);
    const QString oldFileName = oldFile.fileName();

    if (document()->asn1FileName() == oldFileName) {
        QVariantList params { QVariant::fromValue(document()), QVariant::fromValue(newName) };
        QUndoCommand *command = cmd::CommandsFactory::create(cmd::ChangeAsn1File, params);
        if (command) {
            cmd::CommandsStack::push(command);
            Q_EMIT editedExternally(this);
            return true;
        }
    }

    return false;
}

QString IVEditorCore::filePath() const
{
    return m_document->path();
}

bool IVEditorCore::save()
{
    return ive::XmlDocExporter::exportDocSilently(m_document, {}, {});
}

QVector<ivm::AADLObjectFunction *> IVEditorCore::allAadlFunctions() const
{
    return m_aadlFunctions;
}

QVector<ivm::AADLObjectConnection *> IVEditorCore::allAadlConnections() const
{
    return m_aadlConnections;
}

/*!
   Returns a list of the names of all functions in the aadl model
 */
QStringList IVEditorCore::aadlFunctionsNames() const
{
    QStringList functionNames;
    for (const ivm::AADLObjectFunction *aadlFunction : m_aadlFunctions) {
        if (aadlFunction && !aadlFunction->title().isEmpty()) {
            functionNames << aadlFunction->title();
        }
    }
    return functionNames;
}

/*!
   Returns a list of the names of all connections in the aadl model
 */
QStringList IVEditorCore::aadlConnectionNames() const
{
    QStringList connectionNames;
    for (const ivm::AADLObjectConnection *aadlConnection : m_aadlConnections) {
        if (aadlConnection && !aadlConnection->name().isEmpty()) {
            connectionNames << aadlConnection->name();
        }
    }
    connectionNames.removeDuplicates();
    return connectionNames;
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
ivm::AADLObjectIface *IVEditorCore::getInterface(
        const QString &ifName, ivm::AADLObjectIface::IfaceType ifType, ivm::AADLObjectFunction *parentFunction)
{
    if (parentFunction == nullptr) {
        return {};
    }

    ivm::AADLObjectsModel *aadlModel = m_document->objectsModel();
    if (!aadlModel) {
        return {};
    }

    ivm::AADLObjectIface *interface = aadlModel->getIfaceByName(ifName, ifType, parentFunction, m_caseCheck);
    if (!interface) {
        // create the interface
        ivm::AADLObjectIface::CreationInfo createInfo(aadlModel, parentFunction);
        createInfo.name = ifName;
        createInfo.type = ifType;
        QRectF funcRect = ive::rect(parentFunction->coordinates());
        QPointF ifPos(funcRect.left(), funcRect.center().y());
        if (ifType == ivm::AADLObjectIface::IfaceType::Required) {
            ifPos.setX(funcRect.right());
        }
        createInfo.position = ifPos;
        QUndoCommand *command = cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, createInfo.toVarList());
        cmd::CommandsStack::push(command);
        interface = aadlModel->getIfaceByName(ifName, ifType, parentFunction, m_caseCheck);
    }
    return interface;
}

/*!
   Updates the list of functions and connections from the aadl model
 */
void IVEditorCore::updateAadlItems()
{
    m_aadlFunctions.clear();
    m_aadlConnections.clear();

    ivm::AADLObjectsModel *aadlModel = m_document->objectsModel();
    if (!aadlModel) {
        return;
    }

    m_aadlFunctions = aadlModel->allObjectsByType<ivm::AADLObjectFunction>();
    m_aadlConnections = aadlModel->allObjectsByType<ivm::AADLObjectConnection>();
}

}
