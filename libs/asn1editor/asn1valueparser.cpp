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

#include <QVariant>

#include "asn1valueparser.h"

Asn1ValueParser::Asn1ValueParser(QObject *parent)
    : QObject(parent)
{
}

QVariantMap Asn1ValueParser::parseAsn1Value(const QVariantMap &asn1Type, const QString &asn1Value)
{
    QVariantMap valueMap;
    bool ok = true;

    if (asn1Type.empty() || asn1Value.isEmpty())
        return valueMap;

    QString value = asn1Value.trimmed();
    QString valueType = asn1Type["type"].toString();

    valueMap["name"] = asn1Type["name"];

    if (valueType == "integer") {
        int intVal = value.toInt(&ok);

        if (ok && (ok = checkRange(asn1Type, intVal)))
            valueMap["value"] = intVal;
    } else if (valueType == "double") {
        double doubleVal = value.toDouble(&ok);

        if (ok && (ok = checkRange(asn1Type, doubleVal)))
            valueMap["value"] = doubleVal;
    } else if (valueType == "bool") {
        if ((ok = value == "TRUE" || value == "FALSE"))
            valueMap["value"] = value == "TRUE" ? true : false;
    } else if (valueType == "sequence") {
        parseSequenceValue(asn1Type, value, valueMap);
    } else if (valueType == "sequenceOf") {
        parseSequenceOfValue(asn1Type, value, valueMap);
        ok = checkRange(asn1Type, valueMap["seqofvalue"].toList().count());
    } else if (valueType == "enumerated") {
        QVariantList values = asn1Type["values"].toList();

        // check enumerated value
        if ((ok = values.contains(value)))
            valueMap["value"] = value;
    } else if (valueType == "choice") {
        parseChoiceValue(asn1Type, value, valueMap);
    } else {
        // take string between " "
        if (value.startsWith("\""))
            value = value.remove(0, 1);

        if (value.endsWith("\""))
            value.chop(1);

        if ((ok = checkRange(asn1Type, value.length())))
            valueMap["value"] = value;
    }

    if (!ok) {
        valueMap.clear();
        Q_EMIT parseError(QString(tr("Incorrect value for %1")).arg(asn1Type["name"].toString()));
    }

    return valueMap;
}

bool Asn1ValueParser::checkFormat(const QString &asn1Value)
{
    return asn1Value.startsWith("{") && asn1Value.endsWith("}");
}

void Asn1ValueParser::parseSequenceValue(const QVariantMap &asn1Type,
                                         const QString &asn1Value,
                                         QVariantMap &valueMap)
{
    /*
{ intVal 5,
  realVal 42.0,
  intArray {},
  boolVal TRUE,
  enumVal enum2,
  enumArray {enum1, enum3},
  label "Hello World",
  choiceVal choiceReal : 10.5,
  seqVal { iVal 5 }
}
*/
    QVariantList children;
    QString value = asn1Value;

    if (!checkFormat(value))
        Q_EMIT parseError(tr("Incorrect format for ASN.1 sequence value"));

    // delete '{' from begin and '}' from end
    value.remove(0, 1);
    value.chop(1);

    QStringList sequenceField = value.split(",");
    for (const auto &field : sequenceField) {
        QString name = field.left(field.indexOf(" ")).trimmed();
        QString value = field.mid(field.indexOf(" ") + 1).trimmed();

        children.append(parseAsn1Value(findType(name, asn1Type), value));
    }

    valueMap["children"] = children;
}

void Asn1ValueParser::parseSequenceOfValue(const QVariantMap &asn1Type,
                                           const QString &asn1Value,
                                           QVariantMap &valueMap)
{
    /*
{enum1, enum3}
{ { intVal 5, realVal 42.0 }, { intVal 6, realVal 13.9 } )

*/
    QVariantList seqofValues;
    QString value = asn1Value;

    if (!checkFormat(value))
        Q_EMIT parseError(tr("Incorrect format for ASN.1 sequence value"));

    // delete '{' from begin and '}' from end
    value.remove(0, 1);
    value.chop(1);

    QStringList values = value.split(",");
    for (const auto &item : values) {
        seqofValues.append(parseAsn1Value(asn1Type["seqoftype"].toMap(), item));
    }

    valueMap["seqofvalue"] = seqofValues;
}

void Asn1ValueParser::parseChoiceValue(const QVariantMap &asn1Type,
                                       const QString &asn1Value,
                                       QVariantMap &valueMap)
{
    /*
 choiceReal : 10.5
 */
    QString name = asn1Value.left(asn1Value.indexOf(":")).trimmed();
    QString value = asn1Value.mid(asn1Value.indexOf(":") + 1).trimmed();

    valueMap["choice"] = parseAsn1Value(findType(name, asn1Type), value);
}

QVariantMap Asn1ValueParser::findType(const QString &name, const QVariantMap &asn1Type)
{
    QVariantMap result;

    if (asn1Type["name"] == name)
        return asn1Type;

    if (asn1Type.contains("children")) {
        for (const QVariant &child : asn1Type["children"].toList()) {
            result = findType(name, child.toMap());
            if (result.size())
                break;
        }
    }

    if (asn1Type.contains("choices")) {
        for (const QVariant &choice : asn1Type["choices"].toList()) {
            result = findType(name, choice.toMap());
            if (result.size())
                break;
        }
    }

    return result;
}

bool Asn1ValueParser::checkRange(const QVariantMap &asn1Type, const QVariant &value)
{
    if (asn1Type.contains("min") && value < asn1Type["min"])
        return false;
    else if (asn1Type.contains("max") && value > asn1Type["max"])
        return false;

    return true;
}
