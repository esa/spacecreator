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

#pragma once

namespace ivm {
class IVFunction;
class IVInterface;
} // namespace ivm

namespace seds::model {
class Component;
class Package;
} // namespace seds

namespace conversion::seds::translator {

/**
 * @brief   Translator from InterfaceView functions to SEDS components
 */
class FunctionsTranslator final
{
public:
    /**
     * @brief   Deleted constructor
     */
    FunctionsTranslator() = delete;
    /**
     * @brief   Deleted copy constructor
     */
    FunctionsTranslator(const FunctionsTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    FunctionsTranslator(FunctionsTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    FunctionsTranslator &operator=(const FunctionsTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    FunctionsTranslator &operator=(FunctionsTranslator &&) = delete;

public:
    /**
     * @brief   Translates InterfaceView functions to SEDS components
     */
    static auto translateFunction(const ivm::IVFunction *ivFunction, ::seds::model::Package &sedsPackage) -> void;

private:
    static auto translateInterface(const ivm::IVInterface *ivInterface, ::seds::model::Component &sedsComponent)
            -> void;
};

} // namespace conversion::seds::translator
