/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "context.h"

#include <QString>
#include <seds/SedsModel/interfaces/commandargument.h>
#include <seds/SedsModel/types/dimensionsize.h>
#include <vector>

namespace conversion::asn1::translator {

/**
 * @brief   Helper for data types translation
 */
class DataTypeTranslationHelper final
{
public:
    /**
     * @brief   Deleted constructor
     */
    DataTypeTranslationHelper() = delete;

public:
    static auto buildArrayTypeName(
            const QString &baseTypeName, const std::vector<seds::model::DimensionSize> &dimensions) -> QString;
    static auto buildBundledTypeName(const QString &commandName, const QString &interfaceName, QString postfix = "")
            -> QString;
    static auto buildGenericBundledTypeName(
            const QString &componentName, const QString &interfaceName, const QString &commandName) -> QString;
    static auto buildConcreteTypeName(
            const QString &componentName, const QString &interfaceName, const QString &genericName) -> QString;

    static auto handleArrayArgumentType(Context &context, const seds::model::DataTypeRef &argumentTypeRef,
            const std::vector<seds::model::DimensionSize> &dimensions) -> Asn1Acn::Types::Type *;

private:
    inline static const QString m_arrayTypeNameTemplate = "%1Array-%2";
    inline static const QString m_bundledTypeNameTemplate = "%1-%2-Type%3";
    inline static const QString m_genericBundledTypeNameTemplate = "%1-%2-%3Type";
    inline static const QString m_concreteTypeNameTemplate = "%1-%2-%3";
};

} // namespace conversion::asn1::translator
