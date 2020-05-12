/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscerrorlistener.h"

#include <Lexer.h>
#include <Parser.h>
#include <Token.h>

namespace msc {

void MscErrorListener::syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line,
        size_t charPositionInLine, const std::string &msg, std::exception_ptr /*e*/)
{
    const QString lineOfError = QString::number(line);
    const QString positionInLine = QString::number(charPositionInLine);
    const QString errorMessage = QString::fromStdString(msg);

    antlr4::Lexer *lexer = dynamic_cast<antlr4::Lexer *>(recognizer);
    if (lexer) {
        m_errorMessages.append(QString("line %1:%2: <b>%3</b><br>").arg(lineOfError, positionInLine, errorMessage));
        return;
    }

    QString stack;
    antlr4::Parser *parser = dynamic_cast<antlr4::Parser *>(recognizer);
    if (parser) {
        QStringList strList;
        for (const std::string &str : parser->getRuleInvocationStack())
            strList.prepend(QString::fromStdString(str));
        stack = stack.append(strList.join("->"));
    }

    if (offendingSymbol) {
        m_errorMessages.append(
                QString("@%1:%2: <b>'%3' - %4</b>; Rules stack:<br>[%5]<br>")
                        .arg(lineOfError, positionInLine, QString::fromStdString(offendingSymbol->getText()),
                                errorMessage, stack));
    } else {
        m_errorMessages.append(QString("@%1:%2: <b>%3</b>; Rules stack:<br>[%4]<br>")
                                       .arg(lineOfError, positionInLine, errorMessage, stack));
    }
}

QStringList MscErrorListener::getErrorMessages() const
{
    return m_errorMessages;
}
}
