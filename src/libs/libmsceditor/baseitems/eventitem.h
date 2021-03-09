/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

namespace msc {

class MscInstanceEvent;

/*!
   Base class for the graphical represenatin of instance events
 */
class EventItem : public InteractiveObject
{
    Q_OBJECT
public:
    EventItem(
            msc::MscInstanceEvent *event, msc::ChartLayoutManager *chartLayoutManager, QGraphicsItem *parent = nullptr);

    void setTargetHCenter(qreal x);
    void centerOnTargetH();

private:
    qreal m_targetHCenter = -9.9e12;
};

} // namespace msc
