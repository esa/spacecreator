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
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "ivfunctiontype.h"
#include "ivobject.h"
#include "ui/verectgraphicsitem.h"
#include "ui/resizelimits.h"

namespace shared::ui {
class GripPoint;
}

namespace ive {

class IVFunctionNameGraphicsItem;
using shared::ui::GripPoint;

class IVFunctionTypeGraphicsItem : public shared::ui::VERectGraphicsItem
{
    Q_OBJECT
public:
    explicit IVFunctionTypeGraphicsItem(ivm::IVFunctionType *entity, QGraphicsItem *parentItem = nullptr);
    enum
    {
        Type = UserType + static_cast<int>(ivm::IVObject::Type::FunctionType)
    };
    int type() const override { return Type; }

    ivm::IVFunctionType *entity() const override;

    void enableEditMode() override;

    QString prepareTooltip() const override;

    bool isRootItem() const;

    int itemLevel(bool isSelected) const override;

    void init() override;

protected Q_SLOTS:
    void updateNameFromUi(const QString &name);

protected:
    /**
     * @brief resizedRect takes a GripPoint sitting on this VERectGraphicsItem and two
     * QPointFs representing the movement of the GripPoint and returns
     * a QRectF representing the new sceneBoundingRect of this VERectGraphicsItem based on
     * the allowed movement of that particular grip point.
     *
     * A corner GripPoint can move freely. A GripPoint on a horizontal line can only move
     * up or down. A GripPoint on a vertical line can only move left or right.
     *
     * If a transformation is illegal, a QRectF is calculated that performs as much as possible of the
     * transformation as is legal.
     *
     * Limits of movement: An IVFunctionTypeGraphicsItem inherits its resizedRect from VERectGraphicsItem which
     * limits the resizing with respect to interfaces (aka connection endpoints). FunctionTypeGraphicsItem adds
     * respect to the Text displaying the FunctionName, so that the FunctionName is always visible.
     *
     * @param grip a GripPoint
     * @param from a QPointF the GripPoint was moved from
     * @param to a QPointF the GripPoint was moved to
     * @return a QRect representing the new sceneBoundingRect of this IVFunctionTypeGraphicsItem
     */
    virtual QRectF resizedRect(shared::ui::ResizeLimits resizeLimits) override;
    shared::ui::ResizeLimits resizedRectForTextLabel(shared::ui::ResizeLimits resizeLimits);
    shared::ui::ResizeLimits resizeLimitsForCollision(shared::ui::ResizeLimits resizeLimits);


    void rebuildLayout() override;

    /**
     * @brief paint Paints the rectangle with the rounded corners and double borders that is a functiontype. The text is
     * not paintet by this class but handled by TextItem which is a child of this GraphicsItem.
     * @param painter
     * @param option
     * @param widget
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    shared::ColorManager::HandledColors handledColorType() const override;

    /**
     * @brief updateTextPosition Places the text-item within the boundingbox of this FunctionTypeGraphicsItem's bounding
     * box. For a IVFunctionTypeGraphicsItem and inherited types this means the alignment is taken from text item
     * alignment property. Manual alignment is needed because of https://bugreports.qt.io/browse/QTBUG-13467
     */
    void updateTextPosition() override;

    shared::ui::TextItem *initTextItem() override;

};

}
