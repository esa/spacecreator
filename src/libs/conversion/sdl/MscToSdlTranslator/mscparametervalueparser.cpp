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

#include "mscparametervalueparser.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <shared/qstringhash.h>

using Asn1Acn::Types::Type;
using conversion::translator::TranslationException;
using ivm::IVInterface;
using msc::MscParameter;
using shared::InterfaceParameter;

namespace conversion::sdl::translator {

MscParameterValueParser::MscParameterValueParser(const QString &chartName, const Asn1Acn::File *asn1File)
    : m_chartName(chartName)
    , m_asn1File(asn1File)
{
}

MscParameterValueParser::SignalRequirementsMap MscParameterValueParser::parseSignals(
        const std::unordered_map<uint32_t, SignalInfo> &signalsMap) const
{
    SignalRequirementsMap signalRequirements;

    for (const auto &[signalId, signalInfo] : signalsMap) {
        const auto &parameterList = signalInfo.parameterList;

        if (parameterList.empty()) {
            continue;
        }

        auto parametersRequirements = parseSignal(signalInfo);
        signalRequirements.insert({ signalId, std::move(parametersRequirements) });
    }

    return signalRequirements;
}

MscParameterValueParser::ParametersRequirements MscParameterValueParser::parseSignal(const SignalInfo &signalInfo) const
{
    const auto &mscParameters = signalInfo.parameterList;
    const auto &parametersTypes = signalInfo.signal->parametersTypes();

    const auto &signal = signalInfo.signal;

    if (mscParameters.size() != parametersTypes.size()) {
        auto errorMessage =
                QString("Wrong number of parameters in message %1 (chart %2)").arg(signal->name()).arg(m_chartName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto &ivInterfaceName = signalInfo.signal->referencedName();

    ParametersRequirements parametersRequirements;

    for (int parameterIndex = 0; parameterIndex < parametersTypes.size(); ++parameterIndex) {
        const auto &ivParameterTypeName = parametersTypes.at(parameterIndex);
        const auto &mscParameter = mscParameters.at(parameterIndex);

        const auto asn1ValueMap = parseParameter(ivParameterTypeName, mscParameter, ivInterfaceName, parameterIndex);

        const auto signalVariableName = QString("%1_param%2").arg(signalInfo.signal->name()).arg(parameterIndex);
        parseValueMap(asn1ValueMap, signalVariableName, true, false, parametersRequirements);
    }

    return parametersRequirements;
}

QVariantMap MscParameterValueParser::parseParameter(const QString &ivParameterTypeName,
        const MscParameter &mscParameter, const QString &ivInterfaceName, const int parameterIndex) const
{
    const auto ivParameterType = m_asn1File->typeFromName(Escaper::escapeAsn1TypeName(ivParameterTypeName));

    if (ivParameterType == nullptr) {
        auto errorMessage = QString("Unable to find ASN.1 type %1 for IV parameter #%2 of interface %3")
                                    .arg(ivParameterTypeName)
                                    .arg(parameterIndex)
                                    .arg(ivInterfaceName);
        throw TranslationException(std::move(errorMessage));
    }

    QString mscParameterValue;

    switch (mscParameter.type()) {
    case MscParameter::Type::Unknown: {
        auto errorMessage = QString("MSC parameter #%1 is of unknown type").arg(parameterIndex);
        throw TranslationException(std::move(errorMessage));
    } break;
    case MscParameter::Type::Pattern: {
        mscParameterValue = mscParameter.pattern();
    } break;
    case MscParameter::Type::Expression: {
        mscParameterValue = mscParameter.expression();
    }
    }

    bool ok;
    auto asn1Value = m_asn1ValueParser.parseAsn1Value(ivParameterType, mscParameterValue, &ok);

    if (!ok) {
        auto errorMessage = QString("Failed to parse ASN.1 value '%1' for IV parameter #%2 of type %3")
                                    .arg(mscParameterValue)
                                    .arg(parameterIndex)
                                    .arg(ivParameterTypeName);
        throw TranslationException(std::move(errorMessage));
    }

    return asn1Value;
}

void MscParameterValueParser::parseValueMap(const QVariantMap &valueMap, const QString &parentName, const bool skipName,
        const bool isChoice, ParametersRequirements &result) const
{
    if (!valueMap.contains("name")) {
        throw TranslationException("Missing 'name' key in value map from ASN.1 value parser");
    }

    const auto &nameValue = valueMap["name"].toString();

    auto name = parentName;
    if (!skipName && !nameValue.isEmpty()) {
        name += QString(".%2").arg(nameValue);
    }

    // If we are currently handling a choice parameter we want to first check if given option is present
    // We annotate that by using nullopt as the value, because we only check the parameter itself, not the value
    if (isChoice) {
        result.push_back({ name, std::nullopt });
    }

    for (auto iter = valueMap.constBegin(); iter != valueMap.constEnd(); ++iter) {
        const auto &key = iter.key();
        const auto &value = iter.value();

        if (key == "name") {
            continue;
        }

        if (key == "seqofvalue") {
            parseValueSequence(value.toList(), name, result);
        } else if (key == "children") {
            parseValueChildren(value.toList(), name, result);
        } else if (key == "choice") {
            parseValueMap(value.toMap(), name, false, true, result);
        } else if (key == "value") {
            // If the value is invalid it means that value is present but it can be any value (denoted as *)
            // If any value is allowed then we don't need to create any value requirement for that parameter
            if (value.isValid()) {
                result.push_back({ name, value.toString() });
            }
        } else {
            auto errorMessage = QString("Unknown ASN.1 value map key '%1'").arg(key);
            throw TranslationException(std::move(errorMessage));
        }
    }
}

void MscParameterValueParser::parseValueSequence(
        const QVariantList &seqOfValue, const QString &parentName, ParametersRequirements &result) const
{
    // add requirement for length of SEQUENCEOF
    // function length works for both fixed-size and variable-size
    QString lengthExpression = QString("length(%1)").arg(parentName);
    result.push_back({ lengthExpression, QString::number(seqOfValue.size()) });

    for (int i = 0; i < seqOfValue.size(); ++i) {
        const auto &elem = seqOfValue.at(i);

        if (elem.userType() != QMetaType::QVariantMap) {
            throw TranslationException("ASN.1 value sequence element is not a map");
        }

        const auto name = QString("%1(%2)").arg(parentName).arg(i);
        parseValueMap(elem.toMap(), name, false, false, result);
    }
}

void MscParameterValueParser::parseValueChildren(
        const QVariantList &children, const QString &name, ParametersRequirements &result) const
{
    for (int i = 0; i < children.size(); ++i) {
        const auto &child = children.at(i);

        if (child.userType() != QMetaType::QVariantMap) {
            throw TranslationException("ASN.1 value children element is not a map");
        }

        parseValueMap(child.toMap(), name, false, false, result);
    }
}

} // namespace conversion::sdl::translator
