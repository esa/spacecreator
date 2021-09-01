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

#include "translator.h"

#include <QDebug>
#include <QVector>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <seds/SedsModel/interfaces/commandargumentmode.h>
#include <seds/SedsModel/sedsmodel.h>

using conversion::iv::IvOptions;
using conversion::translator::IncorrectSourceModelException;
using conversion::translator::TranslationException;
using conversion::translator::UndeclaredInterfaceException;
using conversion::translator::UnhandledValueException;
using conversion::translator::UnsupportedValueException;
using ivm::IVModel;
using seds::model::SedsModel;

namespace conversion::iv::translator {

std::unique_ptr<Model> SedsToIvTranslator::translateModels(
        std::vector<const Model *> sourceModels, const Options &options) const
{
    if (sourceModels.empty()) {
        throw TranslationException("No models passed for translation");
    } else if (sourceModels.size() > 1) {
        throw TranslationException("Only one model is allowed for translation");
    }

    const auto *sourceModel = sourceModels[0];
    if (sourceModel == nullptr) {
        throw TranslationException("Source model is null");
    }

    const auto *sedsModel = dynamic_cast<const SedsModel *>(sourceModel);
    if (sedsModel == nullptr) {
        throw IncorrectSourceModelException(ModelType::Seds);
    }

    const auto configFilename = options.value(IvOptions::configFilename);
    if (!configFilename) {
        throw TranslationException("InterfaceView configuration file wasn't specified");
    }

    ivm::IVPropertyTemplateConfig *config = ivm::IVPropertyTemplateConfig::instance();
    config->init(*configFilename);

    return translateSedsModel(sedsModel, config, options);
}

std::set<ModelType> SedsToIvTranslator::getDependencies() const
{
    return std::set<ModelType> { ModelType::Seds };
}

std::unique_ptr<IVModel> SedsToIvTranslator::translateSedsModel(
        const SedsModel *sedsModel, ivm::IVPropertyTemplateConfig *config, const Options &options) const
{
    const auto generateFunctionsForPackages = options.isSet(IvOptions::generateFunctionsForPackages);

    auto ivModel = std::make_unique<IVModel>(config);

    const auto &sedsModelData = sedsModel->data();
    if (std::holds_alternative<seds::model::PackageFile>(sedsModelData)) {
        const auto &package = std::get<seds::model::PackageFile>(sedsModelData).package();
        translatePackage(package, ivModel.get(), generateFunctionsForPackages);
    } else if (std::holds_alternative<seds::model::DataSheet>(sedsModelData)) {
        const auto &packages = std::get<seds::model::DataSheet>(sedsModelData).packages();
        for (const auto &package : packages) {
            translatePackage(package, ivModel.get(), generateFunctionsForPackages);
        }
    } else {
        throw TranslationException("Unhandled SEDS model data type");
    }

    return ivModel;
}

void SedsToIvTranslator::translatePackage(
        const seds::model::Package &package, IVModel *model, bool generateFunction) const
{
    QVector<ivm::IVFunction *> components;

    for (const auto &component : package.components()) {
        components.append(translateComponent(component, package.declaredInterfaces(), model));
    }

    if (generateFunction) {
        auto *ivFunction = new ivm::IVFunction();
        ivFunction->setEntityAttribute(
                ivm::meta::Props::token(ivm::meta::Props::Token::name), package.qualifiedName().name().value());

        for (auto *component : components) {
            ivFunction->addChild(component);
        }

        model->addObject(ivFunction);
    } else {
        model->addObjects(components);
    }
}

ivm::IVFunction *SedsToIvTranslator::translateComponent(const seds::model::Component &component,
        const std::vector<seds::model::InterfaceDeclaration> &interfaceDeclarations, IVModel *model) const
{
    auto *ivFunction = new ivm::IVFunction();
    ivFunction->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::name), component.name().value());

    const auto globalInterfaceDeclarationsCount = interfaceDeclarations.size();

    std::vector<const seds::model::InterfaceDeclaration *> allInterfaceDeclarations;
    for (const auto &interfaceDeclaration : interfaceDeclarations) {
        allInterfaceDeclarations.push_back(&interfaceDeclaration);
    }

    for (const auto &providedInterface : component.providedInterfaces()) {
        for (const auto &interfaceDeclaration : component.declaredInterfaces()) {
            allInterfaceDeclarations.push_back(&interfaceDeclaration);
        }

        translateInterface(
                providedInterface, allInterfaceDeclarations, ivm::IVInterface::InterfaceType::Provided, ivFunction);

        allInterfaceDeclarations.resize(globalInterfaceDeclarationsCount);
    }

    for (const auto &requiredInterface : component.requiredInterfaces()) {
        for (const auto &interfaceDeclaration : component.declaredInterfaces()) {
            allInterfaceDeclarations.push_back(&interfaceDeclaration);
        }

        translateInterface(
                requiredInterface, allInterfaceDeclarations, ivm::IVInterface::InterfaceType::Required, ivFunction);

        allInterfaceDeclarations.resize(globalInterfaceDeclarationsCount);
    }

    return ivFunction;
}

