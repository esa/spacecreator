#include "mscfile.h"
#include "mscparservisitor.h"

#include "parser/MscLexer.h"
#include "parser/MscParser.h"
#include "parser/MscBaseVisitor.h"

#include <antlr4-runtime.h>

#include <fstream>

using namespace antlr4;

MscFile::MscFile()
{
}

void MscFile::parseFile(const QString &filename)
{
    std::ifstream stream;
    stream.open(filename.toStdString());

    ANTLRInputStream input(stream);
    MscLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();

    MscParser parser(&tokens);

    MscParserVisitor visitor;
    visitor.visit(parser.r());
}
