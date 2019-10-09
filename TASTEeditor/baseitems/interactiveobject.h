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

#include <QFont>
#include <QGraphicsObject>
#include <QPointer>

namespace taste3 {

class HighlightRectItem;

class InteractiveObject : public QGraphicsObject
{
    Q_OBJECT
public:
    InteractiveObject(QObject *entity, QGraphicsItem *parent = nullptr);

    QObject *modelEntity() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    bool isHovered() const;

    void highlightConnected();
    void highlightDisconnected();
    void doHighlighting(const QColor &color, bool permanent);
    void clearHighlight();
    bool isHighlighting() const;

    QPointF centerInScene() const;

    bool isHighlightable() const;
    void setHighlightable(bool highlightable);

    virtual void postCreatePolishing() {}

    QPen pen() const;
    void setPen(const QPen &pen);

    QBrush brush() const;
    void setBrush(const QBrush &brush);

    QFont font() const;
    void setFont(const QFont &font);

public Q_SLOTS:
    void scheduleLayoutUpdate();
    void instantLayoutUpdate();
    void setRect(const QRectF &geometry);

    virtual void updateGripPoints();

Q_SIGNALS:
    void relocated(const QPointF &from, const QPointF &to) const;
    void moved(InteractiveObject *item);
    void boundingBoxChanged();
    void needUpdateLayout() const;
    void doubleClicked();

private Q_SLOTS:
    virtual void gripPointPressed(GripPoint::Location pos, const QPointF &at);
    virtual void gripPointMoved(GripPoint::Location pos, const QPointF &from, const QPointF &to);
    virtual void gripPointReleased(GripPoint::Location pos, const QPointF &pressedAt, const QPointF &releasedAt);

    void handleSelectionChanged(bool isSelected);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void onManualMoveStart(GripPoint::Location grip, const QPointF &at);
    virtual void onManualMoveProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to);
    virtual void onManualMoveFinish(GripPoint::Location grip, const QPointF &pressedAt, const QPointF &releasedAt);

    virtual void onManualResizeStart(GripPoint::Location grip, const QPointF &at);
    virtual void onManualResizeProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to);
    virtual void onManualResizeFinish(GripPoint::Location grip, const QPointF &pressedAt, const QPointF &releasedAt);

    virtual QSizeF minimalSize() const;

    virtual void onScenePositionChanged(const QPointF &scenePosition);

    virtual void hideGripPoints();
    virtual void showGripPoints();
    virtual void initGripPoints();
    virtual void rebuildLayout();

    HighlightRectItem *createHighlighter();

    QPointer<QObject> m_entity;
    QPointer<GripPointsHandler> m_gripPoints;
    QRectF m_boundingRect;
    bool m_hovered = false;
    qreal m_storedZ = 0.;
    QPointF m_prevPos;
    bool m_layoutDirty = false;

    bool m_highlightable = false;
    HighlightRectItem *m_highlighter = nullptr;
    QPen m_selectedPen;

    QBrush m_brush;
    QPen m_pen;
    QFont m_font;
};

} // namespace taste3
