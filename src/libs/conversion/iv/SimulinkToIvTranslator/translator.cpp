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

#include "translator.h"

#include "options.h"
#include "specialized/portstranslator.h"

#include <QFileInfo>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <ivcore/ivmodel.h>

using conversion::Escaper;
using conversion::iv::IvOptions;
using conversion::iv::SimulinkToIvOptions;
using conversion::translator::TranslationException;
using ivm::IVFunction;
using ivm::IVInterface;
using ivm::IVModel;
using ivm::IVPropertyTemplateConfig;
using ivm::meta::Props;
using simulink::model::SimulinkModel;

namespace conversion::iv::translator::simulink {

// clang-format off
const QStringList SimulinkToIvTranslator::m_supportedIVFunctionImplementations =
{
    "QGenC"
};
// clang-format on

std::vector<std::unique_ptr<Model>> SimulinkToIvTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    checkSourceModelCount(sourceModels);

    const auto *simulinkModel = getModel<SimulinkModel>(sourceModels);

    const auto ivConfigFilePath = getIVConfigFilePathIfExists(options);

    IVPropertyTemplateConfig *ivConfig = IVPropertyTemplateConfig::instance();
    ivConfig->init(ivConfigFilePath);

    return translateSimulinkModel(*simulinkModel, *ivConfig, options);
}

ModelType SimulinkToIvTranslator::getSourceModelType() const
{
    return ModelType::Simulink;
}

ModelType SimulinkToIvTranslator::getTargetModelType() const
{
    return ModelType::InterfaceView;
}

std::set<ModelType> SimulinkToIvTranslator::getDependencies() const
{
    static std::set<ModelType> dependencies { ModelType::Simulink };
    return dependencies;
}

std::vector<std::unique_ptr<Model>> SimulinkToIvTranslator::translateSimulinkModel(
        const SimulinkModel &simulinkModel, IVPropertyTemplateConfig &ivConfig, const Options &options) const
{
    auto *ivFunction = createIVFunction(options);
    auto *ivInterface = createIVInterface(simulinkModel.name(), ivFunction);

    ivFunction->addChild(ivInterface);

    PortsTranslator portsTranslator(simulinkModel, *ivInterface);
    portsTranslator.translatePorts();

    auto ivModel = std::make_unique<IVModel>(&ivConfig);
    ivModel->addObject(ivFunction);

    std::vector<std::unique_ptr<Model>> resultModels;
    resultModels.push_back(std::move(ivModel));

    return resultModels;
}

bool SimulinkToIvTranslator::isIVFunctionImplementationSupported(const QString &ivFunctionImplementation)
{
    return m_supportedIVFunctionImplementations.contains(ivFunctionImplementation, Qt::CaseSensitive);
}

QString SimulinkToIvTranslator::getIVConfigFilePathIfExists(const Options &options)
{
    const auto ivConfigFilePath = options.value(IvOptions::configFilepath);
    if (!ivConfigFilePath) {
        throw TranslationException("InterfaceView configuration file wasn't specified");
    }

    QFileInfo ivConfigFile(*ivConfigFilePath);
    if (!ivConfigFile.exists()) {
        auto errorMsg = QString("InterfaceView configuration file '%1' doesn't exist").arg(*ivConfigFilePath);
        throw TranslationException(std::move(errorMsg));
    }

    return *ivConfigFilePath;
}

IVFunction *SimulinkToIvTranslator::createIVFunction(const Options &options)
{
    const auto ivFunctionName = options.value(SimulinkToIvOptions::ivFunctionName);
    if (!ivFunctionName) {
        throw TranslationException("IV Function block name wasn't specified");
    }

    const auto ivFunctionImplementation = options.value(SimulinkToIvOptions::ivFunctionImplementation);
    if (!ivFunctionImplementation) {
        throw TranslationException("IV Function block implementation wasn't specified");
    }

    if (!isIVFunctionImplementationSupported(*ivFunctionImplementation)) {
        auto errorMsg = QString("'%1' IV Function block implementation isn't supported").arg(*ivFunctionImplementation);
        throw TranslationException(std::move(errorMsg));
    }

    IVFunction *ivFunction = new IVFunction();

    ivFunction->setEntityAttribute(Props::token(Props::Token::name), Escaper::escapeIvName(*ivFunctionName));
    ivFunction->setEntityAttribute(Props::token(Props::Token::language), *ivFunctionImplementation);
    ivFunction->setEntityAttribute(Props::token(Props::Token::is_type), "NO");

    return ivFunction;
}

IVInterface *SimulinkToIvTranslator::createIVInterface(const QString &interfaceName, IVFunction *ivFunction)
{
    IVInterface::CreationInfo creationInfo;

    creationInfo.name = Escaper::escapeIvName(interfaceName);
    creationInfo.type = IVInterface::InterfaceType::Provided;
    creationInfo.kind = IVInterface::OperationKind::Protected;
    creationInfo.function = ivFunction;

    return IVInterface::createIface(creationInfo);
}

} // namespace conversion::iv::translator::simulink
