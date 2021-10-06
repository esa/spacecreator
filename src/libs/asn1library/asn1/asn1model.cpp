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

#include "asn1model.h"

namespace Asn1Acn {

Asn1Model::Asn1Model(Asn1Model::Data data)
    : m_data(std::move(data))
{ }

conversion::ModelType Asn1Model::modelType() const
{
    return conversion::ModelProperties<Asn1Model>::type;
}

const Asn1Model::Data& Asn1Model::data() const
{
    return m_data;
}

Asn1Model::Data& Asn1Model::data()
{
    return m_data;
}

} // namespace Asn1Acn
