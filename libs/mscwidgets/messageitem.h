#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QGraphicsObject>

class QGraphicsLineItem;
class QGraphicsPolygonItem;
class QGraphicsTextItem;

namespace msc {
class InstanceItem;

class MessageItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MessageItem(QGraphicsItem *parent = nullptr);

    void setSourceInstanceItem(InstanceItem *sourceInstance);
    void setTargetInstanceItem(InstanceItem *targetInstance);

    void updateLayout();

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public Q_SLOTS:
    void setName(const QString &name);

private Q_SLOTS:
    void buildLayout();

private:
    void setWidth(double width);
    void centerName();

    QGraphicsLineItem *m_line = nullptr;
    QGraphicsPolygonItem *m_leftArrow = nullptr;
    QGraphicsPolygonItem *m_rightArrow = nullptr;
    QGraphicsTextItem *m_nameItem = nullptr;
    InstanceItem *m_sourceInstance = nullptr;
    InstanceItem *m_targetInstance = nullptr;
    bool m_layoutDirty = false;
};

} // namespace mas

#endif // MESSAGEITEM_H
