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

#include <QFile>
#include <QXmlStreamAttribute>
#include <QtDebug>

namespace shared {

struct XMLReaderPrivate {
    QVariantMap m_metaData;
    QString m_errorString;
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

bool XmlReader::readFile(const QString &file)
{
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

    return false;
}

bool XmlReader::read(const QByteArray &data)
{
    if (data.isEmpty()) {
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

bool XmlReader::readXml(QIODevice *in)
{
    if (!in)
        return false;

    QXmlStreamReader xml(in);
    if (xml.readNext() == QXmlStreamReader::StartDocument)
        if (xml.readNext() == QXmlStreamReader::StartElement)
            if (xml.name().toString() == rootElementName())
                return readView(xml);

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
        return false;
    }

    return true;
}

} // namespace shared
