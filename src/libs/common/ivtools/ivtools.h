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

#include <QList>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <vector>

namespace plugincommon {

/**
 * @brief  Class to store utility functions facilitating easier work with InterfaceView data model
 *
 */
class IvTools
{
public:
    /**
     * @brief Get functions stored in a given model
     *
     * @param model InterfaceView model
     *
     * @return functions stored in the given model
     */
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static auto getFunctions(ivm::IVModel *model) -> std::vector<ivm::IVFunction *>;

#else
    static auto getFunctions(ivm::IVModel *model) -> QList<ivm::IVFunction *>;
#endif

    /**
     * @brief Get the requested interface from IVModel
     *
     * @param ifaceName name of the requested interface
     * @param model     interfaceview data model
     *
     * @return IV interface
     */
    static auto getIfaceFromModel(const QString &ifaceName, ivm::IVModel *model) -> ivm::IVInterface *;
};

} // namespace plugincommon
