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

#include <QFileInfo>
#include <QObject>
//#include <antlr4-runtime.h>
#include <fstream>

#include "asn1file.h"

#include "asn1itemmodel.h"

#include "parser/ASNLexer.h"
#include "parser/ASNBaseVisitor.h"
#include "parser/ASNParser.h"

using namespace antlr4;

namespace msc {

Asn1File::Asn1File() {}

Asn1ItemModel *Asn1File::parseFile(const QString &filename, QStringList *errorMessages)
{
    if (!QFileInfo::exists(filename)) {
        throw QObject::tr("File not found");
    }

    std::ifstream stream;
    stream.open(filename.toStdString());
    if (!stream) {
        throw QObject::tr("Error opening the file");
    }

    ANTLRInputStream input(stream);
    return parse(input, errorMessages);
}

Asn1ItemModel *Asn1File::parseText(const QString &text, QStringList *errorMessages)
{
    ANTLRInputStream input(text.toStdString());
    return parse(input, errorMessages);
}

Asn1ItemModel *Asn1File::parse(ANTLRInputStream &input, QStringList *errorMessages)
{
    //    MscErrorListener errorListener;

    ASNLexer lexer(&input);
    //    lexer.addErrorListener(&errorListener);

    CommonTokenStream tokens(dynamic_cast<TokenSource*>(&lexer));
    tokens.fill();

//    ASNParser parser(&tokens);
    //    parser.addErrorListener(&errorListener);

    //    MscParserVisitor visitor(&tokens);
    //    visitor.visit(parser.file());

    //    if (errorMessages != nullptr) {
    //        *errorMessages = errorListener.getErrorMessages();
    //    }

    //    if (lexer.getNumberOfSyntaxErrors() > 0) {
    //        throw QObject::tr("Lexer syntax error");
    //    }
//    if (parser.getNumberOfSyntaxErrors() > 0) {
//        throw QObject::tr("Parser syntax error");
//    }

    return nullptr; // visitor.detachModel();
}

} // namespace msc
