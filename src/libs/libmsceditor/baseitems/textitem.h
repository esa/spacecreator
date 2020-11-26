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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "ui/textitem.h"

namespace msc {

class TextItem : public shared::ui::TextItem
{
public:
    explicit TextItem(QGraphicsItem *parent = nullptr);

protected:
    bool validateText(const QString &text) const override;
};

class NameItem : public TextItem
{
    // This could be done on the caller side, but currently it's the only used
    // concretization, so let's wrap it here to manage in a centralized manner.
    // And since it's just a single string customization I don't want to place
    // it into a separate file (at least, for now).

    Q_OBJECT
public:
    NameItem(QGraphicsItem *parent = nullptr)
        : TextItem(parent)
    {
        // see the MSC spec or Msc.g4
        static const QLatin1String LETTER("A-Za-z");
        static const QLatin1String DECIMALDIGIT("\\d");
        static const QLatin1String UNDERLINE("_");
        static const QLatin1String FULLSTOP(".");
        static const QLatin1String MINUS("\\-");
        static const QLatin1String BACKQUOTE("`");
        static const QLatin1String SLASH("/");

        static const QString pattern =
                // NAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | MINUS | '`' | '/' )+ ;
                "[" + LETTER + DECIMALDIGIT + UNDERLINE + FULLSTOP + MINUS + BACKQUOTE + SLASH + "]";
        setInputValidationPattern(pattern);
    }
};

}
