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

#include "datatypesdependencyresolver.h"
#include "specialized/datatypetranslatorvisitor.h"
#include "specialized/portdatatypetranslator.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/simulink/matlabstandarddatatypes.h>
#include <simulink/SimulinkModel/simulinkmodel.h>

using Asn1Acn::Asn1Model;
using Asn1Acn::Definitions;
using Asn1Acn::File;
using Asn1Acn::SourceLocation;
using conversion::Escaper;
using conversion::simulink::MatLabStandardDataTypes;
using simulink::model::DataType;
using simulink::model::SimulinkModel;

namespace conversion::asn1::translator {

ModelType SimulinkToAsn1Translator::getSourceModelType() const
{
    return ModelType::Simulink;
}

ModelType SimulinkToAsn1Translator::getTargetModelType() const
{
    return ModelType::Asn1;
}

std::set<ModelType> SimulinkToAsn1Translator::getDependencies() const
{
    static std::set<ModelType> dependencies { ModelType::Simulink };
    return dependencies;
}

std::vector<std::unique_ptr<Model>> SimulinkToAsn1Translator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    checkSourceModelCount(sourceModels);

    const auto *simulinkModel = getModel<SimulinkModel>(sourceModels);

    return translateSimulinkModel(*simulinkModel, options);
}

std::vector<std::unique_ptr<Model>> SimulinkToAsn1Translator::translateSimulinkModel(
        const SimulinkModel &simulinkModel, const Options &options) const
{
    auto matlabStandardDataTypesAsn1AcnDefinitions = std::make_unique<Definitions>(
            MatLabStandardDataTypes::getStandardDataTypesEscapedPackageName(), SourceLocation());

    const auto simulinkModelAsn1AcnDefinitionsName = Escaper::escapeAsn1PackageName(simulinkModel.name());
    auto simulinkModelAsn1AcnDefinitions =
            std::make_unique<Definitions>(simulinkModelAsn1AcnDefinitionsName, SourceLocation());

    Context context(matlabStandardDataTypesAsn1AcnDefinitions, simulinkModelAsn1AcnDefinitions, options);

    addMatLabStandardDataTypes(context);

    translateDataTypesOfModel(context, simulinkModel);
    translateDataTypesOfPorts(context, simulinkModel);

    auto matlabStandardDataTypesAsn1AcnFile =
            std::make_unique<File>(MatLabStandardDataTypes::getStandardDataTypesEscapedPackageName());
    matlabStandardDataTypesAsn1AcnFile->add(std::move(matlabStandardDataTypesAsn1AcnDefinitions));

    auto simulinkModelAsn1AcnFile = std::make_unique<File>(simulinkModelAsn1AcnDefinitionsName);
    simulinkModelAsn1AcnFile->add(std::move(simulinkModelAsn1AcnDefinitions));

    auto asn1Model = std::make_unique<Asn1Model>();

    asn1Model->addAsn1File(std::move(matlabStandardDataTypesAsn1AcnFile));
    asn1Model->addAsn1File(std::move(simulinkModelAsn1AcnFile));

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(asn1Model));

    return result;
}

void SimulinkToAsn1Translator::addMatLabStandardDataTypes(Context &context) const
{
    context.addMatLabStandardAsn1Type(MatLabStandardDataTypes::getStandardAsn1BooleanDataType());

    context.addMatLabStandardAsn1Type(MatLabStandardDataTypes::getStandardAsn1DoubleDataType());
    context.addMatLabStandardAsn1Type(MatLabStandardDataTypes::getStandardAsn1SingleDataType());

    context.addMatLabStandardAsn1Type(MatLabStandardDataTypes::getStandardAsn1Int8DataType());
    context.addMatLabStandardAsn1Type(MatLabStandardDataTypes::getStandardAsn1UInt8DataType());

    context.addMatLabStandardAsn1Type(MatLabStandardDataTypes::getStandardAsn1Int16DataType());
    context.addMatLabStandardAsn1Type(MatLabStandardDataTypes::getStandardAsn1UInt16DataType());

    context.addMatLabStandardAsn1Type(MatLabStandardDataTypes::getStandardAsn1Int32DataType());
    context.addMatLabStandardAsn1Type(MatLabStandardDataTypes::getStandardAsn1UInt32DataType());
}

void SimulinkToAsn1Translator::translateDataTypesOfModel(Context &context, const SimulinkModel &simulinkModel) const
{
    DataTypesDependencyResolver dataTypesDependencyResolver(simulinkModel.dataTypes());
    const auto &resolvedSimulinkModelDataTypes = dataTypesDependencyResolver.resolve();

    DataTypeTranslatorVisitor dataTypeTranslator(context);

    for (const auto dataType : resolvedSimulinkModelDataTypes) {
        std::visit(dataTypeTranslator, *dataType);
    }
}

void SimulinkToAsn1Translator::translateDataTypesOfPorts(Context &context, const SimulinkModel &simulinkModel) const
{
    PortDataTypeTranslator portDataTypeTranslator(context);

    for (const auto &inport : simulinkModel.inports()) {
        portDataTypeTranslator.translate(inport);
    }

    for (const auto &outport : simulinkModel.outports()) {
        portDataTypeTranslator.translate(outport);
    }
}

} // namespace conversion::asn1::translator
