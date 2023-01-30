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

#include "../../../unittests/common/xmldatahelper.h"

#include <QObject>
#include <QProcess>
#include <QtTest>
#include <conversion/converter/converter.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvRegistrar/registrar.h>
#include <conversion/iv/SimulinkToIvTranslator/options.h>
#include <conversion/registry/registry.h>
#include <conversion/simulink/SimulinkRegistrar/registrar.h>
#include <iostream>
#include <simulink/SimulinkOptions/options.h>

using conversion::Registry;
using conversion::iv::IvOptions;
using conversion::iv::IvRegistrar;
using conversion::iv::SimulinkToIvOptions;
using conversion::simulink::SimulinkOptions;
using conversion::simulink::SimulinkRegistrar;

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

    static inline const QString m_ignoredAttrs = "id,version";

private:
    Registry m_registry;
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

void tsti_SimulinkToIV::initTestCase()
{
    IvRegistrar ivRegistrar;
    auto result = ivRegistrar.registerCapabilities(m_registry);
    if (!result) {
        QFAIL("Failed to register IV capabilities");
    }

    SimulinkRegistrar simulinkRegistrar;
    result = simulinkRegistrar.registerCapabilities(m_registry);
    if (!result) {
        QFAIL("Failed to register SIMULINK capabilities");
    }
}

void tsti_SimulinkToIV::testComparingIVTranslationResultWithExpectedResult()
{
    for (const QString &m_directoryOfTest : m_directoriesOfTests) {
        try {
            const QString interfaceFilePath = m_directoryOfTest + m_interfaceXmlFileSubPath;
            const QString expectedIVFilePath = m_directoryOfTest + m_expectedIVFileSubPath;

            Options options;
            options.add(SimulinkOptions::inputFilepath, interfaceFilePath);
            options.add(IvOptions::configFilepath, "config.xml");
            options.add(IvOptions::outputFilepath, m_currentIVFileName);
            options.add(SimulinkToIvOptions::ivFunctionName, "simulink");
            options.add(SimulinkToIvOptions::ivFunctionImplementation, "QGenC");

            Converter converter(m_registry, std::move(options));
            converter.convert({ ModelType::Simulink }, ModelType::InterfaceView, {});

            QProcess diffProcess;

            QStringList args;
            args << "--check"
                 << "--ignored-attributes" << m_ignoredAttrs << m_currentIVFileName << expectedIVFilePath;

            diffProcess.start("xmldiff", args);
            diffProcess.waitForFinished();

            if (diffProcess.exitCode() != 0) {
                QString diffOutput(diffProcess.readAllStandardOutput());
                std::cerr << diffOutput.toStdString().c_str() << '\n';
                QFAIL(diffOutput.toStdString().c_str());
            }
        } catch (const std::exception &ex) {
            QFAIL(ex.what());
        }
    }
}

} // namespace conversion::iv::test

QTEST_MAIN(conversion::iv::test::tsti_SimulinkToIV)

#include "tsti_simulinktoiv.moc"
