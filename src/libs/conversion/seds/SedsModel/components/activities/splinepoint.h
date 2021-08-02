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

namespace seds::model {

class SplinePoint
{
  public:
    SplinePoint() noexcept;
    SplinePoint(SplinePoint&&) = default;
    SplinePoint& operator=(SplinePoint&&) = default;

  public:
    auto order() const -> int;
    auto setOrder(int order) -> void;

    auto raw() const -> double;
    auto setRaw(double raw) -> void;

    auto calibrated() const -> double;
    auto setCalibrated(double calibrated) -> void;

  private:
    int m_order;
    double m_raw;
    double m_calibrated;
};

} // namespace seds::model
