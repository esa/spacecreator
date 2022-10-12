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

#include <asn1library/asn1/asn1model.h>
#include <conversion/common/translation/translator.h>
#include <ivcore/ivmodel.h>
#include <promela/IvToPromelaTranslator/translator.h>
#include <simulatortrail/SimulatorTrailModel/simulatortrailmodel.h>
#include <spintrail/SpinTrailModel/channelevent.h>
#include <spintrail/SpinTrailModel/continuoussignal.h>
#include <spintrail/SpinTrailModel/spintrailmodel.h>

namespace simulatortrail::translator {
class SpinTrailToSimulatorTrailTranslator final : public ::conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given ASN.1 model into an Promela model
     *
     * @param   sourceModels    List of ASN.1 models
     * @param   options         List of options
     *
     * @return  Promela model translated from the ASN.1 models
     */
    std::vector<std::unique_ptr<conversion::Model>> translateModels(
            std::vector<conversion::Model *> sourceModels, const conversion::Options &options) const override;

    /**
     * @brief   Return a model type that is a source of this translator
     *
     * @return  Source model type
     */
    conversion::ModelType getSourceModelType() const override;
    /**
     * @brief   Returns a model type that is a target of this translator
     *
     * @return  Target model type
     */
    conversion::ModelType getTargetModelType() const override;
    /**
     * @brief   Provides a set of all source model types that are required for the translation
     *
     * @return  Set of required models
     */
    std::set<conversion::ModelType> getDependencies() const override;

private:
    struct ChannelInfo {
        QString m_functionName;
        QString m_interfaceName;
        QString m_parameterName;
        QString m_parameterTypeName;
        const Asn1Acn::Types::Type *m_parameterType;
        QMap<QString, QString> m_possibleSenders;
        size_t m_channelSize;
        QStringList m_senders;
    };

private:
    void findChannelNames(const promela::translator::IvToPromelaTranslator::SystemInfo &systemInfo,
            const Asn1Acn::Asn1Model &asn1Model, QMap<QString, ChannelInfo> &channel,
            QMap<QString, std::pair<ChannelInfo, bool>> &observerChannels) const;
    void findProctypes(const promela::translator::IvToPromelaTranslator::SystemInfo &systemInfo,
            QMap<QString, QString> &proctypes) const;
    void translate(simulatortrail::model::SimulatorTrailModel &result,
            const spintrail::model::SpinTrailModel &spinTrailModel, QMap<QString, ChannelInfo> &channels,
            QMap<QString, std::pair<ChannelInfo, bool>> &observerChannels, const QMap<QString, QString> &proctypes,
            const Asn1Acn::Types::Type *observableEvent) const;
    void processSpinTrailEvent(simulatortrail::model::SimulatorTrailModel &result,
            const spintrail::model::ChannelEvent *event, QMap<QString, ChannelInfo> &channels,
            QMap<QString, std::pair<ChannelInfo, bool>> &observerChannels, const QMap<QString, QString> &proctypes,
            const Asn1Acn::Types::Type *observableEvent) const;
    void processSpinTrailEvent(
            simulatortrail::model::SimulatorTrailModel &result, const spintrail::model::ContinuousSignal *event) const;
    Asn1Acn::ValuePtr getValue(const QString &source, const QString &target, const ChannelInfo &info,
            const Asn1Acn::Types::Type *observableEvent, const QStringList &parameters, bool isInput) const;

    bool isFunctionLockChannel(const QString &channelName) const;
};
}
