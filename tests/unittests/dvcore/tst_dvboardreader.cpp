/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvboard.h"
#include "dvboardreaderhelpers.h"
#include "dvhwlibraryreader.h"

#include <QtTest>

class DVBoardReaderTest : public QObject
{
    Q_OBJECT

private:
    void runReader(const QByteArray &content, const int expectedObjectCount = -1, const bool canBeParsed = true,
            QVector<dvm::DVObject *> *objects = nullptr);

private Q_SLOTS:
    void tst_emptyDeploymentViewDoc();
    void tst_singleItems();
};

void DVBoardReaderTest::runReader(const QByteArray &content, const int expectedObjectCount, const bool canBeParsed,
        QVector<dvm::DVObject *> *objects)
{
    QBuffer buffer;
    buffer.setData(content);
    if (!buffer.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    dvm::DVHWLibraryReader reader;
    const bool ok = reader.read(&buffer);
    QCOMPARE(ok, canBeParsed);
    if (ok) {
        const QVector<dvm::DVObject *> objectsList = reader.parsedObjects();
        if (expectedObjectCount != -1) {
            QCOMPARE(objectsList.size(), expectedObjectCount);
        }
        if (objects) {
            *objects = objectsList;
        }
    }
}

void DVBoardReaderTest::tst_emptyDeploymentViewDoc()
{
    runReader(helpers::emptyDoc(), 0, true);
    runReader(QByteArray(), 1, false);
}

void DVBoardReaderTest::tst_singleItems()
{
    runReader(helpers::singleBoard(), 1);
    runReader(helpers::nonValidDoc(), -1, false);
    runReader(helpers::singleBoardWithPort(), 2);
    runReader(helpers::singleBoardWithPorts(), 4);
    runReader(helpers::multiBoardsWithPort(), 4);
    runReader(helpers::unknownProperty(), 0);
    runReader(helpers::openCloseTagMismatched(), -1, false);
    runReader(helpers::malformedXml(), -1, false);
}

QTEST_APPLESS_MAIN(DVBoardReaderTest)

#include "tst_dvboardreader.moc"
