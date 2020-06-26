/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "graphicsitemcompare.h"

#include <QGraphicsItem>

namespace test {
namespace ui {

/*!
   Tests if item \p testItem is above \p referenceItem
 */
bool isAbove(const QGraphicsItem *testItem, const QGraphicsItem *referenceItem)
{
    return referenceItem->sceneBoundingRect().top() > testItem->sceneBoundingRect().bottom()
            && referenceItem->sceneBoundingRect().bottom() > testItem->sceneBoundingRect().bottom();
}

/*!
   Tests if item \p testItem is below \p referenceItem
 */
bool isBelow(const QGraphicsItem *testItem, const QGraphicsItem *referenceItem)
{
    return testItem->sceneBoundingRect().top() > referenceItem->sceneBoundingRect().bottom()
            && testItem->sceneBoundingRect().bottom() > referenceItem->sceneBoundingRect().bottom();
}

bool isVerticalInside(const QGraphicsItem *testItem, const QGraphicsItem *referenceItem)
{
    return testItem->sceneBoundingRect().top() > referenceItem->sceneBoundingRect().top()
            && referenceItem->sceneBoundingRect().bottom() > testItem->sceneBoundingRect().bottom();
}

} // namespace ui
} // namespace test
