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

#include "colors/colormanager.h"
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
    Q_PROPERTY(UserInteraction interactionMode READ interactionMode NOTIFY interactionModeChanged FINAL)

public:
    enum UserInteraction
    {
        None,
        Moving,
        Resizing,
    };
    Q_ENUM(UserInteraction);

    explicit InteractiveObjectBase(QGraphicsItem *parent = nullptr);
    ~InteractiveObjectBase() override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /**
     * @brief boundingRect Any child of QGraphicsItem must implement this method.
     */
    QRectF boundingRect() const override;

    /**
     * @brief setBoundingRect sets the geometry of this item. Position of grippoint-handles are updated an
     * 'boundingboxChanged' is emitted.
     * @param newRect
     */
    void setBoundingRect(const QRectF &newRect);

    GripPointsHandler *gripPointsHandler() const;
    GripPoint *gripPointItem(GripPoint::Location location) const;

    bool isHovered() const;

    void enableHighlight();
    void disableHighlight();
    void doHighlighting(const QColor &color, bool permanent, int duration = 250);
    void clearHighlight();
    bool isHighlighting() const;

    QPointF centerInScene() const;

    bool isHighlightable() const;
    void setHighlightable(bool highlightable);

    virtual void postCreatePolishing();

    virtual void updateGripPoints();

    QPen selectedPen() const;

    /**
     * If supported for this item, horizontal/vertical help lines are shown/removed for this item
     */
    virtual void showHelperLines(bool show);
    /**
     * Returns if actions (move/resize) should show a help lines for this item
     */
    virtual bool helpLinesSupported() const;

    /*!
     * Mode of user interaction. While the user does move/resize the item.
     */
    UserInteraction interactionMode() const;

public Q_SLOTS:
    void scheduleLayoutUpdate();
    void instantLayoutUpdate();

Q_SIGNALS:
    void relocated(const QPointF &from, const QPointF &to);
    void moved(shared::ui::InteractiveObjectBase *item);
    void boundingBoxChanged();
    void needUpdateLayout();
    void interactionModeChanged();

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

    /**
     * @brief handledColorType
     * Each child of of this class must be able to return its type as denoted by the enum ColorManager::HandledColors
     * @return ColorManager::HandledColors
     */
    virtual shared::ColorManager::HandledColors handledColorType() const = 0;

    /**
     * @brief colorHandler
     * The default implementation of colorHandler uses 'handledColorType' to instantiate a ColorHandler
     * @return a ColorHandler
     */
    virtual ColorHandler colorHandler() const;

    void showBoundingBox();

protected Q_SLOTS:
    virtual void rebuildLayout();
    void setInteractionMode(UserInteraction mode);

private Q_SLOTS:
    virtual void gripPointPressed(shared::ui::GripPoint *pos, const QPointF &at);
    virtual void gripPointMoved(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to);
    virtual void gripPointReleased(shared::ui::GripPoint *pos, const QPointF &pressedAt, const QPointF &releasedAt);
    void updateBoundingBox();

private:
    HighlightRectItem *createHighlighter();

    struct InteractiveObjectBasePrivate;
    InteractiveObjectBasePrivate *d;
};
}
}
