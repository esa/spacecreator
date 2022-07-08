
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
    enum class CellColor
    {
        Red,
        Green,
        Black
    };
    struct Cell {
        QVariant value;
        CellColor color;
    };
    typedef QVector<QVector<Cell>> CellTable;

public:
    /**
     * @brief Constructor of the class
     *
     * @param interface tested interface
     * @param csvModel the CSV data with test vectors
     * @param results results obtained from GDB
     * @param delta maximum allowed absolute error
     */
    HtmlResultExporter(
            const IVInterface &interface, const CsvModel &csvModel, const QVector<QVariant> &results, float delta);

    /**
     * @brief exports test result to a HTML file
     *
     * @param filepath path to the html file to generate
     */
    auto exportResult(const QString &filepath) -> void;

private:
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
