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
#include <QProcess>
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

private:
    void compareResults(const QString &filename);

    static inline const QString m_ignoredAttrs = "id,version";
};

void tst::SedsXmlExporter::compareResults(const QString &filename)
{
    QProcess diffProcess;

    QStringList args;
    args << "--check"
         << "--ignored-attrs" << m_ignoredAttrs << filename << QString("resources/%1").arg(filename);

    diffProcess.start("xmldiff", args);
    diffProcess.waitForFinished();

    if (diffProcess.exitCode() != 0) {
        QString diffOutput(diffProcess.readAllStandardOutput());
        QFAIL(diffOutput.toStdString().c_str());
    }
}

void tst_SedsXmlExporter::testExportsInteger()
{
    const auto model = SedsModelBuilder("Model").withIntegerDataType("TestInteger").build();

    const QString filename("testExportsInteger.xml");

    conversion::Options options;
    options.add(SedsOptions::outputFilepath, filename);

    SedsXmlExporter exporter;
    try {
        exporter.exportModel(model.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    compareResults(filename);
}

void tst_SedsXmlExporter::testExportsEnum()
{
    const auto model = SedsModelBuilder("Model").withEnumeratedDataType("TestEnum", { "a", "b", "c" }).build();

    const QString filename("testExportsEnum.xml");

    conversion::Options options;
    options.add(SedsOptions::outputFilepath, filename);

    SedsXmlExporter exporter;
    try {
        exporter.exportModel(model.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    compareResults(filename);
}

void tst_SedsXmlExporter::testExportsFloat()
{
    const auto model = SedsModelBuilder("Model").withFloatDataType("TestFloat").build();

    const QString filename("testExportsFloat.xml");

    conversion::Options options;
    options.add(SedsOptions::outputFilepath, filename);

    SedsXmlExporter exporter;
    try {
        exporter.exportModel(model.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    compareResults(filename);
}

void tst_SedsXmlExporter::testExportsString()
{
    const auto model = SedsModelBuilder("Model").withStringDataType("TestString").build();

    const QString filename("testExportsString.xml");

    conversion::Options options;
    options.add(SedsOptions::outputFilepath, filename);

    SedsXmlExporter exporter;
    try {
        exporter.exportModel(model.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    compareResults(filename);
}

} // namespace seds::test

QTEST_MAIN(seds::test::tst_SedsXmlExporter)

#include "tst_sedsxmlexporter.moc"
