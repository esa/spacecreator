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

#include "htmlresultexporter.h"

#include <QDebug>
#include <QFile>
#include <ivfunctiontype.h>

namespace testgenerator {

HtmlResultExporter::HtmlResultExporter(const QString &chosenBoardName, const IVInterface &interface,
        const CsvModel &csvModel, const QVector<QVariant> &results, const float delta)
    : boardName(chosenBoardName)
    , interfaceName(interface.title())
    , functionName(interface.function()->title())
    , ifaceParams(interface.params())
    , rows(results.size() / ifaceParams.size())
    , maxDelta(delta)
{
    initTableCells(csvModel, results);
    initTableHeader();
}

auto HtmlResultExporter::initTableCells(const CsvModel &csvModel, const QVector<QVariant> &results) -> void
{
    QVector<QString>::size_type rowIndex = 0;
    QVector<QString>::size_type resultIndex = 0;
    if (results.empty()) {
        qWarning() << "Results vector is empty";
        return;
    }
    for (const auto &row : csvModel.records()) {
        csv::Row csvRow = *row;
        auto csvFields = csvRow.fields();
        cells.push_back({});
        QVector<QString>::size_type parameterIndex = 0;
        for (const auto &csvField : csvFields) {
            if (ifaceParams[parameterIndex].isInDirection()) {
                cells[rowIndex].push_back({ results[resultIndex], CellColor::Black });
            } else {
                auto currDelta = abs(results[resultIndex].toFloat() - csvField.toFloat());
                auto color = currDelta > maxDelta ? CellColor::Red : CellColor::Green;
                cells[rowIndex].push_back({ csvField, CellColor::Black });
                cells[rowIndex].push_back({ results[resultIndex], color });
                cells[rowIndex].push_back({ currDelta, color });
            }
            parameterIndex++;
            resultIndex++;
        }
        rowIndex++;
    }
}

auto HtmlResultExporter::initTableHeader() -> void
{
    for (int l = 0; l < ifaceParams.size(); l++) {
        columnNames.append(ifaceParams[l].name());
        if (ifaceParams[l].isInDirection()) {
            columnSizes.append(ColumnSize::Regular);
        } else {
            columnSizes.append(ColumnSize::Wide);
        }
    }
}

auto HtmlResultExporter::getData() -> CellTable
{
    return cells;
}

auto HtmlResultExporter::exportResult(const QString &filepath) -> void
{
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        generateHtmlStream(stream);
        file.close();
    }
}

auto HtmlResultExporter::generateHtmlStream(QTextStream &stream) -> void
{
    stream << "<!DOCTYPE html>\n";
    stream << "<html lang='en'>\n";
    stream << "\t<head>\n";
    stream << "\t\t<title>Test results for interface " << interfaceName << "</title>\n";
    stream << "\t\t<meta charset='utf-8'>\n";
    stream << "\t</head>\n";
    stream << "\t<style> ";
    stream << "table, th, td { ";
    stream << "border: 1px solid black; border-collapse: collapse; font-size: 22px; } ";
    stream << "th { ";
    stream << "font-size: 18px; } ";
    stream << "\t</style>\n";
    stream << "\t<body>\n";
    stream << "\t\t<h2>Test results for interface " << interfaceName << " of function " << functionName << "</h2>\n";
    stream << "\t\t<p style='font-size: 22px'>Chosen board: " << boardName << "</p>\n";
    stream << "\t\t<p style='font-size: 22px'>Maximum allowed absolute error: " << maxDelta << "</p>\n";
    stream << "\t\t<table>\n";

    generateTableHeader(stream);
    for (int rowIndex = 0; rowIndex < rows; rowIndex++) {
        if (cells.size() > rowIndex) {
            generateTableRow(stream, rowIndex);
        }
    }

    stream << "\t\t</table>\n";
    stream << "\t</body>\n";
    stream << "</html>\n";
}

auto HtmlResultExporter::generateTableRow(QTextStream &stream, int row) -> void
{
    stream << "\t\t\t<tr>\n";
    stream << "\t\t\t\t";
    for (int j = 0; j < cells[row].size(); j++) {
        if (cells[row][j].color == CellColor::Green) {
            stream << "<td style='color:green'>";
        } else if (cells[row][j].color == CellColor::Red) {
            stream << "<td style='color:red'>";
        } else {
            stream << "<td>";
        }
        stream << cells[row][j].value.toString();
        stream << "</td>";
    }
    stream << "\n";
    stream << "\t\t\t</tr>\n";
}

auto HtmlResultExporter::generateTableHeader(QTextStream &stream) -> void
{
    stream << "\t\t\t<tr>\n";
    stream << "\t\t\t\t";
    for (int i = 0; i < columnNames.size(); i++) {
        if (columnSizes[i] == ColumnSize::Regular) {
            stream << "<th rowspan='2'>";
            stream << columnNames[i];
            stream << "</th>";
        } else {
            stream << "<th colspan='3'>";
            stream << columnNames[i];
            stream << "</th>";
        }
    }
    stream << "\n";
    stream << "\t\t\t</tr>\n";

    stream << "\t\t\t<tr>\n";
    stream << "\t\t\t\t";
    for (int i = 0; i < columnNames.size(); i++) {
        if (columnSizes[i] == ColumnSize::Wide) {
            stream << "<th>";
            stream << "expected";
            stream << "</th>";

            stream << "<th>";
            stream << "actual";
            stream << "</th>";

            stream << "<th>";
            stream << "&Delta;";
            stream << "</th>";
        }
    }
    stream << "\n";
    stream << "\t\t\t</tr>\n";
}

}
