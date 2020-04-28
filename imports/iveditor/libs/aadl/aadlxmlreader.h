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

#include "aadlobject.h"

#include <QHash>
#include <QObject>
#include <QXmlStreamReader>
#include <memory>

class QIODevice;

namespace aadl {

class AADLObjectIface;
class AADLObjectFunctionType;
class AADLObjectComment;
class AADLObjectConnection;
struct AADLXMLReaderPrivate;
class AADLXMLReader : public QObject
{
    Q_OBJECT

public:
    explicit AADLXMLReader(QObject *parent = nullptr);
    ~AADLXMLReader() override;

    bool readFile(const QString &file);
    bool read(QIODevice *openForRead);

Q_SIGNALS:
    void objectsParsed(const QVector<AADLObject *> &objects);
    void error(const QString &msg) const;

private:
    const std::unique_ptr<AADLXMLReaderPrivate> d;

    bool readXml(QIODevice *in);

    bool readInterfaceView(QXmlStreamReader &xml);
    void processTagOpen(QXmlStreamReader &xml);
    void processTagClose(QXmlStreamReader &xml);

    AADLObjectFunctionType *addFunction(const QString &name, AADLObject::Type fnType);
    AADLObjectIface *addIface(const QString &name, bool isRI);
    AADLObjectComment *addComment(const QString &text);
    AADLObjectConnection *addConnection();
};

}
