#include "graphicsview.h"

namespace msc {

/*!
    \class GraphicsView
    \brief Basic view for an MSC diagram
*/

/*!
    Constructs a MSV view object with the parent \a parent.
*/
GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
}

} // namespace msc
