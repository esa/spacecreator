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

#include <QVector>

#pragma once

class QGraphicsScene;
class QPointF;

namespace dvm {
struct ValidationResult;
}

namespace dve {
namespace gi {

/*!
 * \brief Performs the validation to detect if it's possible to connect the \a scene's items located in \a startPos and
 * \a endPos.
 *
 * Returns the status of such validation as instance of ConnectionCreationValidator::ValidationResult.
 * Anything except the FailReason::NotFail in ConnectionCreationValidator::ValidationResult::status
 * means that the connection creation is prohibited.
 */
dvm::ValidationResult validateConnectionCreate(QGraphicsScene *scene, const QVector<QPointF> &points);

} // namespace gi
} // namespace dve
