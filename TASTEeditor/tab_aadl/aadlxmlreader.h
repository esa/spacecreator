/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QHash>
#include <QObject>
#include <QXmlStreamReader>
#include <memory>

class QIODevice;

namespace taste3 {
namespace aadl {

class AADLObject;
class AADLObjectFunctionType;
class AADLObjectIface;
struct AADLXMLReaderPrivate;
class AADLXMLReader : public QObject
{
    Q_OBJECT

public:
    explicit AADLXMLReader(QObject *parent = nullptr);
    ~AADLXMLReader() override;

    bool parse(const QString &file);

Q_SIGNALS:
    void objectsParsed(const QVector<AADLObject *> &objects);
    void error(const QString &msg) const;

private:
    const std::unique_ptr<AADLXMLReaderPrivate> d;

    static QString badTagWarningMessage(const QXmlStreamReader &xml, const QString &tag);
    bool handleError(const QXmlStreamReader &xml);
    bool parse(QIODevice *in);

    bool readInterfaceView(QXmlStreamReader &xml);
    bool readAADLObject(QXmlStreamReader &xml);
    bool readFunction(QXmlStreamReader &xml, AADLObject *parent = nullptr);
    AADLObjectFunctionType *createFunction(QXmlStreamReader &xml, AADLObject *parent = nullptr);
    bool readFunctionProperty(QXmlStreamReader &xml, AADLObjectFunctionType *obj);
    AADLObjectIface *readInterface(QXmlStreamReader &xml, AADLObject *parent);
    bool readIfaceAttributes(QXmlStreamReader &xml, AADLObjectIface *iface);
    bool readIfaceProperties(QXmlStreamReader &xml, AADLObjectIface *iface);
    bool readIfaceProperty(QXmlStreamReader &xml, AADLObjectIface *iface);
    bool readIfaceParameter(QXmlStreamReader &xml, AADLObjectIface *iface);

    bool readConnection(QXmlStreamReader &xml);
};

} // ns aadl
} // ns taste3
