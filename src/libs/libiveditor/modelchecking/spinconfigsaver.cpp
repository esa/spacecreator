/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "spinconfigsaver.h"

#include <QDebug>

SpinConfigSaver::SpinConfigSaver() { }

SpinConfigData SpinConfigSaver::getConfigData() const
{
    return configData;
}

QString SpinConfigSaver::explorationModeToString(const ExplorationMode &explorationMode)
{
    switch (explorationMode) {
    case ExplorationMode::BreadthFirst:
        return QString("BreadthFirst");
    case ExplorationMode::DepthFirst:
        return QString("DepthFirst");
    }
    return "";
}

ExplorationMode SpinConfigSaver::explorationModeFromString(const QString &explorationModeStr)
{
    if (explorationModeStr == "BreadthFirst") {
        return ExplorationMode::BreadthFirst;
    } else {
        return ExplorationMode::DepthFirst;
    }
}

QString SpinConfigSaver::interfaceGenerationLimitsToString(const QList<QPair<QString, int>> &interfaceGenerationLimits)
{
    QString generationLimitsStr;
    for (const auto &limitPair : interfaceGenerationLimits) {
        generationLimitsStr += limitPair.first + " " + QString::number(limitPair.second) + ";";
    }
    return generationLimitsStr;
}

QList<QPair<QString, int>> SpinConfigSaver::parseIfaceGenerationLimits(const QString &interfaceGenerationLimitsStr)
{
    constexpr int INTERFACE_IDX = 0;
    constexpr int LIMIT_IDX = 1;
    constexpr int LIMIT_PAIR_SIZE = 2;

    QList<QPair<QString, int>> ifaceGenerationLimits;
    for (const auto limitPairStr : interfaceGenerationLimitsStr.split(";")) {
        QStringList limitPairVect = limitPairStr.split(" ");
        if (limitPairVect.length() == LIMIT_PAIR_SIZE) {
            ifaceGenerationLimits.append(
                    QPair<QString, int> { limitPairVect[INTERFACE_IDX], limitPairVect[LIMIT_IDX].toInt() });
        }
    }
    return ifaceGenerationLimits;
}

QString readAttribute(const QString &attribute, const QXmlStreamReader &xml)
{
    if (xml.attributes().hasAttribute(attribute)) {
        return xml.attributes().value(attribute).toString();
    } else {
        qWarning() << "Error, no attribute " << attribute << " in the xmel file";
    }
    return {};
}

std::optional<int> optionalIntFromString(const QString str)
{
    return str.isEmpty() ? std::nullopt : std::optional<int>(str.toInt());
}

QString optionalIntToString(std::optional<int> opt)
{
    return opt == std::nullopt ? "" : QString::number(opt.value());
}

void SpinConfigSaver::saveSpinConfig(const SpinConfigData &configData, QXmlStreamWriter &xml)
{
    xml.writeStartElement("SPINModelChecker");

    xml.writeAttribute("errorlimit", optionalIntToString(configData.errorLimit));
    xml.writeAttribute("explorationmode", explorationModeToString(configData.explorationMode));
    xml.writeAttribute("inputvectorgenerationlimit", optionalIntToString(configData.globalInputVectorGenerationLimit));
    xml.writeAttribute("ifacegenerationlimits", interfaceGenerationLimitsToString(configData.ifaceGenerationLimits));
    xml.writeAttribute("memorylimitmb", optionalIntToString(configData.memoryLimitMB));
    xml.writeAttribute("numberofcores", optionalIntToString(configData.numberOfCores));
    xml.writeAttribute("rawcommandline", configData.rawCommandLine);
    xml.writeAttribute("searchshortestpath", QString::number(configData.searchShortestPath));
    xml.writeAttribute("searchstatelimit", optionalIntToString(configData.searchStateLimit));
    xml.writeAttribute("timelimitseconds", optionalIntToString(configData.timeLimitSeconds));
    xml.writeAttribute("usebithashing", QString::number(configData.useBitHashing));
    xml.writeAttribute("usefairscheduling", QString::number(configData.useFairScheduling));

    xml.writeEndElement();
}

bool SpinConfigSaver::readSpinConfig(QXmlStreamReader &xml)
{
    configData.errorLimit = optionalIntFromString(readAttribute("errorlimit", xml));
    configData.explorationMode = explorationModeFromString(readAttribute("explorationmode", xml));
    configData.globalInputVectorGenerationLimit = optionalIntFromString(readAttribute("inputvectorgenerationlimit", xml));
    configData.ifaceGenerationLimits = parseIfaceGenerationLimits(readAttribute("ifacegenerationlimits", xml));
    configData.memoryLimitMB = optionalIntFromString(readAttribute("memorylimitmb", xml));
    configData.numberOfCores = optionalIntFromString(readAttribute("numberofcores", xml));
    configData.rawCommandLine = readAttribute("rawcommandline", xml);
    configData.searchShortestPath = readAttribute("searchshortestpath", xml).toInt();
    configData.searchStateLimit = optionalIntFromString(readAttribute("searchstatelimit", xml));
    configData.timeLimitSeconds = optionalIntFromString(readAttribute("timelimitseconds", xml));
    configData.useBitHashing = readAttribute("usebithashing", xml).toInt();
    configData.useFairScheduling = readAttribute("usefairscheduling", xml).toInt();

    xml.skipCurrentElement();
    return true;
}
