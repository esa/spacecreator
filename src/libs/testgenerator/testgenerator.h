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
#include "testdrivergenerator/testdrivergenerator.h"
#include "testdrivergenerator/testdrivergeneratorexception.h"

namespace testgenerator {

struct TestResultModel
{
    enum class ColumnSize
    {
        Regular,
        Wide
    };
    enum class CellColor
    {
        Red,
        Green,
        Black
    };
    struct Cell
    {
        QVariant value;
        CellColor color;
    };
    typedef QVector<QVector<Cell>> CellTable;

    TestResultModel(const ivm::IVInterface &interface, const CsvModel &csvModel, const QVector<QVariant> &results, float delta);
    QString interfaceName;
    QString functionName;
    QVector<shared::InterfaceParameter> ifaceParams;
    CellTable cells;
    QVector<QString> columnNames;
    QVector<ColumnSize> columnSizes;
    int rows;
    float maxDelta;
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
     *
     * @return true if there was no error during execution of the testing procedure nad false otherwise
     */
    auto testUsingDataFromCsv(ivm::IVInterface &interface, const csv::CsvModel &csvModel,
            Asn1Acn::Asn1Model &asn1Model, float delta) -> bool;

    /**
     * @brief Prepare test harness files to be used for compilation of tests
     *
     * @param interface interface under test
     * @param csvModel the CSV data with test vectors
     * @param asn1Model ASN.1 model to be used during testing
     *
     * @return true if there was no error during creation of the test harness files
     */
    auto prepareTestHarness(ivm::IVInterface &interface,
            const csv::CsvModel &csvModel, Asn1Acn::Asn1Model &asn1Model) -> QString;
private:
    auto initializePaths(const QString &baseDirectory) -> void;
    auto exportDvModel(dvm::DVModel *dvModel, const QString &outputFilename) -> bool;
    auto exportIvModel(ivm::IVModel *ivModel, const QString &outputFilename) -> bool;
    auto runProcess(QString cmd, QStringList args, QString workingPath) -> bool;
    auto prepareTasteProjectSkeleton() -> bool;
    auto copyFunctionImplementations(const QString &functionName) -> void;
    auto compileSystemUnderTest() -> void;
    auto getAllFunctionsFromModel(const ivm::IVModel &ivModel) -> std::vector<ivm::IVFunction *>;
    auto extractResult(ivm::IVInterface &interface, Asn1Acn::Asn1Model &asn1Model) -> QVector<QVariant>;
    auto generateResultHtmlFile(const TestResultModel &resultData) -> void;
    auto generateResultHtmlStream(QTextStream &stream, const TestResultModel &resultData) -> void;
    auto generateTableRow(QTextStream &stream, const TestResultModel::CellTable &cells, int row) -> void;
    auto generateTableHeader(QTextStream &stream, const TestResultModel &resultsModel) -> void;

    QString projectDirectory;
    QString generatedPath;
    QString generatedCodePath;
    QString generatedIvPath;
    QString generatedDvPath;
};

}
