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

#include "ivinterface.h"
#include "ivobject.h"
#include "ui/veconnectionendpointgraphicsitem.h"

#include <QPointer>

class tst_PositionLookupHelper;

namespace ive {
class IVConnectionGraphicsItem;
class IVFunctionGraphicsItem;
class IVFunctionTypeGraphicsItem;

class IVInterfaceGraphicsItem : public shared::ui::VEConnectionEndPointGraphicsItem
{
    Q_OBJECT
public:
    explicit IVInterfaceGraphicsItem(ivm::IVInterface *entity, QGraphicsItem *parent = nullptr);
    enum
    {
        Type = UserType + static_cast<int>(ivm::IVObject::Type::RequiredInterface) // provided?
    };

    ivm::IVInterface *entity() const override;

    void init() override;
    int type() const override { return Type; }

    void setInterfaceName(const QString &name);

    QPointF connectionEndPoint(const bool nestedConnection) const override;
    QPointF connectionEndPoint(shared::ui::VEConnectionGraphicsItem *connection = nullptr) const override;
    QPainterPath ifaceShape() const override;
    QPainterPath shape() const override;

    void updateFromEntity() override;
    bool layout() override;
    int itemLevel(bool isSelected) const override;
    QString prepareTooltip() const override;

    qreal maxWidth() const;
    static qreal baseLength();

public Q_SLOTS:
    void updateLabel();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void rebuildLayout() override;
    void onSelectionChanged(bool isSelected) override;
    void updateInternalItems(Qt::Alignment alignment) override;
    virtual shared::ColorManager::HandledColors handledColorType() const override;
    shared::ui::TextItem *initTextItem() override;

    qreal typeIconHeight() const;
    void updateEntityTitle(const QString &text);

protected Q_SLOTS:
    virtual void applyColorScheme() override;
    void updateKind();
    void updateIface();
    void onAttrOrPropChanged(const QString &attrName);

    virtual QPainterPath ifacePath() const;
    virtual QPainterPath typePath() const;
    QPainterPath itemPath(Qt::Alignment alignment) const override;

private:
    QTransform typeTransform(Qt::Alignment alignment) const;
    QTransform ifaceTransform(Qt::Alignment alignment) const;
    QTransform textTransform(Qt::Alignment alignment) const;
    QString ifaceLabel() const;
    QPainterPath composeShape() const;

private:
    friend tst_PositionLookupHelper;

    QPainterPath m_shape;
    QGraphicsPathItem *m_type = nullptr;
    QGraphicsPathItem *m_iface = nullptr;
};

}
