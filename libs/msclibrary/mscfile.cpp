/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "mscfile.h"
#include "exceptions.h"
#include "mscmodel.h"
#include "mscparservisitor.h"

#include "parser/MscLexer.h"
#include "parser/MscParser.h"
#include "parser/MscBaseVisitor.h"

#include <antlr4-runtime.h>

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
