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

#ifndef SPINCONFIGSAVER_H
#define SPINCONFIGSAVER_H

#include <QIODevice>
#include <QList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <optional>

namespace ive {
enum class ExplorationMode
{
    DepthFirst,
    BreadthFirst
};

struct SpinConfigData {
    static std::optional<int> optionalIntFromString(const QString str);
    static QString optionalIntToString(std::optional<int> opt);
    static std::optional<float> optionalFloatFromString(const QString str);
    static QString optionalFloatToString(std::optional<float> opt);

    ExplorationMode explorationMode = ExplorationMode::DepthFirst;
    bool searchShortestPath = false;
    bool useFairScheduling = false;
    bool useBitHashing = false;
    bool supportReal = false;
    bool supportMulticast = false;
    std::optional<int> numberOfCores = 1;
    std::optional<int> timeLimitSeconds = 300;
    std::optional<int> searchStateLimit = 1000000;
    std::optional<int> errorLimit = 1;
    std::optional<int> memoryLimitMB = 2048;
    std::optional<int> globalInputVectorGenerationLimit = 4;
    std::optional<int> vectorszValue = 65535;
    std::optional<float> deltaValue;
    QList<QPair<QString, int>> ifaceGenerationLimits;
    QString rawCommandLine; // if non-empty, overrides other configuration settings
};

class SpinConfigSaver
{
public:
    SpinConfigSaver();
    void saveSpinConfig(const SpinConfigData &configData, QXmlStreamWriter &xml);
    bool readSpinConfig(QXmlStreamReader &xml);
    SpinConfigData getConfigData() const;

private:
    QString explorationModeToString(const ExplorationMode &explorationMode);
    ExplorationMode explorationModeFromString(const QString &explorationModeStr);
    QString interfaceGenerationLimitsToString(const QList<QPair<QString, int>> &interfaceGenerationLimits);
    QList<QPair<QString, int>> parseIfaceGenerationLimits(const QString &interfaceGenerationLimitsStr);
    SpinConfigData configData;
};
}

#endif
