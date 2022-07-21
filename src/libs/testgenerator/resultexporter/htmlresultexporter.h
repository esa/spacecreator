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

#include <QTextStream>
#include <csv/CsvModel/csvmodel.h>
#include <ivcore/ivinterface.h>

using csv::CsvModel;
using ivm::IVInterface;

namespace testgenerator {

/**
 * @brief  Provides methods for exporting the results of interface tests into a HTML file
 *
 */
class HtmlResultExporter
{
    enum class ColumnSize
    {
        Regular,
        Wide
    };

public:
    /**
     * @brief Colors of the cells in result table
     *
     */
    enum class CellColor
    {
        Red,
        Green,
        Black
    };

    /**
     * @brief Description of each cell in result table
     *
     */
    struct Cell {
        /** value displayed in the cell */
        QVariant value;
        /** color of the displayed text */
        CellColor color;
        /** operator for cells comparison */
        friend bool operator==(const Cell &l, const Cell &r) { return l.value == r.value && l.color == r.color; }
    };

    /** Type for the two dimentional vector containing cells of the result table */
    typedef QVector<QVector<Cell>> CellTable;

    /**
     * @brief Constructor of the class
     *
     * @param interface tested interface
     * @param csvModel the CSV data with test vectors
     * @param results results obtained from GDB
     * @param delta maximum allowed absolute error
     */
    HtmlResultExporter(const IVInterface &interface, const CsvModel &csvModel, const QVector<QVariant> &results,
            const float delta);

    /**
     * @brief Exports test result to a HTML file
     *
     * @param filepath path to the html file to generate
     */
    auto exportResult(const QString &filepath) -> void;

    /**
     * @brief Getter for an array with cell values of the result table
     *
     * @return an array with cell values of the result table
     */
    auto getData() -> CellTable;

private:
    auto initTableHeader() -> void;
    auto initTableCells(const CsvModel &csvModel, const QVector<QVariant> &results) -> void;
    auto generateHtmlStream(QTextStream &stream) -> void;
    auto generateTableRow(QTextStream &stream, int row) -> void;
    auto generateTableHeader(QTextStream &stream) -> void;

    QString interfaceName;
    QString functionName;
    QVector<shared::InterfaceParameter> ifaceParams;
    CellTable cells;
    QVector<QString> columnNames;
    QVector<ColumnSize> columnSizes;
    int rows;
    float maxDelta;
};

}
