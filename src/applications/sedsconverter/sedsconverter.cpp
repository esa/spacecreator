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

#include "sedsconverter.h"

#include <QDebug>
#include <conversion/asn1/Asn1Registrar/registrar.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/modeltype.h>
#include <conversion/common/options.h>
#include <conversion/converter/converter.h>
#include <conversion/seds/SedsOptions/options.h>
#include <conversion/seds/SedsRegistrar/registrar.h>

using conversion::Converter;
using conversion::ModelType;
using conversion::Options;
using conversion::RegistrationFailedException;
using conversion::Registry;
using conversion::asn1::Asn1Registrar;
using conversion::seds::SedsOptions;
using conversion::seds::SedsRegistrar;

namespace sedsconverter {

SedsConverter::SedsConverter()
{
    initializeRegistry();
}

void SedsConverter::convert(ModelType targetModelType) const
{
    Options options;
    options.add(SedsOptions::inputFilename, "input.xml");
    options.add(SedsOptions::schemaFilename, "seds.xsd");
    options.add(SedsOptions::externalRefFilename, "config.toml");
    options.add(SedsOptions::keepIntermediateFiles);

    Converter converter(m_registry, std::move(options));
    converter.convert({ ModelType::Seds }, targetModelType);
}

void SedsConverter::initializeRegistry()
{
    SedsRegistrar sedsRegistrar;
    auto result = sedsRegistrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::Seds);
    }

    Asn1Registrar asn1Registrar;
    result = asn1Registrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::Asn1);
    }
}

} // namespace sedsconverter
