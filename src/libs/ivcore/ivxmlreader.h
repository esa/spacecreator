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
#include "xmlreader.h"

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
class IVArchetypeReference;
struct IVXMLReaderPrivate;
class IVXMLReader : public shared::XmlReader
{
    Q_OBJECT
public:
    explicit IVXMLReader(QObject *parent = nullptr);
    ~IVXMLReader() override;

    QVector<IVObject *> parsedObjects() const;
    QVector<IVObject *> parsedLayers() const;
    QString uiFileNameTag() const;
    QHash<shared::Id, EntityAttributes> externalAttributes() const;

protected:
    bool processTagOpen(QXmlStreamReader &xml) override;
    bool processTagClose(QXmlStreamReader &xml) override;
    QString rootElementName() const override;

private:
    const std::unique_ptr<IVXMLReaderPrivate> d;

    void processUIData();
    void processUIFile(const QString &file);
    IVFunctionType *addFunction(const shared::Id &id, IVObject::Type fnType);
    IVInterface *addIface(const shared::Id &id, bool isRI);
    IVComment *addComment(const shared::Id &id);
    IVConnection *addConnection(const shared::Id &id);
    IVConnectionGroup *addConnectionGroup(const shared::Id &id, const QString &groupName, const QString &source, const QString &target);
    IVArchetypeReference *addArchetypeReference(const QString &archetypeLibrary,
                                                const QString &archetypeFunction);

    bool isIdUnique(const QUuid &id) const;
    QString uniqueName(const QString &name) const;
};

}
