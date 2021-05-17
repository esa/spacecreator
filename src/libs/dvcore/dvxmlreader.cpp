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

#include "dvxmlreader.h"

#include "dvcommonprops.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvnode.h"
#include "dvobject.h"
#include "dvpartition.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QVariant>
#include <QVector>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>

namespace dvm {

struct DVXMLReaderPrivate {
    QVector<DVObject *> m_allObjects {};
    DVObject *m_currentObject { nullptr };
    void setCurrentObject(DVObject *obj)
    {
        if (m_currentObject == obj)
            return;

        m_currentObject = obj;
        if (m_currentObject && !m_allObjects.contains(m_currentObject))
            m_allObjects.append(m_currentObject);
    }
};

DVXMLReader::DVXMLReader(QObject *parent)
    : shared::XmlReader(parent)
    , d(new DVXMLReaderPrivate)
{
}

DVXMLReader::~DVXMLReader() { }

QVector<DVObject *> DVXMLReader::parsedObjects() const
{
    return d->m_allObjects;
}

void DVXMLReader::processTagOpen(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    const QXmlStreamAttributes attrs = xml.attributes();

    DVObject *obj { nullptr };
    const meta::Props::Token t = meta::Props::token(tagName);
    switch (t) {
    case meta::Props::Token::Partition: {
        obj = new dvm::DVPartition(d->m_currentObject);
    } break;
    case meta::Props::Token::Node: {
        obj = new dvm::DVNode(d->m_currentObject);
    } break;
    case meta::Props::Token::Device: {
        obj = new dvm::DVDevice(d->m_currentObject);
    } break;
    case meta::Props::Token::Function: {
        if (d->m_currentObject && d->m_currentObject->type() == DVObject::Type::Partition) {
            if (auto partition = d->m_currentObject->as<DVPartition *>()) {
                partition->addFunction(xml.readElementText());
            }
        }
    } break;
    case meta::Props::Token::Connection: {
        obj = new dvm::DVConnection(d->m_currentObject);
    } break;

    default:
        break;
    }

    if (obj) {
        for (const QXmlStreamAttribute &xmlAttr : attrs) {
            obj->setEntityAttribute(xmlAttr.name().toString(), QVariant::fromValue(xmlAttr.value().toString()));
        }
        d->setCurrentObject(obj);
    }
}

void DVXMLReader::processTagClose(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    switch (meta::Props::token(tagName)) {
    case meta::Props::Token::Function:
    case meta::Props::Token::Partition:
    case meta::Props::Token::Node:
    case meta::Props::Token::Device:
    case meta::Props::Token::Connection: {
        d->setCurrentObject(d->m_currentObject ? d->m_currentObject->parentObject() : nullptr);
        break;
    }
    default:
        break;
    }
}

QString DVXMLReader::rootElementName() const
{
    return meta::Props::token(meta::Props::Token::DeploymentView);
}

} // namespace dvm
