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

#include "msgidentificationitem.h"

namespace msc {

MsgIdentificationItem::MsgIdentificationItem(QGraphicsItem *parent)
    : NameItem(parent)
{
}

QString MsgIdentificationItem::name() const
{
    return nameFromText(toPlainText());
}

MscParameterList MsgIdentificationItem::parameters() const
{
    return parametersFromText(toPlainText());
}

QString MsgIdentificationItem::nameFromText(const QString &text)
{
    return text.section(QLatin1Char('('), 0, 0).trimmed();
}

MscParameterList MsgIdentificationItem::parametersFromText(const QString &text)
{
    msc::MscParameterList parameters;
    const int idx = text.indexOf('(');
    if (idx > 0) {

        const int paramEnd = text.lastIndexOf(')');
        if (paramEnd > 0) {
            const bool hasExtraOpenBrace = text.at(idx + 1) == MscParameterList::DefaultExtraBraceOpen;
            const bool hasExtraCloseBrace = text.at(paramEnd - 1) == MscParameterList::DefaultExtraBraceClose;
            const int shiftOpen = hasExtraOpenBrace ? 2 : 1;
            const int shiftClose = hasExtraCloseBrace ? 3 : 1;
            QStringRef paramsText = text.midRef(idx + shiftOpen, paramEnd - idx - shiftClose);
            for (auto param : paramsText.split(',')) {
                param = param.trimmed();
                if (!param.isEmpty()) {
                    MscParameter mscParam;
                    if (param.contains(":")) {
                        mscParam.setExpression(param.toString());
                    } else {
                        mscParam.setPattern(param.toString());
                    }
                    parameters.dataRef().append(mscParam);
                }
            }
            if (hasExtraOpenBrace || hasExtraCloseBrace)
                parameters.setExtraBraces(MscParameterList::DefaultExtraBraceOpen,
                                          MscParameterList::DefaultExtraBraceClose);
        }
    }

    return parameters;
}

bool MsgIdentificationItem::validateInput(const QString &text) const
{
    Q_UNUSED(text);
    return true;
}

} // ns msc
