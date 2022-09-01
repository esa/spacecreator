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

#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <shared/qstringhash.h>

using Asn1Acn::Types::Type;
using conversion::translator::TranslationException;
using ivm::IVInterface;
using msc::MscParameter;
using shared::InterfaceParameter;

namespace conversion::sdl::translator {

MscParameterValueParser::MscParameterValueParser(const QString &chartName, const Asn1Acn::File *observerAsn1File)
    : m_chartName(chartName)
    , m_observerAsn1File(observerAsn1File)
{
}

MscParameterValueParser::SignalsParametersRequirementsMap MscParameterValueParser::parseSignals(
        const std::unordered_map<uint32_t, SignalInfo> &signals) const
{
    SignalsParametersRequirementsMap signalsParametersRequirements;

    for (const auto &[signalId, signalInfo] : signals) {
        const auto &parameterList = signalInfo.parameterList;

        if (parameterList.empty()) {
            continue;
        }

        auto signalParametersRequirements = parseSignal(signalInfo);

        signalsParametersRequirements.insert({ signalId, std::move(signalParametersRequirements) });
    }

    return signalsParametersRequirements;
}

MscParameterValueParser::SignalParametersRequirements MscParameterValueParser::parseSignal(
        const SignalInfo &signalInfo) const
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

    SignalParametersRequirements signalParametersRequirements;

    for (int parameterIndex = 0; parameterIndex < parametersTypes.size(); ++parameterIndex) {
        const auto &ivParameterTypeName = parametersTypes.at(parameterIndex);
        const auto &mscParameter = mscParameters.at(parameterIndex);

        const auto asn1ValueMap = parseParameter(ivParameterTypeName, mscParameter, ivInterfaceName, parameterIndex);

        ParameterRequirementsMap parameterRequirements;

        const auto signalVariableName = QString("%1_param%2").arg(signalInfo.signal->name()).arg(parameterIndex);
        parseValueMap(asn1ValueMap, signalVariableName, false, parameterRequirements);

        signalParametersRequirements.push_back(std::move(parameterRequirements));
    }

    return signalParametersRequirements;
}

QVariantMap MscParameterValueParser::parseParameter(const QString &ivParameterTypeName,
        const MscParameter &mscParameter, const QString &ivInterfaceName, const int parameterIndex) const
{
    if (mscParameter.type() == MscParameter::Type::Unknown) {
        auto errorMessage = QString("MSC parameter #%1 is of unknown type").arg(parameterIndex);
        throw TranslationException(std::move(errorMessage));
    } else if (mscParameter.type() == MscParameter::Type::Expression) {
        auto errorMessage = QString("MSC parameter #%1 is an expression which is unsupported").arg(parameterIndex);
        throw TranslationException(std::move(errorMessage));
    }

    const auto ivParameterType = m_observerAsn1File->typeFromName(ivParameterTypeName);

    if (ivParameterType == nullptr) {
        auto errorMessage = QString("Unable to find ASN.1 type %1 for IV parameter #%2 of interface %3")
                                    .arg(ivParameterTypeName)
                                    .arg(parameterIndex)
                                    .arg(ivInterfaceName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto &mscParameterValue = mscParameter.pattern();

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

void MscParameterValueParser::parseValueMap(const QVariantMap &valueMap, const QString &parentName, const bool isChoice,
        ParameterRequirementsMap &result) const
{
    if (!valueMap.contains("name")) {
        throw TranslationException("Missing 'name' key in value map from ASN.1 value parser");
    }

    const auto &nameValue = valueMap["name"].toString();
    const auto name = nameValue.isEmpty() ? parentName : QString("%1.%2").arg(parentName).arg(nameValue);

    for (auto iter = valueMap.constBegin(); iter != valueMap.constEnd(); ++iter) {
        const auto &key = iter.key();
        const auto &value = iter.value();

        if (key == "name") {
            continue;
        }

        if (key == "seqofvalue") {
            parseValueSequence(value.toList(), parentName, result);
        } else if (key == "children") {
            parseValueChildren(value.toList(), name, result);
        } else if (key == "choice") {
            parseValueMap(value.toMap(), name, true, result);
        } else if (key == "value") {
            if (value.isValid()) {
                result.insert({ name, value.toString() });
            } else if (isChoice) {
                result.insert({ name, std::nullopt });
            }
        } else {
            auto errorMessage = QString("Unknown ASN.1 value map key '%1'").arg(key);
            throw TranslationException(std::move(errorMessage));
        }
    }
}

void MscParameterValueParser::parseValueSequence(
        const QVariantList &seqOfValue, const QString &parentName, ParameterRequirementsMap &result) const
{
    for (int i = 0; i < seqOfValue.size(); ++i) {
        const auto &elem = seqOfValue.at(i);

        if (elem.userType() != QMetaType::QVariantMap) {
            throw TranslationException("ASN.1 value sequence element is not a map");
        }

        const auto name = QString("%1(%2)").arg(parentName).arg(i);
        parseValueMap(elem.toMap(), name, false, result);
    }
}

void MscParameterValueParser::parseValueChildren(
        const QVariantList &children, const QString &name, ParameterRequirementsMap &result) const
{
    for (int i = 0; i < children.size(); ++i) {
        const auto &child = children.at(i);

        if (child.userType() != QMetaType::QVariantMap) {
            throw TranslationException("ASN.1 value children element is not a map");
        }

        parseValueMap(child.toMap(), name, false, result);
    }
}

} // namespace conversion::sdl::translator
