/*
Copyright (C) 2020 by Viking Software www.vikingsoftware.com

Permission to use, copy, modify, and/or distribute this software for any purpose
with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.
*/

#include "graphicsitemcompare.h"

#include <QGraphicsItem>

namespace vstest {

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

}
