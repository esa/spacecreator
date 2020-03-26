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
#include "tab_interface/colors/colormanager.h"

#include <QFont>
#include <QGraphicsObject>
#include <QPointer>

namespace taste3 {
class HighlightRectItem;
namespace aadl {

class AADLObject;

class InteractiveObject : public QGraphicsObject
{
    Q_OBJECT
public:
    InteractiveObject(AADLObject *entity, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    AADLObject *aadlObject() const;

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

    virtual void updateEntity();
    virtual void updateFromEntity() = 0;
    virtual QList<QVariantList> prepareChangeCoordinatesCommandParams() const;
    virtual void updateGripPoints();

    virtual QString prepareTooltip() const;

public Q_SLOTS:
    void scheduleLayoutUpdate();
    void instantLayoutUpdate();

Q_SIGNALS:
    void relocated(const QPointF &from, const QPointF &to) const;
    void moved(InteractiveObject *item);
    void boundingBoxChanged();
    void needUpdateLayout() const;
    void clicked();
    void doubleClicked();

public Q_SLOTS:
    virtual void applyColorScheme() = 0;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void onManualMoveStart(GripPoint *gp, const QPointF &at);
    virtual void onManualMoveProgress(GripPoint *gp, const QPointF &from, const QPointF &to);
    virtual void onManualMoveFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt);
    virtual void onManualGripPointAdd(GripPoint *gp);
    virtual void onManualGripPointRemove(GripPoint *gp);

    virtual void onManualResizeStart(GripPoint *gp, const QPointF &at);
    virtual void onManualResizeProgress(GripPoint *gp, const QPointF &from, const QPointF &to);
    virtual void onManualResizeFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt);

    virtual void hideGripPoints();
    virtual void showGripPoints();
    virtual void initGripPoints();

    virtual void rebuildLayout();
    virtual void onSelectionChanged(bool isSelected);

    HighlightRectItem *createHighlighter();
    void mergeGeometry();

    virtual ColorManager::HandledColors handledColorType() const = 0;
    virtual ColorHandler colorHandler() const;

private Q_SLOTS:
    virtual void gripPointPressed(GripPoint *pos, const QPointF &at);
    virtual void gripPointMoved(GripPoint *gp, const QPointF &from, const QPointF &to);
    virtual void gripPointReleased(GripPoint *pos, const QPointF &pressedAt, const QPointF &releasedAt);

protected:
    const QPointer<AADLObject> m_dataObject;
    QPointer<GripPointsHandler> m_gripPointsHandler;
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

} // namespace aadl
} // namespace taste3
