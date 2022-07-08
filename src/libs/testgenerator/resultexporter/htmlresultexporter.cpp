#include "htmlresultexporter.h"

#include <QFile>
#include <ivfunctiontype.h>

namespace testgenerator {

HtmlResultExporter::HtmlResultExporter(
        const IVInterface &interface, const CsvModel &csvModel, const QVector<QVariant> &results, float delta)
    : interfaceName(interface.title())
    , functionName(interface.function()->title())
    , ifaceParams(interface.params())
    , rows(results.size() / ifaceParams.size())
    , maxDelta(delta)
{
    std::vector<QString>::size_type i = 0;
    std::vector<QString>::size_type k = 0;
    for (const auto &row : csvModel.records()) {
        csv::Row csvRow = *row;
        auto csvFields = csvRow.fields();
        cells.push_back({});
        std::vector<QString>::size_type j = 0;
        for (const auto &csvField : csvFields) {
            if (ifaceParams[j].isInDirection()) {
                cells[i].push_back({ results[k], CellColor::Black });
            } else {
                auto currDelta = results[k].toFloat() - csvField.toFloat();
                auto color = abs(currDelta) > maxDelta ? CellColor::Red : CellColor::Green;
                cells[i].push_back({ csvField, CellColor::Black });
                cells[i].push_back({ results[k], color });
                cells[i].push_back({ currDelta, color });
            }
            j++;
            k++;
        }
        i++;
    }
    for (int l = 0; l < ifaceParams.size(); l++) {
        columnNames.append(ifaceParams[l].name());
        if (ifaceParams[l].isInDirection()) {
            columnSizes.append(ColumnSize::Regular);
        } else {
            columnSizes.append(ColumnSize::Wide);
        }
    }
}

auto HtmlResultExporter::exportResult(const QString &filepath) -> void
{
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        generateHtmlStream(stream);
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
    stream << "\t\t<p style='font-size: 22px'>Maximum allowed absolute error: " << maxDelta << "</p>\n";
    stream << "\t\t<table>\n";

    generateTableHeader(stream);
    for (int i = 0; i < rows; i++) {
        generateTableRow(stream, i);
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
