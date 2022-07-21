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

#include "csvimporter.h"

#include "csv/CsvModel/csvmodel.h"
#include "csv/CsvModel/row.h"
#include "importerexception.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <memory>
#include <shared/sharedlibrary.h>
#include <utility>

namespace csv::importer {

auto CsvImporter::importModel(const Options &options) const -> std::unique_ptr<CsvModel>
{
    const QString fileToImportName = options.value(CsvOptions::inputFilepath).value_or("");
    if (fileToImportName.isEmpty()) {
        throw ImporterException("No name of CSV file to import supplied");
    }

    QFile fileToImport = QFile(fileToImportName);
    if (!fileToImport.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw ImporterException("CSV file to import cannot be opened.");
    }

    const QString separator = options.value(CsvOptions::separator).value_or(",");

    QTextStream fileToImportTextStream(&fileToImport);

    auto model = std::make_unique<CsvModel>();

    const QStringList firstReadLine = fileToImportTextStream.readLine().split(separator);
    const int fieldsNumber = firstReadLine.size();

    if (isLineAHeader(firstReadLine)) {
        model->setHeader(firstReadLine);
    } else {
        addLineToModel(firstReadLine, model.get());
    }

    while (!fileToImportTextStream.atEnd()) {
        const QStringList readLine = fileToImportTextStream.readLine().split(separator);
        if (readLine.size() != fieldsNumber) {
            throw ImporterException("Invalid CSV file. Number of columns does not match");
        }
        addLineToModel(readLine, model.get());
    }

    return model;
}

bool CsvImporter::isLineAHeader(const QStringList &line) const
{
    for (const auto &word : line) {
        if (word.isEmpty()) {
            return false;
        }
    }

    bool areAllNumbers = true;
    std::for_each(line.begin(), line.end(), [&areAllNumbers](const QString &word) {
        bool conversionSuccessfull = false;
        word.toDouble(&conversionSuccessfull);
        if (!conversionSuccessfull) {
            areAllNumbers = false;
        }
    });

    return !areAllNumbers;
}

void CsvImporter::addLineToModel(const QStringList &line, CsvModel *const model) const
{
    if (model == nullptr) {
        throw ImporterException("Model is a nullptr");
    }

    if (line.isEmpty()) {
        return;
    }

    if (line.first().isEmpty()) {
        return;
    }

    model->addRecord(std::make_unique<Row>(line));
}

} // namespace csv::importer
