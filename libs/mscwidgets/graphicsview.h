#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

namespace msc {

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr);

    void setZoom(double percent);
};

} // namespace msc

#endif // GRAPHICSVIEW_H
