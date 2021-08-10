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

#include "symboldefinitionreader.h"

#include <QFileInfo>
#include <QVariant>
#include <conversion/converter/import/exceptions.h>
#define TOML_EXCEPTIONS 0
#include "exceptions.h"

#include <seds/ThirdParty/toml.h>

using converter::import::FileNotFound;

namespace seds::symbolreader {

SymbolDefinitionReader::ExternalReferencesMap SymbolDefinitionReader::readSymbols(QString filename)
{
    const QFileInfo fileInfo(filename);
    if (!fileInfo.exists()) {
        throw FileNotFound(filename, "while reading symbol definitions");
    }

    const auto result = toml::parse_file(filename.toStdString());
    if (!result) {
        throw SymbolDefinitionReaderException(filename, result.error().description());
    }

    ExternalReferencesMap externalReferences;

    for (auto &&[key, value] : result) {
        const auto referenceName = QString::fromStdString(key);

        value.visit([&](auto &&node) {
            if constexpr (toml::is_string<decltype(node)>) {
                externalReferences.insert({ referenceName, QString::fromStdString(*node) });
            } else if constexpr (toml::is_integer<decltype(node)> || toml::is_floating_point<decltype(node)>) {
                externalReferences.insert({ referenceName, QString::number(*node) });
            } else if constexpr (toml::is_boolean<decltype(node)>) {
                externalReferences.insert({ referenceName, QVariant(*node).toString() });
            } else {
                const auto message = QString("key '%1' was expected to be a string, an integer, a float or a boolean")
                                             .arg(referenceName);
                throw SymbolDefinitionReaderException(filename, message);
            }
        });
    }

    return externalReferences;
}

} // namespace seds::symbolreader
