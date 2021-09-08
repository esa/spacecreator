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

#include "exporter.h"

#include "Asn1Options/options.h"
#include "asn1editor/asn1itemmodel.h"
#include "export/exceptions.h"

// #include <asn1library/asn1/>

using asn1::Asn1ItemModel;
using conversion::asn1::Asn1Options;
using conversion::exporter::ExportException;

namespace conversion::asn1::exporter {

void Asn1Exporter::exportModel(const Model *model, const Options &options) const
{

    const auto outputFilename = options.value(Asn1Options::outputFilename);
    if (outputFilename == nullptr) {
        throw ExportException("Output filename wasn't specified");
    }

    // TODO
}

} // namespace conversion::asn1::exporter
