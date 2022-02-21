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

#include "dvtools.h"

#include <QBuffer>
#include <QDebug>
#include <dvcore/dvobject.h>
#include <dvcore/dvxmlreader.h>
#include <memory>
#include <stdexcept>

namespace tests {
namespace dvtools {

auto getDvObjectsFromXml(const QByteArray &content) -> std::unique_ptr<QVector<dvm::DVObject *>>
{
    QBuffer buffer;
    buffer.setData(content);
    if (!buffer.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("XML data cannot be opened");
    }

    dvm::DVXMLReader reader;
    const bool readSuccessfull = reader.read(&buffer);
    if (!readSuccessfull) {
        throw std::runtime_error("XML data cannot be read");
    }

    auto objects = std::make_unique<QVector<dvm::DVObject *>>(reader.parsedObjects());

    return objects;
}

} // namespace dvtools
} // namespace tests
