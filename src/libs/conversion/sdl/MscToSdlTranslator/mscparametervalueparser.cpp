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
#include <iostream>
#include <ivcore/ivfunction.h>

using Asn1Acn::Types::Type;
using conversion::translator::TranslationException;
using ivm::IVInterface;
using msc::MscParameter;
using shared::InterfaceParameter;

namespace conversion::sdl::translator {

MscParameterValueParser::MscParameterValueParser(
        const QString &chartName, const Asn1Acn::File *observerAsn1File, const ivm::IVModel *ivModel)
    : m_chartName(chartName)
    , m_observerAsn1File(observerAsn1File)
    , m_ivModel(ivModel)
{
}

void MscParameterValueParser::parseSignals(const std::unordered_map<uint32_t, SignalInfo> &signals) const
{
    for (const auto &[signalId, signalInfo] : signals) {
        const auto &parameterList = signalInfo.parameterList;

        if (parameterList.empty()) {
            continue;
        }

        const auto &ivFunctionName = signalInfo.signal->referencedFunctionName();
        const auto &ivInterfaceName = signalInfo.signal->referencedName();

        const auto ivInterface = findIvInterface(ivFunctionName, ivInterfaceName);

        parseSignal(signalInfo, ivInterface);
    }
}

void MscParameterValueParser::parseSignal(const SignalInfo &signalInfo, const IVInterface *ivInterface) const
{
    const auto &ivParameters = ivInterface->params();
    const auto &mscParameters = signalInfo.parameterList;

    const auto &signal = signalInfo.signal;

    if (mscParameters.size() != ivParameters.size()) {
        auto errorMessage =
                QString("Wrong number of parameters in message %1 (chart %2)").arg(signal->name()).arg(m_chartName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto &ivInterfaceName = ivInterface->title();

    for (int i = 0; i < ivParameters.size(); ++i) {
        const auto &ivParameter = ivParameters.at(i);
        const auto &mscParameter = mscParameters.at(i);

        const auto asn1Value = parseParameter(ivParameter, mscParameter, ivInterfaceName);
    }
}

QVariantMap MscParameterValueParser::parseParameter(
        const InterfaceParameter &ivParameter, const MscParameter &mscParameter, const QString &ivInterfaceName) const
{
    const auto &ivParameterName = ivParameter.name();
    const auto &ivParameterTypeName = ivParameter.paramTypeName();

    if (mscParameter.type() == MscParameter::Type::Unknown) {
        auto errorMessage = QString("MSC parameter for IV parameter %1 is of unknown type").arg(ivParameterName);
        throw TranslationException(std::move(errorMessage));
    } else if (mscParameter.type() == MscParameter::Type::Expression) {
        auto errorMessage =
                QString("MSC parameter for IV parameter %1 is an expression which is unsupported").arg(ivParameterName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto ivParameterType = getIvParameterType(ivParameter);
    if (ivParameterType == nullptr) {
        auto errorMessage = QString("Unable to find ASN.1 type %1 for IV parameter %2 of interface %3")
                                    .arg(ivParameterTypeName)
                                    .arg(ivParameterName)
                                    .arg(ivInterfaceName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto &mscParameterValue = mscParameter.pattern();

    bool ok;
    auto asn1Value = m_asn1ValueParser.parseAsn1Value(ivParameterType, mscParameterValue, &ok);

    if (!ok) {
        auto errorMessage = QString("Failed to parse ASN.1 value '%1' for IV parameter %2 of type %3")
                                    .arg(mscParameterValue)
                                    .arg(ivParameterName)
                                    .arg(ivParameterTypeName);
        throw TranslationException(std::move(errorMessage));
    }

    return asn1Value;
}

IVInterface *MscParameterValueParser::findIvInterface(
        const QString &ivFunctionName, const QString &ivInterfaceName) const
{
    const auto ivFunction = m_ivModel->getFunction(ivFunctionName, Qt::CaseInsensitive);

    if (ivFunction == nullptr) {
        auto errorMessage = QString("Unable to find function named %1 in given InterfaceView").arg(ivFunctionName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto ivInterfaces = ivFunction->interfaces();
    const auto ivInterfaceFound = std::find_if(ivInterfaces.begin(), ivInterfaces.end(),
            [&](const auto interface) { return interface->title() == ivInterfaceName; });

    if (ivInterfaceFound == ivInterfaces.end()) {
        auto errorMessage = QString("Unable to find interface named %1 in function %2 in given InterfaceView")
                                    .arg(ivInterfaceName)
                                    .arg(ivFunctionName);
        throw TranslationException(std::move(errorMessage));
    }

    return *ivInterfaceFound;
}

const Type *MscParameterValueParser::getIvParameterType(const InterfaceParameter &ivParameter) const
{
    const auto &ivParameterTypeName = ivParameter.paramTypeName();
    return m_observerAsn1File->typeFromName(ivParameterTypeName);
}

} // namespace conversion::sdl::translator
