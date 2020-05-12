#include "mscplugin.h"

#include "commandlineparser.h"

#include <QMainWindow>
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

void MSCPlugin::populateCommandLineArguments(shared::CommandLineParser *parser) const
{
    parser->handleOption(shared::CommandLineParser::Positional::OpenFileMsc);
    parser->handleOption(shared::CommandLineParser::Positional::DbgOpenMscExamplesChain);
    parser->handleOption(shared::CommandLineParser::Positional::StartRemoteControl);
    parser->handleOption(shared::CommandLineParser::Positional::DropUnsavedChangesSilently);
}

}
