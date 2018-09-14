#include "graphicsview.h"

namespace msc {

/*!
  \class msc::GraphicsView
  \brief Basic view for an MSC diagram

  \inmodule MscWidgets

  Some documentation
*/

/*!
  Constructs a MSV view object with the parent \a parent.
*/
GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
}

void GraphicsView::setZoom(double percent)
{
    resetTransform();
    scale(percent / 100.0, percent / 100.0);
}

} // namespace msc
