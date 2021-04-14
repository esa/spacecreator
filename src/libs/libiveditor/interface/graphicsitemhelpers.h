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

#include <QVector>

#pragma once

class QGraphicsItem;
class QGraphicsScene;
class QRectF;
class QPointF;

namespace ivm {
class IVFunction;
class IVFunctionType;
class IVInterface;
class IVComment;
class IVConnection;
class IVObject;
struct ValidationResult;
}

namespace ive {

namespace gi {

QList<int> knownGraphicsItemTypes();

QList<int> rectangularTypes();

ivm::IVFunction *functionObject(QGraphicsItem *item);

ivm::IVFunctionType *functionTypeObject(QGraphicsItem *item);

ivm::IVInterface *interfaceObject(QGraphicsItem *item);

ivm::IVComment *commentObject(QGraphicsItem *item);

ivm::IVConnection *connectionObject(QGraphicsItem *item);

ivm::IVObject *object(const QGraphicsItem *item);

enum RectOperation
{
    Create,
    Edit
};

bool isBounded(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect);
bool isCollided(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect);

/*!
 * \brief Performs the validation to detect if it's possible to connect the \a scene's items located in \a startPos and
 * \a endPos.
 *
 * Returns the status of such validation as instance of ConnectionCreationValidator::ValidationResult.
 * Anything except the FailReason::NotFail in ConnectionCreationValidator::ValidationResult::status
 * means that the connection creation is prohibited.
 */
ivm::ValidationResult validateConnectionCreate(QGraphicsScene *scene, const QVector<QPointF> &points);

} // namespace gi
} // namespace ive
