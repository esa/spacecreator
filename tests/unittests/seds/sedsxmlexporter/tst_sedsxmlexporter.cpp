/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include <QDir>
#include <QObject>
#include <QtTest>
#include <seds/SedsModel/sedsmodel.h>
#include <seds/SedsOptions/options.h>
#include <seds/SedsXmlExporter/exporter.h>
#include <sedsmodelbuilder/sedsmodelbuilder.h>

using conversion::seds::SedsOptions;
using seds::exporter::SedsXmlExporter;
using tests::conversion::common::SedsModelBuilder;
namespace seds::test {

class tst_SedsXmlExporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testExportsInteger();
    void testExportsEnum();
    void testExportsFloat();
    void testExportsString();
};

QString getFileContents(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    return file.readAll().trimmed();
}

void tst_SedsXmlExporter::testExportsInteger()
{
    const auto model = SedsModelBuilder("Model").withIntegerDataType("TestInteger").build();
    conversion::Options options;
    options.add(SedsOptions::outputFilepath, "testExportsInteger.xml");
    SedsXmlExporter exporter;
    try {
        exporter.exportModel(model.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QCOMPARE(getFileContents("testExportsInteger.xml"), getFileContents("resources/testExportsInteger.xml"));
}

void tst_SedsXmlExporter::testExportsEnum()
{
    const auto model = SedsModelBuilder("Model").withEnumeratedDataType("TestEnum", { "a", "b", "c" }).build();
    conversion::Options options;
    options.add(SedsOptions::outputFilepath, "testExportsEnum.xml");
    SedsXmlExporter exporter;
    try {
        exporter.exportModel(model.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QCOMPARE(getFileContents("testExportsEnum.xml"), getFileContents("resources/testExportsEnum.xml"));
}

void tst_SedsXmlExporter::testExportsFloat()
{
    const auto model = SedsModelBuilder("Model").withFloatDataType("TestFloat").build();
    conversion::Options options;
    options.add(SedsOptions::outputFilepath, "testExportsFloat.xml");
    SedsXmlExporter exporter;
    try {
        exporter.exportModel(model.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QCOMPARE(getFileContents("testExportsFloat.xml"), getFileContents("resources/testExportsFloat.xml"));
}

void tst_SedsXmlExporter::testExportsString()
{
    const auto model = SedsModelBuilder("Model").withStringDataType("TestString").build();
    conversion::Options options;
    options.add(SedsOptions::outputFilepath, "testExportsString.xml");
    SedsXmlExporter exporter;
    try {
        exporter.exportModel(model.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QCOMPARE(getFileContents("testExportsString.xml"), getFileContents("resources/testExportsString.xml"));
}

} // namespace seds::test

QTEST_MAIN(seds::test::tst_SedsXmlExporter)

#include "tst_sedsxmlexporter.moc"
