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

#include "ivobject.h"

#include <QObject>
#include <QVariantMap>
#include <QVector>
#include <memory>

class QIODevice;
class QXmlStreamReader;

namespace ivm {

class IVInterface;
class IVFunctionType;
class IVComment;
class IVConnection;
class IVConnectionGroup;
struct IVXMLReaderPrivate;
class IVXMLReader : public QObject
{
    Q_OBJECT

public:
    explicit IVXMLReader(QObject *parent = nullptr);
    ~IVXMLReader() override;

    bool readFile(const QString &file);
    bool read(QIODevice *openForRead);
    bool read(const QByteArray &data);

Q_SIGNALS:
    void objectsParsed(const QVector<ivm::IVObject *> &objects);
    void metaDataParsed(const QVariantMap &metadata);
    void error(const QString &msg);

private:
    const std::unique_ptr<IVXMLReaderPrivate> d;

    bool readXml(QIODevice *in);

    bool readInterfaceView(QXmlStreamReader &xml);
    void processTagOpen(QXmlStreamReader &xml);
    void processTagClose(QXmlStreamReader &xml);

    IVFunctionType *addFunction(const QString &name, IVObject::Type fnType);
    IVInterface *addIface(const QString &name, bool isRI);
    IVComment *addComment(const QString &text);
    IVConnection *addConnection();
    IVConnectionGroup *addConnectionGroup(const QString &groupName);
};

}
