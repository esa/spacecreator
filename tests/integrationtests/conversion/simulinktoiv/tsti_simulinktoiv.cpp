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
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvXmlExporter/exporter.h>
#include <conversion/iv/SimulinkToIvTranslator/options.h>
#include <conversion/iv/SimulinkToIvTranslator/translator.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <libiveditor/iveditor.h>
#include <simulink/SimulinkOptions/options.h>
#include <simulink/SimulinkXmlImporter/importer.h>

using conversion::iv::IvOptions;
using conversion::iv::SimulinkToIvOptions;
using conversion::iv::exporter::IvXmlExporter;
using conversion::iv::translator::SimulinkToIvTranslator;
using conversion::simulink::SimulinkOptions;
using ivm::IVModel;
using simulink::importer::SimulinkXmlImporter;
using simulink::model::SimulinkModel;

namespace conversion::iv::test {

class tsti_SimulinkToIV : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testComparingIVTranslationResultWithExpectedResult();

private:
    static const std::array<const QString, 24> m_directoriesOfTests;

    static const QString m_interfaceXmlFileSubPath;

    static const QString m_expectedIVFileSubPath;
    static const QString m_currentIVFileName;
};

// clang-format off
const std::array<const QString, 24> tsti_SimulinkToIV::m_directoriesOfTests
{ 
    "resources/BasicTypes/InOutPortsDimension-1", 
    "resources/BasicTypes/InOutPortsDimension2",
    "resources/BasicTypes/InOutPortsDimension[2 2]",

    "resources/Aliases/InOutPortsDimension-1", 
    "resources/Aliases/InOutPortsDimension2",
    "resources/Aliases/InOutPortsDimension[2 2]",

    "resources/AliasesToAliases/InOutPortsDimension-1", 
    "resources/AliasesToAliases/InOutPortsDimension2",
    "resources/AliasesToAliases/InOutPortsDimension[2 2]",

    "resources/Enums/InOutPortsDimension-1", 
    "resources/Enums/InOutPortsDimension2",
    "resources/Enums/InOutPortsDimension[2 2]",

    "resources/BusMembersBasicTypes/InOutPortsDimension-1MemberDimension1", 
    "resources/BusMembersBasicTypes/InOutPortsDimension-1MemberDimension2",
    "resources/BusMembersBasicTypes/InOutPortsDimension-1MemberDimension[2 2]",

    "resources/BusMembersAliases/InOutPortsDimension-1MemberDimension1", 
    "resources/BusMembersAliases/InOutPortsDimension-1MemberDimension2",
    "resources/BusMembersAliases/InOutPortsDimension-1MemberDimension[2 2]",

    "resources/BusMembersEnums/InOutPortsDimension-1MemberDimension1", 
    "resources/BusMembersEnums/InOutPortsDimension-1MemberDimension2",
    "resources/BusMembersEnums/InOutPortsDimension-1MemberDimension[2 2]",

    "resources/BusMembersBasicTypesAliasesEnums/InOutPortsDimension-1MemberDimension1", 
    "resources/BusMembersBasicTypesAliasesEnums/InOutPortsDimension-1MemberDimension2",
    "resources/BusMembersBasicTypesAliasesEnums/InOutPortsDimension-1MemberDimension[2 2]",
};

const QString tsti_SimulinkToIV::m_interfaceXmlFileSubPath = "/TASTEExporterOutput/interface.xml";

const QString tsti_SimulinkToIV::m_expectedIVFileSubPath = "/iv/expected_interface_view.xml";
const QString tsti_SimulinkToIV::m_currentIVFileName = "interface_view.xml";

// clang-format on

QString getFileContents(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}

std::unique_ptr<conversion::Model> loadModel(const QString &interfaceFilePath)
{
    Options options;
    options.add(SimulinkOptions::inputFilepath, interfaceFilePath);

    SimulinkXmlImporter importer;
    auto importedModel = importer.importModel(options);

    return std::move(importedModel);
}

std::vector<std::unique_ptr<conversion::Model>> translateModel(SimulinkModel *simulinkModel)
{
    Options options;
    options.add(IvOptions::configFilepath, "config.xml");
    options.add(SimulinkToIvOptions::ivFunctionName, "simulink");
    options.add(SimulinkToIvOptions::ivFunctionImplementation, "QGenC");

    SimulinkToIvTranslator translator;

    auto translatedModels = translator.translateModels({ simulinkModel }, options);

    return std::move(translatedModels);
}

void exportModel(const IVModel *ivModel, const QString &outputIVFilePath)
{
    Options options;
    options.add(IvOptions::outputFilepath, outputIVFilePath);
    IvXmlExporter exporter;

    exporter.exportModel(ivModel, options);
}

void tsti_SimulinkToIV::initTestCase()
{
    ivm::initIVLibrary();
    ive::initIVEditor();
}

void tsti_SimulinkToIV::testComparingIVTranslationResultWithExpectedResult()
{
    for (const QString &m_directoryOfTest : m_directoriesOfTests) {
        try {
            const QString interfaceFilePath = m_directoryOfTest + m_interfaceXmlFileSubPath;

            const auto importedModel = loadModel(interfaceFilePath);
            auto *simulinkModel = dynamic_cast<SimulinkModel *>(importedModel.get());
            QVERIFY(simulinkModel);

            const auto translatedModels = translateModel(simulinkModel);
            QCOMPARE(translatedModels.size(), 1);

            const auto &translatedModel = translatedModels[0];
            const auto *ivModel = dynamic_cast<IVModel *>(translatedModel.get());
            QVERIFY(ivModel);

            exportModel(ivModel, m_currentIVFileName);

            const QString expectedIVFilePath = m_directoryOfTest + m_expectedIVFileSubPath;

            QVERIFY(getFileContents(m_currentIVFileName) == getFileContents(expectedIVFilePath));
        } catch (const std::exception &ex) {
            QFAIL(ex.what());
        }
    }
}

} // namespace conversion::iv::test

QTEST_MAIN(conversion::iv::test::tsti_SimulinkToIV)

#include "tsti_simulinktoiv.moc"
