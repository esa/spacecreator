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

#include "aadlnamevalidator.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"
#include "baseitems/common/aadlutils.h"
#include "commandlineparser.h"
#include "commandsstack.h"
#include "context/action/actionsmanager.h"
#include "interface/commands/cmdfunctionitemcreate.h"
#include "interface/commands/commandsfactory.h"
#include "interface/interfacedocument.h"
#include "mainwindow.h"
#include "xmldocexporter.h"

#include <QDebug>
#include <QFileInfo>
#include <QMainWindow>
#include <QMenu>
#include <QRectF>
#include <QToolBar>
#include <QUndoStack>

namespace aadlinterface {

/*!
 * \namespace aadlinterface
 * \brief Has most of the code for the Interface View Editor UI
 */

IVEditorCore::IVEditorCore(QObject *parent)
    : shared::EditorCore(parent)
    , m_document(new aadlinterface::InterfaceDocument(this))
    , m_docToolBar(new QToolBar)
{
    m_document->init();

    m_docToolBar->setObjectName("Document ToolBar");
    m_docToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    m_docToolBar->setMovable(true);
}

IVEditorCore::~IVEditorCore() { }

/*!
   Returns the interface document
 */
InterfaceDocument *IVEditorCore::document() const
{
    return m_document;
}

void IVEditorCore::setPluginActive(bool active)
{
    m_actionSaveSceneRender->setVisible(active);
}

shared::ui::GraphicsViewBase *IVEditorCore::chartView()
{
    return qobject_cast<shared::ui::GraphicsViewBase *>(m_document->view());
}

void IVEditorCore::addToolBars(QMainWindow *window)
{
    window->addToolBar(mainToolBar());
    window->addToolBar(m_docToolBar);
}

/*!
 * \brief Fills the File menu with actions.
 */
void IVEditorCore::addMenuFileActions(QMenu *menu, QMainWindow *window)
{
    auto mainWindow = dynamic_cast<MainWindow *>(window);
    m_actionSaveSceneRender = menu->addAction(tr("Render Scene..."), mainWindow, &MainWindow::onSaveRenderRequested);
    m_actionShowAsnDialog = menu->addAction(tr("ASN1 dialog..."), mainWindow, &MainWindow::openAsn1Dialog);

    ActionsManager::registerAction(
            Q_FUNC_INFO, m_actionSaveSceneRender, "Render", "Save current scene complete render.");
    ActionsManager::registerAction(Q_FUNC_INFO, m_actionShowAsnDialog, "Asn1", "Edit the ASN1 file");
}

void IVEditorCore::addMenuEditActions(QMenu *menu, QMainWindow * /*window*/)
{
    QMenu *root = new QMenu(tr("Common Settings"));
    root->addActions(document()->customActions());
    menu->addMenu(root);
}

void IVEditorCore::addMenuViewActions(QMenu *menu, QMainWindow *window)
{
    EditorCore::addMenuViewActions(menu, window);
}

/*!
 * \brief Fills the Help menu with actions.
 */
void IVEditorCore::addMenuHelpActions(QMenu *menu, QMainWindow *window)
{
    auto mainWindow = dynamic_cast<MainWindow *>(window);
    auto report = menu->addAction(tr("Send report..."), mainWindow, &MainWindow::onReportRequested);

    ActionsManager::registerAction(Q_FUNC_INFO, report, "Report", "Send the debug information");
}

void IVEditorCore::populateCommandLineArguments(shared::CommandLineParser *parser) const
{
    parser->handlePositional(shared::CommandLineParser::Positional::OpenAADLXMLFile);
    parser->handlePositional(shared::CommandLineParser::Positional::OpenStringTemplateFile);
    parser->handlePositional(shared::CommandLineParser::Positional::ExportToFile);
    parser->handlePositional(shared::CommandLineParser::Positional::ListScriptableActions);
    parser->handlePositional(shared::CommandLineParser::Positional::DropUnsavedChangesSilently);
}

/*!
   Adds an aadl function with the given \p name
   @todo handle positioning of the function
 */
bool IVEditorCore::addFunction(const QString &name)
{
    if (!aadlinterface::cmd::CommandsStack::current()) {
        aadlinterface::cmd::CommandsStack::setCurrent(new QUndoStack(this));
    }

    aadl::AADLObjectFunction *parent = nullptr;
    const QVariantList params = { QVariant::fromValue(m_document->objectsModel()), QVariant::fromValue(parent),
        QVariant::fromValue(QRectF(QPointF(10., 10.), DefaultGraphicsItemSize)), QVariant::fromValue(name) };
    cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateFunctionEntity, params));

    Q_EMIT editedExternally(this);
    return true;
}

