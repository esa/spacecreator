/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "baseitems/interactiveobject.h"

#include <QPointer>

namespace msc {
class MscCoregion;
class ChartViewModel;
class InstanceItem;

class CoregionItem : public InteractiveObject
{
    Q_OBJECT
public:
    explicit CoregionItem(ChartViewModel *model, QGraphicsItem *parent = nullptr);

    void setBegin(MscCoregion *begin);
    MscCoregion *begin() const { return m_begin; }

    void setEnd(MscCoregion *end) { m_end = end; }
    MscCoregion *end() const { return m_end; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void connectObjects(InstanceItem *instance, qreal y);
    void setInstance(InstanceItem *instance);

protected:
    void initGripPoints() override;
    void rebuildLayout() override;

private:
    QPointer<ChartViewModel> m_model;
    QPointer<InstanceItem> m_instance;

    MscCoregion *m_begin = nullptr;
    MscCoregion *m_end = nullptr;

    bool m_unorderedEntities = true;
};

} // namespace msc
