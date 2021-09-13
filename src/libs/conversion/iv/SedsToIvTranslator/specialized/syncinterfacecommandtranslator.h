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

#include "specialized/interfacecommandtranslator.h"

#pragma once

namespace conversion::iv::translator {

/**
 * @brief   Translator from SEDS sync interface command to InterfaceView interface
 */
class SyncInterfaceCommandTranslator final : public InterfaceCommandTranslator
{
public:
    /**
     * @brief   Constructor
     *
     * @param   ivFunction      Output interface view function
     * @param   interface       Parent interface
     */
    SyncInterfaceCommandTranslator(const seds::model::Interface &interface, ivm::IVFunction *ivFunction);
    /**
     * @brief   Deleted copy constructor
     */
    SyncInterfaceCommandTranslator(const SyncInterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    SyncInterfaceCommandTranslator(SyncInterfaceCommandTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    SyncInterfaceCommandTranslator &operator=(const SyncInterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    SyncInterfaceCommandTranslator &operator=(SyncInterfaceCommandTranslator &&) = delete;

public:
    auto translateCommand(const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType interfaceType)
            -> void;
};

} // namespace conversion::iv::translator
