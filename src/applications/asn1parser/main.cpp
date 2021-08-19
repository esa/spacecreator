#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <asn1library/asn1/file.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1reader.h>

using namespace Asn1Acn;

int main(int argc, char **argv)
{
    Asn1Reader reader;

    const auto filename = QString::fromUtf8(argv[1]);
    const auto file = QFileInfo(filename);

    if (file.exists()) {
        const auto result = reader.parseAsn1XmlFile(filename);

        const auto &integerType = dynamic_cast<Types::Integer *>(result->definitionsList()[0]->types()[0]->type());
        const auto &constraint = integerType->constraints().constraints()[0];
        qDebug() << "ok";
    } else {
        qDebug() << "doesnt exist";
    }

    return 0;
}
