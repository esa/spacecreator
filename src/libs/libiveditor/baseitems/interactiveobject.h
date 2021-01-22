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

#include "interface/colors/colormanager.h"
#include "ui/interactiveobjectbase.h"

#include <QFont>
#include <QPen>
#include <QPointer>

namespace aadl {
class AADLObject;
}

namespace ive {

class InteractiveObject : public shared::ui::InteractiveObjectBase
{
    Q_OBJECT

public:
    explicit InteractiveObject(aadl::AADLObject *entity, QGraphicsItem *parent = nullptr);

    aadl::AADLObject *aadlObject() const;

    QPen pen() const;
    void setPen(const QPen &pen);

    QBrush brush() const;
    void setBrush(const QBrush &brush);

    QFont font() const;
    void setFont(const QFont &font);

    virtual void init();
    virtual void updateEntity();
    virtual void updateFromEntity() = 0;
    virtual QList<QVariantList> prepareChangeCoordinatesCommandParams() const;

    virtual QString prepareTooltip() const;

Q_SIGNALS:
    void clicked();
    void doubleClicked();

public Q_SLOTS:
    virtual void applyColorScheme() = 0;
    virtual void updateGraphicsItem();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void onSelectionChanged(bool isSelected) override;

    void mergeGeometry();

    virtual ColorManager::HandledColors handledColorType() const = 0;
    virtual ColorHandler colorHandler() const;

protected:
    const QPointer<aadl::AADLObject> m_dataObject;

    QBrush m_brush;
    QPen m_pen;
    QFont m_font;
};

}
