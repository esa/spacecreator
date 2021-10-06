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

#include <asn1library/asn1/asn1model.h>
#include <memory>

namespace Asn1Acn {
class Definitions;
} // namespace Asn1Acn

namespace tests::conversion::common {

class Asn1ModelBuilder final
{
public:
    Asn1ModelBuilder(QString name);
    std::unique_ptr<Asn1Acn::Asn1Model> build();

public:
    auto withIntegerDataType(QString name) -> Asn1ModelBuilder &;

private:
    std::unique_ptr<Asn1Acn::File> m_file;
    Asn1Acn::Definitions *m_definitions;
};

} // namespace tests::conversion::common
