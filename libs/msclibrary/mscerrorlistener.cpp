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

#include "mscerrorlistener.h"

namespace msc {

void MscErrorListener::syntaxError(antlr4::Recognizer * /*recognizer*/,
                                   antlr4::Token * /*offendingSymbol*/,
                                   size_t line,
                                   size_t charPositionInLine,
                                   const std::string &msg,
                                   std::exception_ptr e)
{
    m_errorMessages.append(QString("line %1:%2 %3").arg(line)
                                                   .arg(charPositionInLine)
                                                   .arg(QString::fromStdString(msg)));
}

QStringList MscErrorListener::getErrorMessages() const
{
    return m_errorMessages;
}

}