/*!
   Adds the connection and the needed interfaces between the given instances
 */
bool IVEditorCore::addConnection(QString name, const QString &fromInstanceName, const QString &toInstanceName)
{
    if (!aadlinterface::cmd::CommandsStack::current()) {
        aadlinterface::cmd::CommandsStack::setCurrent(new QUndoStack(this));
    }

    name = aadl::AADLNameValidator::decodeName(aadl::AADLObject::Type::ProvidedInterface, name);

    aadl::AADLObjectsModel *aadlModel = m_document->objectsModel();
    if (aadlModel->getConnection(name, fromInstanceName, toInstanceName) != nullptr) {
        // The connection exists already
        return false;
    }

    aadl::AADLObjectFunction *fromFunc = aadlModel->getFunction(fromInstanceName);
    aadl::AADLObjectFunction *toFunc = aadlModel->getFunction(toInstanceName);

    if (!fromFunc && !toFunc) {
        qDebug() << Q_FUNC_INFO << "No function for the connection" << name;
    }

    cmd::CommandsStack::current()->beginMacro("Add connection");

    aadl::AADLObjectIface *fromInterface = getInterface(name, aadl::AADLObjectIface::IfaceType::Required, fromFunc);
    aadl::AADLObjectIface *toInterface = getInterface(name, aadl::AADLObjectIface::IfaceType::Provided, toFunc);

    if (fromInterface && toInterface) {
        QVector<QPointF> points;
        points.append(aadlinterface::pos(fromInterface->coordinates()));
        points.append(aadlinterface::pos(toInterface->coordinates()));
        const QVariantList params = { QVariant::fromValue(aadlModel), QVariant::fromValue(fromFunc),
            fromInterface->id(), toInterface->id(), QVariant::fromValue(points) };
        QUndoCommand *command = cmd::CommandsFactory::create(cmd::CreateConnectionEntity, params);
        cmd::CommandsStack::current()->push(command);
    }

    cmd::CommandsStack::current()->endMacro();

    Q_EMIT editedExternally(this);
    return true;
}

/*!
   Changes the asn1 referenceto \p newName if the existing one if pointing to \p oldName
 */
bool IVEditorCore::renameAsnFile(const QString &oldName, const QString &newName)
{
    if (!aadlinterface::cmd::CommandsStack::current()) {
        QUndoStack *currentStack = document()->commandsStack();
        if (currentStack) {
            aadlinterface::cmd::CommandsStack::setCurrent(currentStack);
        } else {
            aadlinterface::cmd::CommandsStack::setCurrent(new QUndoStack(this));
        }
    }

    QFileInfo oldFile(oldName);
    const QString oldFileName = oldFile.fileName();

    if (document()->asn1FileName() == oldFileName) {
        QVariantList params { QVariant::fromValue(document()), QVariant::fromValue(newName) };
        QUndoCommand *command = cmd::CommandsFactory::create(cmd::ChangeAsn1File, params);
        if (command) {
            cmd::CommandsStack::current()->push(command);
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
    return aadlinterface::XmlDocExporter::exportDocSilently(m_document, {}, {});
}

/*!
   Get the interface, or creates it if it does not exist. In case the \p parentFunction is a nullptr, a nullptr is
   returned
 */
aadl::AADLObjectIface *IVEditorCore::getInterface(
        const QString &ifName, aadl::AADLObjectIface::IfaceType ifType, aadl::AADLObjectFunction *parentFunction)
{
    if (parentFunction == nullptr) {
        return {};
    }

    aadl::AADLObjectsModel *aadlModel = m_document->objectsModel();
    if (!aadlModel) {
        return {};
    }

    aadl::AADLObjectIface *interface = aadlModel->getIfaceByName(ifName, ifType, parentFunction);
    if (!interface) {
        // create the interface
        aadl::AADLObjectIface::CreationInfo createInfo(aadlModel, parentFunction);
        createInfo.name = ifName;
        createInfo.type = ifType;
        QRectF funcRect = aadlinterface::rect(parentFunction->coordinates());
        QPointF ifPos(funcRect.left(), funcRect.center().y());
        if (ifType == aadl::AADLObjectIface::IfaceType::Required) {
            ifPos.setX(funcRect.right());
        }
        createInfo.position = ifPos;
        QUndoCommand *command = cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, createInfo.toVarList());
        cmd::CommandsStack::current()->push(command);
        interface = aadlModel->getIfaceByName(ifName, ifType, parentFunction);
    }
    return interface;
}

}
