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

#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "baseitems/common/aadlutils.h"
#include "commandlineparser.h"
#include "commandsstack.h"
#include "context/action/actionsmanager.h"
#include "interface/commands/cmdfunctionitemcreate.h"
#include "interface/commands/commandsfactory.h"
#include "interface/interfacedocument.h"
#include "mainwindow.h"

#include <QDebug>
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
void IVEditorCore::addFunction(const QString &name)
{
    if (!aadlinterface::cmd::CommandsStack::current()) {
        aadlinterface::cmd::CommandsStack::setCurrent(new QUndoStack(this));
    }

    aadl::AADLObjectFunction *parent = nullptr;
    const QVariantList params = { QVariant::fromValue(m_document->objectsModel()), QVariant::fromValue(parent),
        QVariant::fromValue(QRectF(QPointF(10., 10.), DefaultGraphicsItemSize)), QVariant::fromValue(name) };
    cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateFunctionEntity, params));
}

}
