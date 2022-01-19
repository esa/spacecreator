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

#include "CsvModel/row.h"
#include "csv/CsvModel/csvmodel.h"
#include "importerexception.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <memory>
#include <utility>

namespace csv::importer {

auto CsvImporter::importModel(const Options &options) const -> std::unique_ptr<CsvModel>
{
    const QString fileToImport = options.value(CsvOptions::inputFilepath).value_or("");
    if (fileToImport.isEmpty()) {
        throw ImporterException("No name of file to import supplied");
    }

    QFile importedFile = QFile(fileToImport);
    if (!importedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw ImporterException("File to import cannot be opened.");
    }
    QTextStream importedFileTextStream(&importedFile);

    auto model = std::make_unique<CsvModel>();

    const QString separator = options.value(CsvOptions::separator).value_or(",");

    bool labelsImported = false;
    while (!importedFileTextStream.atEnd()) {
        const QStringList words = importedFileTextStream.readLine().split(separator);
        if (labelsImported) {
            if (!words.empty()) {
                model->addRecord(std::make_unique<Row>(words));
            }
        } else {
            if (!words.isEmpty() && !words.first().isEmpty()) {
                bool isNumber = false;
                std::for_each(words.begin(), words.end(), [&isNumber](const QString &word) {
                    bool conversionSuccessfull = false;
                    word.toDouble(&conversionSuccessfull);
                    if (conversionSuccessfull) {
                        isNumber = true;
                    }
                });
                if (!isNumber) {
                    model->setHeader(words);
                } else if (!words.empty()) {
                    model->addRecord(std::make_unique<Row>(words));
                }
                labelsImported = true;
            }
        }
    }

    return model;
}

} // namespace csv::importer
