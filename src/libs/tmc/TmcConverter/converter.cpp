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

#include "converter.h"

#include <conversion/asn1/Asn1Registrar/registrar.h>
#include <conversion/converter/converter.h>
#include <conversion/converter/exceptions.h>
#include <conversion/iv/IvRegistrar/registrar.h>
#include <conversion/promela/PromelaRegistrar/registrar.h>

using conversion::Converter;
using conversion::ModelType;
using conversion::RegistrationFailedException;
using conversion::asn1::Asn1Registrar;
using conversion::iv::IvRegistrar;
using conversion::promela::PromelaRegistrar;

namespace tmc::converter {
TmcConverter::TmcConverter()
{
    Asn1Registrar asn1Registrar;
    bool result = asn1Registrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::Asn1);
    }
    IvRegistrar ivRegistrar;
    result = ivRegistrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::InterfaceView);
    }
    PromelaRegistrar tmcRegistrar;
    result = tmcRegistrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::Promela);
    }
}

void TmcConverter::convert(std::set<conversion::ModelType> sourceModelTypes, conversion::ModelType targetModelType,
        const std::set<conversion::ModelType> auxilaryModelTypes, conversion::Options options) const
{
    Converter converter(m_registry, std::move(options));
    converter.convert(sourceModelTypes, targetModelType, auxilaryModelTypes);
}
}
