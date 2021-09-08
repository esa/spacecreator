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

#include "registrar.h"

#include <conversion/common/modeltype.h>
#include <conversion/iv/IvXmlImporter/importer.h>
#include <memory>

namespace conversion::iv {

using importer::IvXmlImporter;

bool IvRegistrar::registerCapabilities(conversion::Registry &registry)
{
    auto ivImporter = std::make_unique<IvXmlImporter>();
    auto result = registry.registerImporter(ModelType::InterfaceView, std::move(ivImporter));
    if (!result) {
        return false;
    }

    return true;
}

} // namespace conversion::iv
