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

#include "dvobject.h"
#include "dvpartition.h"
#include "dvxmlreader.h"
#include "dvxmlreaderhelpers.h"

#include <QtTest>

class DVXMLReaderTest : public QObject
{
    Q_OBJECT

private:
    void runReader(const QByteArray &content, const int expectedObjectCount = -1, const bool canBeParsed = true,
            QVector<dvm::DVObject *> *objects = nullptr);

private Q_SLOTS:
    void tst_emptyDeploymentViewDoc();
    void tst_singleItems();
    void tst_functions();
};

void DVXMLReaderTest::runReader(const QByteArray &content, const int expectedObjectCount, const bool canBeParsed,
        QVector<dvm::DVObject *> *objects)
{
    QBuffer buffer;
    buffer.setData(content);
    if (!buffer.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    dvm::DVXMLReader reader;
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
    } else {
        qWarning() << reader.errorString();
    }
}

void DVXMLReaderTest::tst_emptyDeploymentViewDoc()
{
    runReader(helpers::emptyDoc(), 0, true);
    runReader(QByteArray(), 1, false);
}

void DVXMLReaderTest::tst_singleItems()
{
    runReader(helpers::singleNode(), 1);
    runReader(helpers::nonValidDoc(), -1, false);
    runReader(helpers::singlePartitionWithNode(), 2);
    runReader(helpers::singlePartitionOnly(), 1);
    runReader(helpers::singlePartitionWithFunctions(), 1);
    runReader(helpers::singleDeviceWithNode(), 2);
    runReader(helpers::singleDeviceOnly(), 1);
    runReader(helpers::singleNodeWithPartitionAndDevice(), 3);
    runReader(helpers::unknownProperty(), 0);
    runReader(helpers::openCloseTagMismatched(), -1, false);
    runReader(helpers::malformedXml(), -1, false);
    runReader(helpers::twoConnections(), 2);
    runReader(helpers::twoConnectionsAndNode(), 3);
    runReader(helpers::twoConnectionsAndNodeWithPartitionAndDevice(), 5);
    runReader(helpers::twoConnectionsAndNodeWithDeviceAndPartitionWithFunctions(), 5);
}

void DVXMLReaderTest::tst_functions()
{
    auto findPartition = [](const QVector<dvm::DVObject *> &objects) -> dvm::DVPartition * {
        auto it = std::find_if(objects.begin(), objects.end(),
                [](const dvm::DVObject *obj) { return obj->deployType() == dvm::DVObject::Type::Partition; });
        if (it != objects.end()) {
            return (*it)->as<dvm::DVPartition *>();
        }
        return nullptr;
    };

    QVector<dvm::DVObject *> objects;
    runReader(helpers::twoConnectionsAndNodeWithPartitionAndDevice(), -1, true, &objects);
    dvm::DVPartition *partition = findPartition(objects);
    Q_ASSERT(partition);
    QVERIFY(partition->functions().isEmpty());

    objects.clear();
    runReader(helpers::singlePartitionWithNode(), -1, true, &objects);
    partition = findPartition(objects);
    Q_ASSERT(partition);
    QVERIFY(partition->functions().isEmpty());

    objects.clear();
    runReader(helpers::singlePartitionOnly(), -1, true, &objects);
    partition = findPartition(objects);
    Q_ASSERT(partition);
    QVERIFY(partition->functions().isEmpty());

    objects.clear();
    runReader(helpers::singlePartitionWithFunctions(), -1, true, &objects);
    partition = findPartition(objects);
    Q_ASSERT(partition);
    QCOMPARE(partition->functions().size(), 10);

    objects.clear();
    runReader(helpers::twoConnectionsAndNodeWithDeviceAndPartitionWithFunctions(), -1, true, &objects);
    partition = findPartition(objects);
    Q_ASSERT(partition);
    QCOMPARE(partition->functions().size(), 10);
}

QTEST_APPLESS_MAIN(DVXMLReaderTest)

#include "tst_dvxmlreader.moc"
