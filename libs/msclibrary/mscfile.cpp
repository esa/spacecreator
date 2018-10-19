#include "mscfile.h"
#include "exceptions.h"
#include "mscmodel.h"

#include "mscparservisitor.h"
#include "parser/MscLexer.h"
#include "parser/MscParser.h"
#include "parser/MscBaseVisitor.h"

#ifdef SUPPRESS_ANTLR_WRNS_WINDOWS
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif // SUPPRESz_ANTLR_WRNS_WINDOWS

#include <antlr4-runtime.h>

#ifdef SUPPRESS_ANTLR_WRNS_WINDOWS
    #pragma warning(pop)
#endif // SUPPRESz_ANTLR_WRNS_WINDOWS

#include <QFileInfo>
#include <QObject>

#include <fstream>

/*!
  \class MscFile
  \inmodule MscLibrary

  The class to load MSC files
*/

using namespace antlr4;

namespace msc {
/*!
  \brief MscFile::MscFile
*/
MscFile::MscFile()
{
}

/*!
  \fn MscFile::parseFile(const QString &filename)

  Loads the file \a filename
*/
MscModel *MscFile::parseFile(const QString &filename)
{
    if (!QFileInfo::exists(filename)) {
        throw FileNotFoundException();
    }

    std::ifstream stream;
    stream.open(filename.toStdString());
    if (!stream) {
        throw IOException(QObject::tr("Error opening the file"));
    }

    ANTLRInputStream input(stream);
    return parse(input);
}

MscModel *MscFile::parseText(const QString &text)
{
    ANTLRInputStream input(text.toStdString());
    return parse(input);
}

MscModel *MscFile::parse(ANTLRInputStream &input)
{
    MscLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();

    MscParser parser(&tokens);

    MscParserVisitor visitor;
    visitor.visit(parser.file());

    if (lexer.getNumberOfSyntaxErrors() > 0) {
        throw ParserException(QObject::tr("Syntax error"));
    }
    if (parser.getNumberOfSyntaxErrors() > 0) {
        throw ParserException(QObject::tr("Syntax error"));
    }

    return visitor.detachModel();
    ;
}

} // namespace msc
