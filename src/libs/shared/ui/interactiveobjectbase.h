/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "grippoint.h"

#include <QGraphicsObject>

namespace shared {
namespace ui {

class GripPoint;
class GripPointsHandler;
class HighlightRectItem;

/*!
  \class shared::ui::InteractiveObject
  \brief Movable/resizable QGraphicsObject with geometry change notifications.

  \ingroup shared::ui
*/
class InteractiveObjectBase : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit InteractiveObjectBase(QGraphicsItem *parent = nullptr);
    ~InteractiveObjectBase() override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF boundingRect() const override;
    void setBoundingRect(const QRectF &newRect);

    GripPointsHandler *gripPointsHandler() const;
    GripPoint *gripPointItem(GripPoint::Location location) const;

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
