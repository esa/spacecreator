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

#include "specialized/devicetranslator.h"

#include <conversion/common/overloaded.h>
#include <ivcore/ivcomment.h>
#include <seds/SedsModel/device/device.h>

namespace conversion::iv::translator::seds {

DeviceTranslator::DeviceTranslator(const ::seds::model::Device &sedsDevice)
    : m_sedsDevice(sedsDevice)
{
}

ivm::IVComment *DeviceTranslator::translateMetadata() const
{
    const auto &sedsDeviceMetadata = m_sedsDevice.metadata();

    if (!sedsDeviceMetadata) {
        return nullptr;
    }

    QStringList commentContent;

    for (const auto &metadataValue : sedsDeviceMetadata->values()) {
        commentContent << translateMetadataValue(metadataValue);
    }

    for (const auto &metadataCategory : sedsDeviceMetadata->categories()) {
        commentContent << translateMetadataCategory(metadataCategory, 1);
    }

    auto ivComment = new ivm::IVComment();
    ivComment->setTitle(commentContent.join("\\n"));

    return ivComment;
}

QString DeviceTranslator::translateMetadataValue(const ::seds::model::MetadataValueType &metadataValue) const
{
    QString result;

    // clang-format off
    std::visit(overloaded {
        [&](const auto &value) {
            result = m_metadataValueTemplate.arg(value.nameStr()).arg(value.value());
        }
    }, metadataValue);
    // clang-format on

    return result;
}

QStringList DeviceTranslator::translateMetadataCategory(
        const ::seds::model::MetadataCategory &metadataCategory, const int indent) const
{
    QStringList result;

    QStringList categoryHeader;
    categoryHeader << QString(indent, '*');
    categoryHeader << metadataCategory.nameStr();
    categoryHeader << QString(indent, '*');

    result << categoryHeader.join(' ');

    for (const auto &metadataValue : metadataCategory.values()) {
        result << translateMetadataValue(metadataValue);
    }

    for (const auto &metadataSubcategory : metadataCategory.subcategories()) {
        result << translateMetadataCategory(metadataSubcategory, indent + 1);
    }

    return result;
}

} // namespace conversion::iv::translator::seds
