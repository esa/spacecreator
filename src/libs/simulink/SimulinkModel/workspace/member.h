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

#include <QString>

namespace simulink::model {

class Member final
{
public:
    Member() = default;
    Member(Member &&) = default;
    Member &operator=(Member &&) = default;

public:
    auto name() const -> const QString &;
    auto setName(QString name) -> void;

    auto dataType() const -> const QString &;
    auto setDataType(QString dataType) -> void;

    auto complexity() const -> const QString &;
    auto setComplexity(QString complexity) -> void;

    auto description() const -> const QString &;
    auto setDescription(QString description) -> void;

    auto dimensions() const -> const QString &;
    auto setDimensions(QString dimensions) -> void;

    auto dimensionsMode() const -> const QString &;
    auto setDimensionsMode(QString dimensionsMode) -> void;

    auto max() const -> const QString &;
    auto setMax(QString max) -> void;

    auto min() const -> const QString &;
    auto setMin(QString min) -> void;

    auto sampleTime() const -> const QString &;
    auto setSampleTime(QString sampleTime) -> void;

    auto unit() const -> const QString &;
    auto setUnit(QString unit) -> void;

private:
    QString m_name;
    QString m_dataType;
    QString m_complexity;
    QString m_description;
    QString m_dimensions;
    QString m_dimensionsMode;
    QString m_max;
    QString m_min;
    QString m_sampleTime;
    QString m_unit;
};

} // namespace simulink::model
