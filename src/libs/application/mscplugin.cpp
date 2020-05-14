#include "mscplugin.h"

#include "commandlineparser.h"
#include "mainwindow.h"
#include "tools/basetool.h"

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
    m_editSeparator->setVisible(active);
    m_actionCopy->setVisible(active);
    m_actionPaste->setVisible(active);
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

void MSCPlugin::addMenuEditActions(QMenu *menu, QMainWindow *window)
{
    auto mainWindow = dynamic_cast<MainWindow *>(window);

    menu->addSeparator();
    menu->addAction(mainWindow->deleteTool()->action());
    m_editSeparator = menu->addSeparator();
    menu->addAction(createActionCopy(mainWindow));
    menu->addAction(createActionPaste(mainWindow));
}

void MSCPlugin::populateCommandLineArguments(shared::CommandLineParser *parser) const
{
    parser->handlePositional(shared::CommandLineParser::Positional::OpenFileMsc);
    parser->handlePositional(shared::CommandLineParser::Positional::DbgOpenMscExamplesChain);
    parser->handlePositional(shared::CommandLineParser::Positional::StartRemoteControl);
    parser->handlePositional(shared::CommandLineParser::Positional::DropUnsavedChangesSilently);
}

QAction *MSCPlugin::createActionCopy(MainWindow *window)
{
    if (m_actionCopy == nullptr) {
        m_actionCopy = new QAction(tr("Copy:"), this);
        m_actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
        m_actionCopy->setMenu(new QMenu(window));
        m_actionCopy->menu()->addAction(tr("Copy Diagram"), window, &MainWindow::copyCurrentChart, QKeySequence::Copy);
        m_actionCopy->menu()->addAction(tr("Copy as Picture"), window, &MainWindow::copyCurrentChartAsPicture);
    }
    return m_actionCopy;
}

QAction *MSCPlugin::createActionPaste(MainWindow *window)
{
    if (m_actionPaste == nullptr) {
        m_actionPaste = new QAction(tr("Paste:"), window);
        m_actionPaste->setShortcut(QKeySequence::Paste);
        m_actionPaste->setIcon(QIcon::fromTheme("edit-paste"));
        connect(m_actionPaste, &QAction::triggered, window, &MainWindow::pasteChart);
    }
    return m_actionPaste;
}

}
