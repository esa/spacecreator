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

#pragma once

#include "BaseErrorListener.h"

#include <QStringList>

namespace msc {

class MscErrorListener : public antlr4::BaseErrorListener
{
public:
    MscErrorListener(bool notifyErrorHub);

    QStringList getErrorMessages() const;

private:
    virtual void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line,
            size_t charPositionInLine, const std::string &msg, std::exception_ptr e) override;

private:
    QStringList m_errorMessages;
    bool m_notifyErrorHub = false;
};

} // namespace msc
