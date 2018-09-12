#ifndef INSTANCEITEM_H
#define INSTANCEITEM_H

#include <QGraphicsObject>

class QGraphicsRectItem;
class QGraphicsTextItem;

namespace msc {

class InstanceItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(double horizontalCenter READ horizontalCenter NOTIFY horizontalCenterChanged)

public:
    explicit InstanceItem(QGraphicsItem *parent = nullptr);

    double horizontalCenter() const;

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void setName(const QString &name);

signals:
    void horizontalCenterChanged();

protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

private:
    QGraphicsRectItem *m_headSymbol = nullptr;
    QGraphicsTextItem *m_nameItem = nullptr;
    QGraphicsLineItem *m_axisSymbol = nullptr;
    QGraphicsRectItem *m_endSymbol = nullptr;
};

} // namespace msc

#endif // INSTANCEITEM_H
