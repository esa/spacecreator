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

#include "CsvModel/csvmodel.h"
#include "CsvModel/row.h"
#include "CsvOptions/options.h"
#include "csv/CsvModel/csvmodel.h"
#include "import/exceptions.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <memory>
#include <utility>

using csv::CsvOptions;

namespace csv::importer {

auto CsvImporter::importModel(const conversion::Options &options) const -> std::unique_ptr<conversion::Model>
{
    const QString fileToImport = options.value(CsvOptions::inputFilepath).value_or("");
    if (fileToImport.isEmpty()) {
        throw conversion::importer::ImportException("No name of file to import supplied");
    }

    QFile importedFile = QFile(fileToImport);
    qDebug() << "cur path: " << QDir::currentPath();
    if (!importedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw conversion::importer::ImportException("File to import cannot be opened.");
    }
    QTextStream importedFileTextStream(&importedFile);

    auto model = std::make_unique<CsvModel>();

    model->setSeparator(options.value(CsvOptions::separator).value_or(","));
    model->setHeader(importedFileTextStream.readLine().split(model->separator()));
    while (!importedFileTextStream.atEnd()) {
        // TODO: import records
    }

    return std::move(model);
}

} // namespace csv::importer
