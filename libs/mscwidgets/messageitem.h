#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QGraphicsObject>

class QGraphicsLineItem;
class QGraphicsPolygonItem;
class QGraphicsTextItem;

namespace msc {

class MessageItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MessageItem(QGraphicsItem *parent = nullptr);

    void setWidth(double width);

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void setName(const QString &name);

private:
    void centerName();

    QGraphicsLineItem *m_line = nullptr;
    QGraphicsPolygonItem *m_arrow = nullptr;
    QGraphicsTextItem *m_nameItem = nullptr;
};

} // namespace mas

#endif // MESSAGEITEM_H
