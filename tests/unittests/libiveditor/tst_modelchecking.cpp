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

#include "modelchecking/xmelreader.h"
#include "modelchecking/xmelwriter.h"
#include "ivlibrary.h"

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
    Q_ASSERT(configFile.open(QFile::ReadWrite | QFile::Text) == true);

    SpinConfigData spinConfigInput;

    XmelWriter writer({}, {}, {}, {}, spinConfigInput);
    Q_ASSERT(writer.writeFile(&configFile, configFilePath) == true);

    XmelReader reader;
    Q_ASSERT(reader.read(&configFile) == 0);

    SpinConfigData spinConfig = reader.getSpinConfig();
    qDebug() << "Result: " << spinConfig.numberOfCores;

    configFile.remove();
}

QTEST_MAIN(tst_ModelChecking)

#include "tst_modelchecking.moc"
