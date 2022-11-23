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

#pragma once

#include <QStringList>
#include <seds/SedsModel/device/metadata/metadatacategory.h>
#include <seds/SedsModel/device/metadata/metadatavalueset.h>

namespace ivm {
class IVComment;
} // namespace ivm

namespace seds::model {
class Device;
} // namespace seds::model

namespace conversion::iv::translator::seds {

/**
 * @brief   Translator from SEDS device to InterfaceView elements
 */
class DeviceTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sedsDevice      Device to translate
     */
    explicit DeviceTranslator(const ::seds::model::Device &sedsDevice);
    /**
     * @brief   Deleted copy constructor
     */
    DeviceTranslator(const DeviceTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    DeviceTranslator(DeviceTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    DeviceTranslator &operator=(const DeviceTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    DeviceTranslator &operator=(DeviceTranslator &&) = delete;

public:
    /**
     * @brief   Translate metadata to a IV comment
     *
     * @return  InterfaceView comment
     */
    auto translateMetadata() const -> ivm::IVComment *;

private:
    auto translateMetadataValue(const ::seds::model::MetadataValueType &metadataValue) const -> QString;
    auto translateMetadataCategory(const ::seds::model::MetadataCategory &metadataCategory, const int indent) const
            -> QStringList;

private:
    static const inline QString m_metadataValueTemplate = "%1: %2";

    const ::seds::model::Device &m_sedsDevice;
};

} // namespace conversion::iv::translator::seds
