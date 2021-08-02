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

#include "base/positivelong.h"
#include "types/encodings/dataencoding.h"

#include <optional>
#include <variant>

namespace seds::model {

enum class CoreStringEncoding
{
    ASCII,
    UTF8
};

class StringDataEncoding final : public DataEncoding
{
public:
    /** DoTStringEncoding, ExtStringEncoding */
    using Encoding = std::variant<CoreStringEncoding>;

public:
    StringDataEncoding() noexcept;

public:
    auto encoding() const -> const std::optional<Encoding> &;
    auto setEncoding(Encoding encoding) -> void;

    auto terminationByte() const -> const std::optional<uint8_t> &;
    auto setTerminationByte(uint8_t terminationByte) -> void;

private:
    std::optional<Encoding> m_encoding;
    std::optional<uint8_t> m_terminationByte;
};

} // namespace seds::model
