#include "iveditorplugin.h"

#include "baseitems/graphicsview.h"

#include <QToolBar>

namespace aadlinterface {

IVEditorPlugin::IVEditorPlugin(QObject *parent)
    : shared::Plugin(parent)
    , m_graphicsView(new GraphicsView)
    , m_docToolbar(new QToolBar)
{
    m_docToolbar->setObjectName("Document ToolBar");
    m_docToolbar->setAllowedAreas(Qt::AllToolBarAreas);
    m_docToolbar->setMovable(true);
}

IVEditorPlugin::~IVEditorPlugin() { }

GraphicsView *IVEditorPlugin::graphicsView()
{
    return m_graphicsView;
}

QVector<QToolBar *> IVEditorPlugin::additionalToolBars()
{
    return { m_docToolbar };
}

}
