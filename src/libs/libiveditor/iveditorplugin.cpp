#include "iveditorplugin.h"

#include "commandlineparser.h"
#include "context/action/actionsmanager.h"
#include "mainwindow.h"

#include <QMainWindow>
#include <QMenu>
#include <QToolBar>

namespace aadlinterface {

IVEditorPlugin::IVEditorPlugin(QObject *parent)
    : shared::Plugin(parent)
    , m_graphicsView(new GraphicsView)
    , m_docToolBar(new QToolBar)
{
    m_docToolBar->setObjectName("Document ToolBar");
    m_docToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    m_docToolBar->setMovable(true);
}

void IVEditorPlugin::setPluginActive(bool active)
{
    m_actionSaveSceneRender->setVisible(active);
}

GraphicsView *IVEditorPlugin::graphicsView()
{
    return m_graphicsView;
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
    auto mainWindow = dynamic_cast<taste3::MainWindow *>(window);
    m_actionSaveSceneRender =
            menu->addAction(tr("Render Scene..."), mainWindow, &taste3::MainWindow::onSaveRenderRequested);

    taste3::ctx::ActionsManager::registerAction(
            Q_FUNC_INFO, m_actionSaveSceneRender, "Render", "Save current scene complete render.");
}

void IVEditorPlugin::addMenuEditActions(QMenu * /*menu*/, QMainWindow * /*window*/)
{
    // Nothing to add
}

void IVEditorPlugin::addMenuViewActions(QMenu *menu, QMainWindow * /*window*/)
{
    menu->hide();
}

/*!
 * \brief Fills the Help menu with actions.
 */
void IVEditorPlugin::addMenuHelpActions(QMenu *menu, QMainWindow *window)
{
    auto mainWindow = dynamic_cast<taste3::MainWindow *>(window);
    auto report = menu->addAction(tr("Send report..."), mainWindow, &taste3::MainWindow::onReportRequested);
    auto about = menu->addAction(tr("About"), mainWindow, &taste3::MainWindow::onAboutRequested);

    taste3::ctx::ActionsManager::registerAction(Q_FUNC_INFO, report, "Report", "Send the debug information");
    taste3::ctx::ActionsManager::registerAction(Q_FUNC_INFO, about, "About", "Show About dialog");
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
