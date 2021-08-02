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

#include "components/parametermapdata.h"
#include "components/activities/body.h"

#include <memory>

namespace seds::model {

class ParameterActivityMap final
{
  public:
    ParameterActivityMap() = default;
    ParameterActivityMap(ParameterActivityMap&&) = default;
    ParameterActivityMap& operator=(ParameterActivityMap&&) = default;

  public:
    auto provided() const -> const ParameterMapData&;
    auto setProvided(ParameterMapData provided) -> void;

    auto required() const -> const ParameterMapData&;
    auto setRequired(ParameterMapData provided) -> void;

    auto getActivity() const -> Body*;
    auto setGetActivity(std::unique_ptr<Body> getActivity) -> void;

    auto setActivity() const -> Body*;
    auto setSetActivity(std::unique_ptr<Body> setActivity) -> void;

  private:
    ParameterMapData m_provided;
    ParameterMapData m_required;
    std::unique_ptr<Body> m_getActivity;
    std::unique_ptr<Body> m_setActivity;
};

} // namespace seds::model
