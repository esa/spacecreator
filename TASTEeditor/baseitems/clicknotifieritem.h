#pragma once

#include <QGraphicsObject>

namespace taste3 {

class ClickNotifierItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ClickNotifierItem(QObject *obj, QGraphicsItem *parent = nullptr);

    QObject *dataObject() const;

signals:
    void clicked();
    void doubleClicked();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    QObject* m_dataObject {nullptr};
};

} // namespace taste3
