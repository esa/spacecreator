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

#include "ivlibrary.h"
#include "modelchecking/xmelreader.h"
#include "modelchecking/xmelwriter.h"

#include <QDebug>
#include <QFile>
#include <QtTest>

class tst_ModelChecking : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void tst_spinConfigReadWrite();
    void tst_spinConfigEmptyFields();
};

void tst_ModelChecking::initTestCase()
{
    ivm::initIVLibrary();
}

void tst_ModelChecking::tst_spinConfigReadWrite()
{
    QString configFilePath = "config.xml";
    QFile configFile(configFilePath);
    Q_ASSERT(configFile.open(QFile::WriteOnly | QFile::Text));

    ive::SpinConfigData spinConfigInput;
    spinConfigInput.errorLimit = 3;
    spinConfigInput.explorationMode = ive::ExplorationMode::BreadthFirst;
    spinConfigInput.globalInputVectorGenerationLimit = 6;
    spinConfigInput.ifaceGenerationLimits = { { "Iface1", 2 }, { "Iface2", 3 }, { "Iface3", 4 } };
    spinConfigInput.memoryLimitMB = 1024;
    spinConfigInput.numberOfCores = 2;
    spinConfigInput.rawCommandLine = "Dummy cmd line";
    spinConfigInput.searchShortestPath = true;
    spinConfigInput.searchStateLimit = 5000;
    spinConfigInput.timeLimitSeconds = 100;
    spinConfigInput.useBitHashing = true;
    spinConfigInput.useFairScheduling = true;
    spinConfigInput.supportReal = true;
    spinConfigInput.deltaValue = 0.1;

    XmelWriter writer({}, {}, { "DummyFunction" }, {}, spinConfigInput);
    Q_ASSERT(writer.writeFile(&configFile, configFilePath));
    configFile.close();

    Q_ASSERT(configFile.open(QFile::ReadOnly | QFile::Text));
    XmelReader reader;
    Q_ASSERT(reader.read(&configFile) == 0);

    ive::SpinConfigData spinConfig = reader.getSpinConfig();
    Q_ASSERT(spinConfig.errorLimit == spinConfigInput.errorLimit);
    Q_ASSERT(spinConfig.explorationMode == spinConfigInput.explorationMode);
    Q_ASSERT(spinConfig.globalInputVectorGenerationLimit == spinConfigInput.globalInputVectorGenerationLimit);
    Q_ASSERT(spinConfig.ifaceGenerationLimits == spinConfigInput.ifaceGenerationLimits);
    Q_ASSERT(spinConfig.memoryLimitMB == spinConfigInput.memoryLimitMB);
    Q_ASSERT(spinConfig.numberOfCores == spinConfigInput.numberOfCores);
    Q_ASSERT(spinConfig.rawCommandLine == spinConfigInput.rawCommandLine);
    Q_ASSERT(spinConfig.searchShortestPath == spinConfigInput.searchShortestPath);
    Q_ASSERT(spinConfig.searchStateLimit == spinConfigInput.searchStateLimit);
    Q_ASSERT(spinConfig.timeLimitSeconds == spinConfigInput.timeLimitSeconds);
    Q_ASSERT(spinConfig.useBitHashing == spinConfigInput.useBitHashing);
    Q_ASSERT(spinConfig.useFairScheduling == spinConfigInput.useFairScheduling);
    Q_ASSERT(spinConfig.supportReal == spinConfigInput.supportReal);
    Q_ASSERT(abs(spinConfig.deltaValue.value() - spinConfigInput.deltaValue.value()) < 0.0001);

    configFile.remove();
}

void tst_ModelChecking::tst_spinConfigEmptyFields()
{
    QString configFilePath = "config.xml";
    QFile configFile(configFilePath);
    Q_ASSERT(configFile.open(QFile::WriteOnly | QFile::Text));

    ive::SpinConfigData spinConfigInput;
    spinConfigInput.errorLimit = std::nullopt;
    spinConfigInput.explorationMode = ive::ExplorationMode::BreadthFirst;
    spinConfigInput.globalInputVectorGenerationLimit = std::nullopt;
    spinConfigInput.ifaceGenerationLimits = { { "Iface1", 2 }, { "Iface2", 3 }, { "Iface3", 4 } };
    spinConfigInput.memoryLimitMB = std::nullopt;
    spinConfigInput.numberOfCores = std::nullopt;
    spinConfigInput.rawCommandLine = "Dummy cmd line";
    spinConfigInput.searchShortestPath = true;
    spinConfigInput.searchStateLimit = std::nullopt;
    spinConfigInput.timeLimitSeconds = std::nullopt;
    spinConfigInput.useBitHashing = true;
    spinConfigInput.useFairScheduling = true;
    spinConfigInput.deltaValue = std::nullopt;
    spinConfigInput.supportReal = false;

    XmelWriter writer({}, {}, { "DummyFunction" }, {}, spinConfigInput);
    Q_ASSERT(writer.writeFile(&configFile, configFilePath));
    configFile.close();

    Q_ASSERT(configFile.open(QFile::ReadOnly | QFile::Text));
    XmelReader reader;
    Q_ASSERT(reader.read(&configFile) == 0);

    ive::SpinConfigData spinConfig = reader.getSpinConfig();
    Q_ASSERT(spinConfig.errorLimit == std::nullopt);
    Q_ASSERT(spinConfig.explorationMode == spinConfigInput.explorationMode);
    Q_ASSERT(spinConfig.globalInputVectorGenerationLimit == std::nullopt);
    Q_ASSERT(spinConfig.ifaceGenerationLimits == spinConfigInput.ifaceGenerationLimits);
    Q_ASSERT(spinConfig.memoryLimitMB == std::nullopt);
    Q_ASSERT(spinConfig.numberOfCores == std::nullopt);
    Q_ASSERT(spinConfig.rawCommandLine == spinConfigInput.rawCommandLine);
    Q_ASSERT(spinConfig.searchShortestPath == spinConfigInput.searchShortestPath);
    Q_ASSERT(spinConfig.searchStateLimit == std::nullopt);
    Q_ASSERT(spinConfig.timeLimitSeconds == std::nullopt);
    Q_ASSERT(spinConfig.useBitHashing == spinConfigInput.useBitHashing);
    Q_ASSERT(spinConfig.useFairScheduling == spinConfigInput.useFairScheduling);
    Q_ASSERT(spinConfig.supportReal == spinConfigInput.supportReal);
    Q_ASSERT(spinConfig.deltaValue == spinConfigInput.deltaValue);

    configFile.remove();
}

QTEST_MAIN(tst_ModelChecking)

#include "tst_modelchecking.moc"