void SedsToIvTranslator::translateInterface(const seds::model::Interface &interface,
        const std::vector<const seds::model::InterfaceDeclaration *> &interfaceDeclarations,
        ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction) const
{
    const auto &interfaceTypeName = interface.type().value().name().value();

    const auto interfaceDeclaration = std::find_if(interfaceDeclarations.begin(), interfaceDeclarations.end(),
            [&interfaceTypeName](const seds::model::InterfaceDeclaration *interfaceDeclaration) {
                return interfaceDeclaration->name().value() == interfaceTypeName;
            });
    if (interfaceDeclaration == interfaceDeclarations.end()) {
        throw UndeclaredInterfaceException(interfaceTypeName);
    }

    for (const auto &command : (*interfaceDeclaration)->commands()) {
        translateInterfaceCommand(command, interfaceType, ivFunction);
    }
}

void SedsToIvTranslator::translateInterfaceCommand(const seds::model::InterfaceCommand &command,
        ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction) const
{
    ivm::IVInterface::CreationInfo creationInfo;
    creationInfo.function = ivFunction;
    creationInfo.type = interfaceType;
    creationInfo.name = command.name().value();
    creationInfo.kind = convertInterfaceCommandMode(command.mode());

    auto *interface = ivm::IVInterface::createIface(creationInfo);

    for (const auto &argument : command.arguments()) {
        interface->addParam(translateArgument(argument));
    }

    ivFunction->addChild(interface);
}

ivm::InterfaceParameter SedsToIvTranslator::translateArgument(const seds::model::CommandArgument &argument) const
{
    return ivm::InterfaceParameter(argument.name().value(), ivm::BasicParameter::Type::Other,
            argument.type().value().name().value(), QStringLiteral("ACN"), convertCommandArgumentMode(argument.mode()));
}

ivm::IVInterface::OperationKind SedsToIvTranslator::convertInterfaceCommandMode(
        seds::model::InterfaceCommandMode commandMode) const
{
    switch (commandMode) {
    case seds::model::InterfaceCommandMode::Sync:
        return ivm::IVInterface::OperationKind::Protected;
    case seds::model::InterfaceCommandMode::Async:
        return ivm::IVInterface::OperationKind::Sporadic;
    default:
        throw UnhandledValueException("InterfaceCommandMode");
        break;
    }
}

ivm::InterfaceParameter::Direction SedsToIvTranslator::convertCommandArgumentMode(
        seds::model::CommandArgumentMode argumentMode) const
{
    switch (argumentMode) {
    case seds::model::CommandArgumentMode::In:
        return ivm::InterfaceParameter::Direction::IN;
    case seds::model::CommandArgumentMode::Out:
        return ivm::InterfaceParameter::Direction::OUT;
    case seds::model::CommandArgumentMode::InOut:
    case seds::model::CommandArgumentMode::Notify:
        throw UnsupportedValueException("InterfaceCommandMode");
        break;
    default:
        throw UnhandledValueException("InterfaceCommandMode");
        break;
    }
}

} // namespace conversion::iv::translator
