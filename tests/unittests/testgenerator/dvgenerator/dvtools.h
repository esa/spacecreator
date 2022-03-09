/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#pragma once

#include <QByteArray>
#include <QVector>
#include <dvcore/dvmodel.h>
#include <dvcore/dvobject.h>
#include <memory>

namespace tests {
namespace dvtools {

/**
 * @brief Get the DVObjects from requested file
 *
 * @param filepath  path to the file to be read
 *
 * @return DVObjects
 */
auto getDvObjectsFromFile(const QString &filepath) -> std::unique_ptr<QVector<dvm::DVObject *>>;

/**
 * @brief Get the DVObjects from Model
 *
 * @param model DeploymentView data model
 *
 * @return DVObjects
 */
auto getDvObjectsFromModel(dvm::DVModel *model) -> std::unique_ptr<QVector<dvm::DVObject *>>;

} // namespace dvtools
} // namespace tests
