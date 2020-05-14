#include "mscplugin.h"

#include "commandlineparser.h"
#include "mainwindow.h"

#include <QMainWindow>
#include <QMenu>
#include <QToolBar>

namespace msc {

MSCPlugin::MSCPlugin(QObject *parent)
    : shared::Plugin(parent)
    , m_graphicsView(new GraphicsView)
    , m_mscToolBar(new QToolBar(tr("MSC")))
    , m_hierarchyToolBar(new QToolBar(tr("Hierarchy")))
{
    m_mscToolBar->setObjectName("mscTools");
    m_mscToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    m_hierarchyToolBar->setObjectName("hierarchyTools");
    m_hierarchyToolBar->setAllowedAreas(Qt::AllToolBarAreas);
}

void MSCPlugin::setPluginActive(bool active)
{
    m_actionScreenshot->setVisible(active);
}

GraphicsView *MSCPlugin::graphicsView()
{
    return m_graphicsView;
}

void MSCPlugin::addToolBars(QMainWindow *window)
{
    window->addToolBar(mainToolBar());
    window->addToolBar(Qt::LeftToolBarArea, m_mscToolBar);
    window->addToolBar(Qt::LeftToolBarArea, m_hierarchyToolBar);
}

/*!
 * \brief Fills the File menu with actions.
 */
void MSCPlugin::addMenuFileActions(QMenu *menu, QMainWindow *window)
{
    auto mainWindow = dynamic_cast<MainWindow *>(window);
    m_actionScreenshot = menu->addAction(QIcon(QLatin1String(":/sharedresources/icons/save.svg")),
            tr("Save Screenshot..."), mainWindow, &MainWindow::saveScreenshot, QKeySequence(Qt::ALT + Qt::Key_S));
    menu->addSeparator();
}

void MSCPlugin::populateCommandLineArguments(shared::CommandLineParser *parser) const
{
    parser->handlePositional(shared::CommandLineParser::Positional::OpenFileMsc);
    parser->handlePositional(shared::CommandLineParser::Positional::DbgOpenMscExamplesChain);
    parser->handlePositional(shared::CommandLineParser::Positional::StartRemoteControl);
    parser->handlePositional(shared::CommandLineParser::Positional::DropUnsavedChangesSilently);
}

}
