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

#include <conversion/common/translation/translator.h>
#include <ivcore/ivinterface.h>
#include <ivcore/parameter.h>

namespace ivm {
class IVFunction;
class IVModel;
class IVPropertyTemplateConfig;
class InterfaceParameter;
} // namespace ivm

namespace seds::model {
class CommandArgument;
class Component;
class GenericTypeMap;
class Interface;
class InterfaceCommand;
class InterfaceDeclaration;
class Package;
class SedsModel;
enum class CommandArgumentMode;
enum class InterfaceCommandMode;
} // namespace seds::model

namespace conversion::iv::translator {

class SedsToIvTranslator final : public ::conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given SEDS models into an ASN.1 model
     *
     * @param   sources     List of SEDS models
     *
     * @return  ASN.1 model translated from SEDS models
     */
    virtual auto translateModels(std::vector<const Model *> sourceModels, const Options &options) const
            -> std::vector<std::unique_ptr<Model>> override;
    /**
     * @brief   Provides a set of all source model types that are required for the translation
     *
     * @return  Set of required models
     */
    virtual auto getDependencies() const -> std::set<ModelType> override;

private:
    auto translateSedsModel(const seds::model::SedsModel *sedsModel, ivm::IVPropertyTemplateConfig *config,
            const Options &options) const -> std::vector<std::unique_ptr<Model>>;

private:
    auto translatePackage(const seds::model::Package &package, ivm::IVModel *model, bool generateFunction) const
            -> void;

    auto translateComponent(const seds::model::Component &component,
            const std::vector<seds::model::InterfaceDeclaration> &interfaceDeclarations, ivm::IVModel *model) const
            -> ivm::IVFunction *;
    auto translateInterface(const seds::model::Interface &interface,
            const std::vector<const seds::model::InterfaceDeclaration *> &interfaceDeclarations,
            ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction) const -> void;
    auto translateInterfaceCommand(const seds::model::InterfaceCommand &command,
            ivm::IVInterface::InterfaceType interfaceType, const std::vector<seds::model::GenericTypeMap> &typeMaps,
            ivm::IVFunction *ivFunction) const -> void;
    auto translateArgument(const seds::model::CommandArgument &argument,
            const std::vector<seds::model::GenericTypeMap> &typeMaps) const -> ivm::InterfaceParameter;

    auto convertInterfaceCommandMode(seds::model::InterfaceCommandMode commandMode) const
            -> ivm::IVInterface::OperationKind;

    auto convertCommandArgumentMode(seds::model::CommandArgumentMode argumentMode) const
            -> ivm::InterfaceParameter::Direction;
};

} // namespace conversion::iv::translator
