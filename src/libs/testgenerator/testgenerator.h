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
    int rows;
    float maxDelta;
};

class TestGenerator final
{
public:
    TestGenerator(const QString &baseDirectory);
    auto testUsingDataFromCsv(ivm::IVInterface &interface, const csv::CsvModel &csvModel,
            Asn1Acn::Asn1Model &asn1Model, float delta) -> void;
    auto prepareTestHarness(ivm::IVInterface &interface,
            const csv::CsvModel &csvModel, Asn1Acn::Asn1Model &asn1Model) -> QString;
private:
    auto initializePaths(const QString &baseDirectory) -> void;
    auto exportDvModel(dvm::DVModel *dvModel, const QString &outputFilename) -> void;
    auto exportIvModel(ivm::IVModel *ivModel, const QString &outputFilename) -> void;
    auto getDvObjectsFromModel(dvm::DVModel *const model) -> std::unique_ptr<QVector<dvm::DVObject *>>;
    auto runProcess(QString cmd, QStringList args, QString workingPath) -> void;
    auto prepareTasteProjectSkeleton() -> void;
    auto copyFunctionImplementations(const QString &functionName) -> void;
    auto compileSystemUnderTest() -> void;
    auto getAllFunctionsFromModel(const ivm::IVModel &ivModel) -> std::vector<ivm::IVFunction *>;
    auto extractResult(ivm::IVInterface &interface, Asn1Acn::Asn1Model &asn1Model) -> QVector<QVariant>;
    auto generateResultHtmlFile(const TestResultModel &resultData) -> void;
    auto generateResultHtmlStream(QTextStream &stream, const TestResultModel &resultData) -> void;
    auto generateTableRow(QTextStream &stream, const TestResultModel::CellTable &cells, int row) -> void;
    
    QString projectDirectory;
    QString generatedPath;
    QString generatedCodePath;
    QString generatedIvPath;
    QString generatedDvPath;
};

}
