/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "xmlreader.h"

#include "entityattribute.h"
#include "errorhub.h"

#include <QFile>
#include <QXmlStreamAttribute>
#include <QtDebug>

namespace shared {

struct XMLReaderPrivate {
    QVariantMap m_metaData;
    QString m_errorString;
    QString m_file;
};

XmlReader::XmlReader(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<XMLReaderPrivate>())
{
}

XmlReader::~XmlReader() { }

void XmlReader::setMetaData(const QXmlStreamAttributes &attributes)
{
    for (const QXmlStreamAttribute &attribute : attributes) {
        d->m_metaData[attribute.name().toString()] = QVariant::fromValue(attribute.value().toString());
    }
}

InterfaceParameter XmlReader::addIfaceParameter(
        const EntityAttributes &otherAttrs, InterfaceParameter::Direction direction)
{
    shared::InterfaceParameter param;
    for (const EntityAttribute &attr : otherAttrs) {
        if (attr.name() == "name") {
            param.setName(attr.value<QString>());
        } else if (attr.name() == "type") {
            param.setParamTypeName(attr.value<QString>());
        } else if (attr.name() == "encoding") {
            param.setEncoding(attr.value<QString>());
        } else {
            shared::ErrorHub::addError(shared::ErrorItem::Warning,
                    QObject::tr("Interface Parameter - unknown attribute: %1").arg(attr.name()));
        }
    }

    param.setDirection(direction);
    return param;
}

EntityAttributes XmlReader::attributes(const QXmlStreamAttributes &xmlAttrs)
{
    EntityAttributes attrs;
    for (const QXmlStreamAttribute &xmlAttr : xmlAttrs) {
        const QString &name = xmlAttr.name().toString();
        attrs.insert(name, EntityAttribute(name, xmlAttr.value().toString(), EntityAttribute::Type::Attribute));
    }
    return attrs;
}

bool XmlReader::readFile(const QString &file)
{
    d->m_file = file;
    QFile in(file);
    if (in.exists(file) && in.open(QFile::ReadOnly | QFile::Text))
        return read(&in);

    setErrorString(QString("Can't open file %1: %2").arg(file, in.errorString()));
    return false;
}

bool XmlReader::read(QIODevice *openForRead)
{
    if (openForRead && openForRead->isOpen() && openForRead->isReadable()) {
        return readXml(openForRead);
    }

    setErrorString(tr("Can't read device"));
    return false;
}

bool XmlReader::read(const QByteArray &data)
{
    if (data.isEmpty()) {
        setErrorString(tr("Can't read data"));
        return false;
    }

    QXmlStreamReader xml(data);
    if (xml.readNext() == QXmlStreamReader::StartDocument) {
        if (xml.readNext() == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == rootElementName()) {
                return readView(xml);
            }
        }
    }
    setErrorString(tr("Malformed XML"));
    return false;
}

QString XmlReader::errorString() const
{
    return d->m_errorString;
}

QVariantMap XmlReader::metaData() const
{
    return d->m_metaData;
}

/*!
   Returns the file path of the file being parsed
 */
const QString &XmlReader::file() const
{
    return d->m_file;
}

bool XmlReader::readXml(QIODevice *in)
{
    if (!in) {
        setErrorString(tr("Can't read device"));
        return false;
    }

    QXmlStreamReader xml(in);
    if (xml.readNext() == QXmlStreamReader::StartDocument)
        if (xml.readNext() == QXmlStreamReader::StartElement)
            if (xml.name().toString() == rootElementName())
                return readView(xml);

    setErrorString(tr("Error parsing XML"));
    return false;
}

void XmlReader::setErrorString(const QString &string)
{
    qWarning() << string;
    d->m_errorString = string;
}

bool XmlReader::readView(QXmlStreamReader &xml)
{
    setMetaData(xml.attributes());

    while (!xml.atEnd()) {
        if (xml.hasError()) {
            setErrorString(xml.errorString());
            qDebug() << Q_FUNC_INFO << 1 << xml.errorString();
            return false;
        }

        switch (xml.readNext()) {
        case QXmlStreamReader::TokenType::StartElement:
            processTagOpen(xml);
            break;
        case QXmlStreamReader::TokenType::EndElement:
            processTagClose(xml);
            break;
        default:
            break;
        }
    }

    if (xml.hasError()) {
        setErrorString(xml.errorString());
        qDebug() << Q_FUNC_INFO << 2 << xml.errorString();
        return false;
    }

    return true;
}

} // namespace shared
