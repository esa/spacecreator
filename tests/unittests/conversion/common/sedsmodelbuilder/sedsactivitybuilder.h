/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include <seds/SedsCommon/basetypesmappings.h>
#include <seds/SedsModel/components/activities/activity.h>
#include <seds/SedsModel/components/activities/coremathoperator.h>

namespace tests::conversion::common {

class SedsActivityBuilder final
{
public:
    SedsActivityBuilder(QString name);

    auto build() -> seds::model::Activity;

    auto withValueAssignment(const QString target, const QString value) -> SedsActivityBuilder &;
    auto withMathOperation(const QString target, const seds::model::CoreMathOperator op, const QString argument)
            -> SedsActivityBuilder &;
    auto withPolynomialCalibration(const QString target, const QString source, const std::vector<double> coefficients,
            const std::vector<uint64_t> exponents) -> SedsActivityBuilder &;
    auto withActivityCall(const QString target, const std::vector<QString> argumentNames,
            const std::vector<QString> argumentValues) -> SedsActivityBuilder &;

    auto withArgument(const QString name, const QString typeName) -> SedsActivityBuilder &;

private:
    seds::model::Activity m_activity;
};

} // namespace tests::conversion::common
