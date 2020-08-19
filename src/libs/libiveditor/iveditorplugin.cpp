#include "iveditorplugin.h"

#include "commandlineparser.h"
#include "context/action/actionsmanager.h"
#include "interface/interfacedocument.h"
#include "mainwindow.h"

#include <QMainWindow>
#include <QMenu>
#include <QToolBar>

namespace aadlinterface {

/*!
 * \namespace aadlinterface
 * \brief Has most of the code for the Interface View Editor UI
 */

IVEditorPlugin::IVEditorPlugin(QObject *parent)
    : shared::Plugin(parent)
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
InterfaceDocument *IVEditorPlugin::document() const
{
    return m_document;
}

void IVEditorPlugin::setPluginActive(bool active)
{
    m_actionSaveSceneRender->setVisible(active);
}

shared::ui::GraphicsViewBase *IVEditorPlugin::chartView()
{
    return qobject_cast<shared::ui::GraphicsViewBase *>(m_document->view());
}

void IVEditorPlugin::addToolBars(QMainWindow *window)
{
    window->addToolBar(mainToolBar());
    window->addToolBar(m_docToolBar);
}

/*!
 * \brief Fills the File menu with actions.
 */
void IVEditorPlugin::addMenuFileActions(QMenu *menu, QMainWindow *window)
{
    auto mainWindow = dynamic_cast<MainWindow *>(window);
    m_actionSaveSceneRender = menu->addAction(tr("Render Scene..."), mainWindow, &MainWindow::onSaveRenderRequested);
    m_actionShowAsnDialog = menu->addAction(tr("ASN1 dialog..."), mainWindow, &MainWindow::openAsn1Dialog);

    ActionsManager::registerAction(
            Q_FUNC_INFO, m_actionSaveSceneRender, "Render", "Save current scene complete render.");
    ActionsManager::registerAction(Q_FUNC_INFO, m_actionShowAsnDialog, "Asn1", "Edit the ASN1 file");
}

void IVEditorPlugin::addMenuEditActions(QMenu *menu, QMainWindow * /*window*/)
{
    menu->addMenu(document()->customMenu());
}

void IVEditorPlugin::addMenuViewActions(QMenu *menu, QMainWindow *window)
{
    Plugin::addMenuViewActions(menu, window);
}

/*!
 * \brief Fills the Help menu with actions.
 */
void IVEditorPlugin::addMenuHelpActions(QMenu *menu, QMainWindow *window)
{
    auto mainWindow = dynamic_cast<MainWindow *>(window);
    auto report = menu->addAction(tr("Send report..."), mainWindow, &MainWindow::onReportRequested);

    ActionsManager::registerAction(Q_FUNC_INFO, report, "Report", "Send the debug information");
}

void IVEditorPlugin::populateCommandLineArguments(shared::CommandLineParser *parser) const
{
    parser->handlePositional(shared::CommandLineParser::Positional::OpenAADLXMLFile);
    parser->handlePositional(shared::CommandLineParser::Positional::OpenStringTemplateFile);
    parser->handlePositional(shared::CommandLineParser::Positional::ExportToFile);
    parser->handlePositional(shared::CommandLineParser::Positional::ListScriptableActions);
    parser->handlePositional(shared::CommandLineParser::Positional::DropUnsavedChangesSilently);
}

}
