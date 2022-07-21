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
 * @brief Struct containing tested binary launching params.
 *
 */
struct LaunchConfiguration {
    /** path to the GDB script for launching the tested binary */
    QString scriptPath;
    /** debugger client to use for testing binary */
    QString clientName;
    /** arguments for debugger client */
    QString clientArgs;
    /** debugger client to use for testing binary */
    QString serverName;
    /** arguments for debugger server */
    QString serverArgs;
    /** arguments for debugger client converted to list with parsed variables */
    QStringList clientArgsParsed;
    /** arguments for debugger server converted to list with parsed variables */
    QStringList serverArgsParsed;

    /**
     * @brief Default constructor of the class
     *
     */
    LaunchConfiguration() = default;

    /**
     * @brief Constructor of the class
     *
     * @param launchScriptPath path to the GDB script for launching the tested binary
     * @param client name of the debugger client to use for testing binary
     * @param clientParams arguments for debugger client
     * @param server name of the debugger server to use for testing binary
     * @param serverParams arguments for debugger server
     *
     * @return true if there was no error during creation of the test harness files
     */
    LaunchConfiguration(const QString &launchScriptPath, const QString &client, QString clientParams,
            const QString &server, QString serverParams);
};

/**
 * @brief Class for storing launch parameters of every board
 *
 */
class LaunchConfigLoader
{
public:
    /**
     * @brief Constructor of the class
     *
     * @param launchConfigPath path to the file with launch configuration of each board
     *
     */
    LaunchConfigLoader(const QString &launchConfigPath);

    /**
     * @brief Getter for the configuration map of board names and their launch parameters
     *
     * @return map of board names as keys and launch parameters as values
     */
    auto getConfig() -> QMap<QString, LaunchConfiguration> &;

    /**
     * @brief Load launch parameters for every board and stores in configMap private field
     *
     * @return true if file with boards' launch configuration was successfuly opened
     */
    auto loadConfig() -> bool;

    /**
     * @brief Store launch parameters of every board in a CSV file specified in constructor
     *
     * @return true if saving configuration is successful, false otherwise
     */
    auto saveConfig(const QString &boardName, const LaunchConfiguration &launchConfig) -> bool;

private:
    QString configPath;
    QMap<QString, LaunchConfiguration> configMap;
};

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
     * @param boardName name of the board the tests are run on
     * @param launchConfig tested binary launching params
     *
     * @return true if there was no error during execution of the testing procedure nad false otherwise
     */
    auto testUsingDataFromCsv(IVInterface &interface, const CsvModel &csvModel, Asn1Model &asn1Model, const float delta,
            const QString &boardName, const LaunchConfiguration &launchConfig) -> bool;

    /**
     * @brief Prepare test harness files to be used for compilation of tests
     *
     * @param interface interface under test
     * @param csvModel the CSV data with test vectors
     * @param asn1Model ASN.1 model to be used during testing
     * @param boardName name of the board the tests are run on
     *
     * @return true if there was no error during creation of the test harness files
     */
    auto prepareTestHarness(IVInterface &interface, const CsvModel &csvModel, Asn1Model &asn1Model,
            const QString &boardName) -> QString;

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
