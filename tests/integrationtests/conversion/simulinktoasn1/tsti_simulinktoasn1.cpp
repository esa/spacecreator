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
#include <conversion/asn1/Asn1Exporter/exporter.h>
#include <conversion/asn1/SimulinkToAsn1Translator/translator.h>
#include <simulink/SimulinkOptions/options.h>
#include <simulink/SimulinkXmlImporter/importer.h>
#include <array>

using Asn1Acn::Asn1Model;
using conversion::asn1::exporter::Asn1Exporter;
using conversion::asn1::translator::SimulinkToAsn1Translator;
using conversion::simulink::SimulinkOptions;
using simulink::importer::SimulinkXmlImporter;
using simulink::model::SimulinkModel;

namespace conversion::asn1::test {

class tsti_SimulinkToAsn1 : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testComparingAsn1TranslationResultWithExpectedResult();

private:
    static const std::array<const QString, 24> m_directoriesOfTests;

    static const QString m_interfaceXmlFileSubPath;

    static const QString m_expectedInterfaceFileSubPath;
    static const QString m_expectedMatLabStandardDataTypesFileSubPath;

    static const QString m_currentInterfaceFileName;
    static const QString m_currentMatLabStandardDataTypesFileName;
};

// clang-format off
const std::array<const QString, 24> tsti_SimulinkToAsn1::m_directoriesOfTests
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

const QString tsti_SimulinkToAsn1::m_interfaceXmlFileSubPath = "/TASTEExporterOutput/interface.xml";

const QString tsti_SimulinkToAsn1::m_expectedInterfaceFileSubPath = "/asn1/expected_INTERFACE";
const QString tsti_SimulinkToAsn1::m_expectedMatLabStandardDataTypesFileSubPath = "/asn1/expected_MATLAB-STANDARD-DATATYPES";

const QString tsti_SimulinkToAsn1::m_currentInterfaceFileName = "INTERFACE";
const QString tsti_SimulinkToAsn1::m_currentMatLabStandardDataTypesFileName = "MATLAB-STANDARD-DATATYPES";

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
    SimulinkToAsn1Translator translator;

    auto translatedModels = translator.translateModels({ simulinkModel }, options);

    return std::move(translatedModels);
}

void exportModel(const Asn1Model *asn1Model)
{
    Options options;
    Asn1Exporter exporter;

    exporter.exportModel(asn1Model, options);
}

void tsti_SimulinkToAsn1::testComparingAsn1TranslationResultWithExpectedResult()
{
    for (const QString &m_directoryOfTest : m_directoriesOfTests) {
        try {
            QString interfaceFilePath = m_directoryOfTest + m_interfaceXmlFileSubPath;

            const auto importedModel = loadModel(interfaceFilePath);
            auto *simulinkModel = dynamic_cast<SimulinkModel *>(importedModel.get());
            QVERIFY(simulinkModel);

            const auto translatedModels = translateModel(simulinkModel);
            QCOMPARE(translatedModels.size(), 1);

            const auto &translatedModel = translatedModels[0];
            const auto *asn1Model = dynamic_cast<Asn1Model *>(translatedModel.get());
            QVERIFY(asn1Model);

            exportModel(asn1Model);

            const QString expectedInterfaceAsnFilePath = m_directoryOfTest + m_expectedInterfaceFileSubPath + ".asn";
            const QString expectedInterfaceAcnFilePath = m_directoryOfTest + m_expectedInterfaceFileSubPath + ".acn";

            const QString expectedMatLabStandardDataTypesAsnFilePath =
                    m_directoryOfTest + m_expectedMatLabStandardDataTypesFileSubPath + ".asn";
            const QString expectedMatLabStandardDataTypesAcnFilePath =
                    m_directoryOfTest + m_expectedMatLabStandardDataTypesFileSubPath + ".acn";

            QVERIFY(getFileContents(m_currentInterfaceFileName + ".asn")
                    == getFileContents(expectedInterfaceAsnFilePath));
            QVERIFY(getFileContents(m_currentInterfaceFileName + ".acn")
                    == getFileContents(expectedInterfaceAcnFilePath));

            QVERIFY(getFileContents(m_currentMatLabStandardDataTypesFileName + ".asn")
                    == getFileContents(expectedMatLabStandardDataTypesAsnFilePath));
            QVERIFY(getFileContents(m_currentMatLabStandardDataTypesFileName + ".acn")
                    == getFileContents(expectedMatLabStandardDataTypesAcnFilePath));
        } catch (const std::exception &ex) {
            QFAIL(ex.what());
        }
    }
}

} // namespace conversion::asn1::test

QTEST_MAIN(conversion::asn1::test::tsti_SimulinkToAsn1)

#include "tsti_simulinktoasn1.moc"
