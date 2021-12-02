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

#include "sedsinvocationbuilder.h"

#include <assert.h>
#include <seds/SedsModel/components/activities/calibration.h>
#include <seds/SedsModel/components/activities/variablerefoperand.h>
#include <seds/SedsModel/sedsmodel.h>

using namespace seds::model;

namespace tests::conversion::common {

SedsInvocationBuilder::SedsInvocationBuilder(QString name)
{
    m_invocation.setActivity(name);
}

auto SedsInvocationBuilder::build() -> seds::model::ActivityInvocation
{
    return std::move(m_invocation);
}

auto SedsInvocationBuilder::withArgument(const QString name, const QString value) -> SedsInvocationBuilder &
{
    seds::model::NamedArgumentValue argument;
    seds::model::ValueOperand operand;
    operand.setValue(value);
    argument.setName(name);
    argument.setValue(std::move(operand));
    m_invocation.addArgumentValue(std::move(argument));

    return *this;
}

}
