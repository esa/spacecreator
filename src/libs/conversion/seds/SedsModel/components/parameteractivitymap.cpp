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

#include "components/parameteractivitymap.h"

namespace seds::model {

const ParameterMapData&
ParameterActivityMap::provided() const
{
    return m_provided;
}

void
ParameterActivityMap::setProvided(ParameterMapData provided)
{
    m_provided = std::move(provided);
}

const ParameterMapData&
ParameterActivityMap::required() const
{
    return m_required;
}

void
ParameterActivityMap::setRequired(ParameterMapData required)
{
    m_required = std::move(required);
}

Body*
ParameterActivityMap::getActivity() const
{
    return m_getActivity.get();
}

void
ParameterActivityMap::setGetActivity(std::unique_ptr<Body> getActivity)
{
    m_getActivity = std::move(getActivity);
}

Body*
ParameterActivityMap::setActivity() const
{
    return m_setActivity.get();
}

void
ParameterActivityMap::setSetActivity(std::unique_ptr<Body> setActivity)
{
    m_setActivity = std::move(setActivity);
}

} // namespace seds::model
