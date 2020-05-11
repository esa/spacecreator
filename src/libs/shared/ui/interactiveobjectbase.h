#pragma once

#include <QGraphicsObject>

namespace shared {
namespace ui {

class GripPoint;
class GripPointsHandler;
class HighlightRectItem;

/*!
  \class shared::ui::InteractiveObject
  \brief Movable/resizable QGraphicsObject with geometry change notifications.

  \inmodule shared::ui
*/
class InteractiveObjectBase : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit InteractiveObjectBase(QGraphicsItem *parent = nullptr);
    ~InteractiveObjectBase() override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF boundingRect() const override;
    void setBoundingRect(const QRectF newRect);

    GripPointsHandler *gripPointsHandler();

    bool isHovered() const;

    void highlightConnected();
    void highlightDisconnected();
    void doHighlighting(const QColor &color, bool permanent);
    void clearHighlight();
    bool isHighlighting() const;

    QPointF centerInScene() const;

    bool isHighlightable() const;
    void setHighlightable(bool highlightable);

    virtual void postCreatePolishing();

    virtual void updateGripPoints();

    QPen selectedPen() const;

public Q_SLOTS:
    void scheduleLayoutUpdate();
    void instantLayoutUpdate();

Q_SIGNALS:
    void relocated(const QPointF &from, const QPointF &to);
    void moved(shared::ui::InteractiveObjectBase *item);
    void boundingBoxChanged();
    void needUpdateLayout();

protected:
    virtual void onManualMoveStart(GripPoint *gp, const QPointF &at);
    virtual void onManualMoveProgress(GripPoint *gp, const QPointF &from, const QPointF &to);
    virtual void onManualMoveFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt);
    virtual void onManualGripPointAdd(GripPoint *gp);
    virtual void onManualGripPointRemove(GripPoint *gp);
    virtual void onManualResizeStart(GripPoint *gp, const QPointF &at);
    virtual void onManualResizeProgress(GripPoint *gp, const QPointF &from, const QPointF &to);
    virtual void onManualResizeFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt);
    virtual void onSelectionChanged(bool isSelected);

    virtual void initGripPoints();
    virtual void showGripPoints();
    virtual void hideGripPoints();

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

protected Q_SLOTS:
    virtual void rebuildLayout();

private Q_SLOTS:
    virtual void gripPointPressed(shared::ui::GripPoint *pos, const QPointF &at);
    virtual void gripPointMoved(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to);
    virtual void gripPointReleased(shared::ui::GripPoint *pos, const QPointF &pressedAt, const QPointF &releasedAt);

private:
    HighlightRectItem *createHighlighter();

    struct InteractiveObjectBasePrivate;
    InteractiveObjectBasePrivate *d;
};

}
}
