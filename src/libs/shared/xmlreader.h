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

#pragma once

#include "veobject.h"

#include <QObject>
#include <QXmlStreamReader>

namespace shared {
struct XMLReaderPrivate;

class XmlReader : public QObject
{
    Q_OBJECT
public:
    explicit XmlReader(QObject *parent = nullptr);
    ~XmlReader() override;

    bool readFile(const QString &file);
    bool read(QIODevice *openForRead);
    bool read(const QByteArray &data);

    QString errorString() const;
    QVariantMap metaData() const;

    const QString &file() const;

private:
    bool readView(QXmlStreamReader &xml);
    bool readXml(QIODevice *in);
    void setErrorString(const QString &string);

protected:
    void setMetaData(const QXmlStreamAttributes &attributes);

    virtual void processTagOpen(QXmlStreamReader &xml) = 0;
    virtual void processTagClose(QXmlStreamReader &xml) = 0;
    virtual QString rootElementName() const = 0;

private:
    const std::unique_ptr<XMLReaderPrivate> d;
};

} // namespace shared
