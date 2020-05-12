#include "iveditorplugin.h"

#include <QMainWindow>
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

GraphicsView *IVEditorPlugin::graphicsView()
{
    return m_graphicsView;
}

void IVEditorPlugin::addToolBars(QMainWindow *window)
{
    window->addToolBar(mainToolBar());
    window->addToolBar(m_docToolBar);
}

}
