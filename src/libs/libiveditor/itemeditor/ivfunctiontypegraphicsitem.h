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
namespace shared::ui{
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
    void applyColorScheme() override;
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
     * @param grip a GripPoint
     * @param from a QPointF the GripPoint was moved from
     * @param to a QPointF the GripPoint was moved to
     * @return a QRect representing the new sceneBoundingRect of this VERectGraphicsItem
     */
    virtual QRectF resizedRect(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to) override;

    /**
     * @brief resizedRectForTextLabel
     * @param grip
     * @param from
     * @param to
     * @return
     */
    virtual QRectF resizedRectForTextLabel(GripPoint *grip, const QPointF &from, const QPointF &to) const;

    void rebuildLayout() override;

    /**
     * @brief paint Paints the rectangle with the rounded corners and double borders that is a functiontype. The text is not paintet
     * by this class but handled by TextItem which is a child of this GraphicsItem.
     * @param painter
     * @param option
     * @param widget
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    shared::ColorManager::HandledColors handledColorType() const override;

    void updateTextPosition() override;

    /**
     * @brief alignTextItem Places the text-item within the boundingbox of this FunctionTypeGraphicsItem's bounding box.
     * For a IVFunctionTypeGraphicsItem this means the top-left of the text is aligned with the top-left of the IVFunctionTypeGraphicsItem's bounding box.
     * Child classes can override this to make their text align differently, like in the center of the bounding box.
     */
    virtual void alignTextItem() const;

    shared::ui::TextItem *initTextItem() override;
};

}
