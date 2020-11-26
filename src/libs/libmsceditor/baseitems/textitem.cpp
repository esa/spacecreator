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

#include "textitem.h"

#include "mscreader.h"

namespace msc {

TextItem::TextItem(QGraphicsItem *parent)
    : shared::ui::TextItem(parent)
{
}

bool TextItem::validateText(const QString &text) const
{
    if (m_mscValidationTest.isEmpty()) {
        return true;
    }

    MscReader reader;
    QStringList errors;
    QString testDoc = m_mscValidationTest.arg(text);
    try {
        reader.parseText(testDoc, &errors);
    } catch (...) {
        return false;
    }
    return errors.isEmpty();
}

}
