/*
   Copyright (C) 2022 N7Space - <akoszewski@n7space.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
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

    SpinConfigData spinConfigInput;
    spinConfigInput.errorLimit = 3;
    spinConfigInput.explorationMode = ExplorationMode::BreadthFirst;
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

    XmelWriter writer({}, {}, { "DummyFunction" }, {}, spinConfigInput);
    Q_ASSERT(writer.writeFile(&configFile, configFilePath));
    configFile.close();

    Q_ASSERT(configFile.open(QFile::ReadOnly | QFile::Text));
    XmelReader reader;
    Q_ASSERT(reader.read(&configFile) == 0);

    SpinConfigData spinConfig = reader.getSpinConfig();
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

    configFile.remove();
}

QTEST_MAIN(tst_ModelChecking)

#include "tst_modelchecking.moc"
