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

#pragma once

#include "harness/dvgenerator/dvgenerator.h"
#include "harness/ivgenerator/ivgenerator.h"
#include "launchconfigloader/launchconfigloader.h"
#include "resultexporter/htmlresultexporter.h"
#include "testdrivergenerator/testdrivergenerator.h"
#include "testdrivergenerator/testdrivergeneratorexception.h"

using csv::CsvModel;
using dvm::DVModel;
using ivm::IVFunction;
using ivm::IVInterface;
using ivm::IVModel;

namespace testgenerator {

/**
 * @brief  Class for generating and executing interface tests. Backend for FunctionTesterPlugin.
 *
 */
class TestGenerator final
{
public:
    /**
     * @brief Constructor of the class
     *
     * @param baseDirectory directory where the TestGenerator looks for taste project files
     */
    TestGenerator(const QString &baseDirectory);

    /**
     * @brief Generate, compile and execute test of the specified interface
     *
     * @param interface interface to test
     * @param csvModel the CSV data with test vectors
     * @param asn1Model ASN.1 model to be used during testing
     * @param delta maximum allowed absolute error
     * @param launchConfig tested binary launching params
     *
     * @return true if there was no error during execution of the testing procedure nad false otherwise
     */
    auto testUsingDataFromCsv(IVInterface &interface, const CsvModel &csvModel, Asn1Model &asn1Model, const float delta,
            const LaunchConfiguration &launchConfig) -> bool;

    /**
     * @brief Prepare test harness files to be used for compilation of tests
     *
     * @param interface interface under test
     * @param csvModel the CSV data with test vectors
     * @param asn1Model ASN.1 model to be used during testing
     * @param boardName name of the board the tests are run on
     * @param stackSize size of the stack in the generated binary in kilobytes
     *
     * @return true if there was no error during creation of the test harness files
     */
    auto prepareTestHarness(IVInterface &interface, const CsvModel &csvModel, Asn1Model &asn1Model,
            const QString &boardName, const int stackSize) -> QString;

    /**
     * @brief Run the tests in GDB debugger
     *
     * @param interface interface under test
     * @param asn1Model ASN.1 model to be used during testing
     * @param launchConfig tested binary launching params
     *
     * @return vector of the test results obtained from GDB
     */
    auto runTests(IVInterface &interface, Asn1Model &asn1Model, const LaunchConfiguration &launchConfig)
            -> QVector<QVariant>;

private:
    auto initializePaths(const QString &baseDirectory) -> void;
    auto exportDvModel(DVModel *dvModel, const QString &outputFilename) -> bool;
    auto exportIvModel(IVModel *ivModel, const QString &outputFilename) -> bool;
    auto runProcess(QString cmd, QStringList args, QString workingPath) -> bool;
    auto prepareTasteProjectSkeleton() -> bool;
    auto copyFunctionImplementations(const QString &functionName) -> void;
    auto compileSystemUnderTest() -> void;
    auto getAllFunctionsFromModel(const IVModel &ivModel) -> std::vector<IVFunction *>;

    QString projectDirectory;
    QString generatedPath;
    QString generatedCodePath;
    QString generatedIvPath;
    QString generatedDvPath;
    QString binaryPath;
};

}
