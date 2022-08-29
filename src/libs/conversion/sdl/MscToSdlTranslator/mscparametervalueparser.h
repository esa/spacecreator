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

#include "signalinfo.h"

#include <QVariantMap>
#include <asn1library/asn1/file.h>
#include <asn1library/asn1/types/type.h>
#include <asn1library/asn1valueparser.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivmodel.h>
#include <shared/parameter.h>
#include <unordered_map>

namespace conversion::sdl::translator {

/**
 * @brief   Parser for MSC parameter ASN.1 values
 */
class MscParameterValueParser final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   observerAsn1File    ASN.1 with observer dataview
     * @param   ivModel             IV model
     */
    MscParameterValueParser(
            const QString &chartName, const Asn1Acn::File *observerAsn1File, const ivm::IVModel *ivModel);
    /**
     * @brief   Deleted copy constructor
     */
    MscParameterValueParser(const MscParameterValueParser &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    MscParameterValueParser(MscParameterValueParser &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    MscParameterValueParser &operator=(const MscParameterValueParser &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    MscParameterValueParser &operator=(MscParameterValueParser &&) = delete;

public:
    /**
     * @brief   Parse given signals
     *
     * @param   signals     Signals to parse
     */
    auto parseSignals(const std::unordered_map<uint32_t, SignalInfo> &signals) const -> void;

private:
    auto parseSignal(const SignalInfo &signalInfo, const ivm::IVInterface *ivInterface) const -> void;
    auto parseParameter(const shared::InterfaceParameter &ivParameter, const msc::MscParameter &mscParameter,
            const QString &ivInterfaceName) const -> QVariantMap;

    auto findIvInterface(const QString &ivFunctionName, const QString &ivInterfaceName) const -> ivm::IVInterface *;
    auto getIvParameterType(const shared::InterfaceParameter &ivParameter) const -> const Asn1Acn::Types::Type *;

private:
    QString m_chartName;
    const Asn1Acn::File *m_observerAsn1File;
    const ivm::IVModel *m_ivModel;

    Asn1Acn::Asn1ValueParser m_asn1ValueParser;
};

} // namespace conversion::sdl::translator
