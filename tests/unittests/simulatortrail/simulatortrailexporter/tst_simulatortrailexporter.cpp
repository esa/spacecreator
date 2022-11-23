/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include <QObject>
#include <QtTest>
#include <asn1library/asn1/choicevalue.h>
#include <asn1library/asn1/multiplevalue.h>
#include <asn1library/asn1/namedvalue.h>
#include <asn1library/asn1/singlevalue.h>
#include <simulatortrail/SimulatorTrailExporter/simulatortrailexporter.h>
#include <simulatortrail/SimulatorTrailModel/simulatortrailmodel.h>
#include <simulatortrail/SimulatorTrailOptions/options.h>
#include <simulatortrail/SpinTrailToSimulatorTrailTranslator/ia5stringprinter.h>
#include <simulatortrail/SpinTrailToSimulatorTrailTranslator/octetstringprinter.h>

namespace simulatortrail::test {
using Asn1Acn::ChoiceValue;
using Asn1Acn::MultipleValue;
using Asn1Acn::NamedValue;
using Asn1Acn::SingleValue;
using Asn1Acn::ValuePtr;
using conversion::simulatortrail::SimulatorTrailOptions;
using simulatortrail::exporter::SimulatorTrailExporter;
using simulatortrail::model::SimulatorTrailModel;
using simulatortrail::translator::IA5StringPrinter;
using simulatortrail::translator::OctetStringPrinter;

class tst_SimulatorTrailExporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testBasicTypes();
    void testStructures();

private:
    QString exportModel(const SimulatorTrailModel &model);
    QString getFileContents(const QString &filename);
};

void tst_SimulatorTrailExporter::testBasicTypes()
{
    SimulatorTrailModel model;
    model.appendValue(std::make_unique<SingleValue>("4"));
    model.appendValue(std::make_unique<SingleValue>("enumValue"));
    model.appendValue(std::make_unique<SingleValue>("test", IA5StringPrinter()));
    model.appendValue(std::make_unique<SingleValue>("special'char", IA5StringPrinter()));
    model.appendValue(std::make_unique<SingleValue>("00ffaa99", OctetStringPrinter()));

    QString out = exportModel(model);
    QString expected = getFileContents("expected_basic_types.sim");
    QCOMPARE(out, expected);
}

void tst_SimulatorTrailExporter::testStructures()
{
    SimulatorTrailModel model;
    std::unique_ptr<NamedValue> namedValue = std::make_unique<NamedValue>();
    namedValue->addValue("a", std::make_unique<SingleValue>("0"));
    namedValue->addValue("b", std::make_unique<SingleValue>("1"));
    std::unique_ptr<ChoiceValue> choiceValue =
            std::make_unique<ChoiceValue>("some", std::make_unique<SingleValue>("3"));
    std::unique_ptr<MultipleValue> multipleValue = std::make_unique<MultipleValue>();
    multipleValue->addValue(std::make_unique<SingleValue>("4"));
    multipleValue->addValue(std::make_unique<SingleValue>("5"));
    multipleValue->addValue(std::make_unique<SingleValue>("6"));
    model.appendValue(std::move(namedValue));
    model.appendValue(std::move(choiceValue));
    model.appendValue(std::move(multipleValue));

    QString out = exportModel(model);
    QString expected = getFileContents("expected_structures.sim");
    QCOMPARE(out, expected);
}

QString tst_SimulatorTrailExporter::exportModel(const SimulatorTrailModel &model)
{
    SimulatorTrailExporter exporter;
    conversion::Options options;
    const QString outputFilename = "generated_trail.sim";
    options.add(SimulatorTrailOptions::outputFilepath, outputFilename);

    exporter.exportModel(&model, options);

    return getFileContents(outputFilename);
}

QString tst_SimulatorTrailExporter::getFileContents(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}
}
QTEST_MAIN(simulatortrail::test::tst_SimulatorTrailExporter)

#include "tst_simulatortrailexporter.moc"
