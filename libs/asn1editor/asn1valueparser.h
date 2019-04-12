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

#include <QObject>

namespace asn1 {

class Asn1ValueParser : public QObject
{
    Q_OBJECT
public:
    Asn1ValueParser(QObject *parent = nullptr);

    QVariantMap parseAsn1Value(const QVariantMap &asn1Type, const QString &asn1Value, bool *valueOk = nullptr) const;

Q_SIGNALS:
    void parseError(const QString &error) const;

private:
    bool checkFormat(const QString &asn1Value) const;

    bool parseSequenceValue(const QVariantMap &asn1Type, const QString &asn1Value, QVariantMap &valueMap) const;
    bool parseSequenceOfValue(const QVariantMap &asn1Type, const QString &asn1Value, QVariantMap &valueMap) const;
    bool parseChoiceValue(const QVariantMap &asn1Type, const QString &asn1Value, QVariantMap &valueMap) const;

    QVariantMap getType(const QString &name, const QVariantMap &asn1Type) const;
    bool checkRange(const QVariantMap &asn1Type, const QVariant &value) const;

    int nextIndex(const QString &value) const;
};

} // namespace asn1
