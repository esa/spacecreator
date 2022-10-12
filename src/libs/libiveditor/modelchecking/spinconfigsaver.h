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

enum class ExplorationMode
{
    DepthFirst,
    BreadthFirst
};

struct SpinConfigData {
    ExplorationMode explorationMode = ExplorationMode::DepthFirst;
    bool searchShortestPath = false;
    bool useFairScheduling = false;
    bool useBitHashing = false;
    int numberOfCores = 1;
    int timeLimitSeconds = 300;
    int searchStateLimit = 1000000;
    int errorLimit = 1;
    int memoryLimitMB = 2048;
    int globalInputVectorGenerationLimit = 4;
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
    ExplorationMode explorationModeFromStringRef(const QStringRef &explorationModeStr);
    QString interfaceGenerationLimitsToString(const QList<QPair<QString, int>> &interfaceGenerationLimits);
    QList<QPair<QString, int>> parseIfaceGenerationLimits(const QStringRef &interfaceGenerationLimitsStr);
    SpinConfigData configData;
};

#endif
