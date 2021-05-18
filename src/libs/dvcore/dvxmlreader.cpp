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

#include "dvbus.h"
#include "dvcommonprops.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvfunction.h"
#include "dvnode.h"
#include "dvobject.h"
#include "dvpartition.h"
#include "dvprocessor.h"

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
    QHash<QString, DVProcessor *> m_processors;
    QHash<QString, DVBus *> m_buses;
    void addObject(DVObject *obj)
    {
        if (obj && !m_allObjects.contains(obj)) {
            m_allObjects.append(obj);
            if (obj->type() == DVObject::Type::Processor) {
                Q_ASSERT(!m_processors.contains(obj->title()));
                m_processors.insert(obj->title(), obj->as<DVProcessor *>());
            } else if (obj->type() == DVObject::Type::Bus) {
                Q_ASSERT(!m_buses.contains(obj->title()));
                m_buses.insert(obj->title(), obj->as<DVBus *>());
            }
        }
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
        const QString cpuName = attrs.value(QLatin1String("cpu")).toString();
        auto cpu = d->m_processors.value(cpuName);
        if (!cpu) {
            cpu = createProcessor(cpuName);
        }
        if (cpu) {
            auto partition = new dvm::DVPartition(cpu);
            cpu->addPartition(partition);
            obj = partition;
        }
    } break;
    case meta::Props::Token::Node: {
        obj = new dvm::DVNode(d->m_currentObject);
    } break;
    case meta::Props::Token::Device: {
        if (auto node = qobject_cast<DVNode *>(d->m_currentObject)) {
            const QString cpuName = attrs.value(QLatin1String("proc")).toString();
            auto cpu = d->m_processors.value(cpuName);
            if (!cpu) {
                cpu = createProcessor(cpuName);
            }
            const QString busName = attrs.value(QLatin1String("bus")).toString();
            dvm::DVBus *bus = d->m_buses.value(busName);
            if (!bus) {
                bus = new dvm::DVBus;
                bus->setTitle(busName);
                d->addObject(bus);
            }

            auto dev = new dvm::DVDevice(d->m_currentObject);
            node->setDevice(dev);
            obj = dev;
        }
    } break;
    case meta::Props::Token::Function: {
        if (auto partition = qobject_cast<dvm::DVPartition *>(d->m_currentObject)) {
            auto fn = new dvm::DVFunction(d->m_currentObject);
            fn->setTitle(xml.readElementText());
            partition->addFunction(fn);
            obj = fn;
        }
    } break;
    case meta::Props::Token::Connection: {
        const QString busName = attrs.value(QLatin1String("to_bus")).toString();
        dvm::DVBus *bus = d->m_buses.value(busName);
        if (!bus) {
            bus = new dvm::DVBus;
            bus->setTitle(busName);
            d->addObject(bus);
        }
        auto connection = new dvm::DVConnection(bus);
        bus->addConnection(connection);
        obj = connection;
    } break;

    default:
        static const QString msg("The '%1' is unknown/unexpected here: %2@%3 %4");
        qWarning() << msg.arg(
                tagName, QString::number(xml.lineNumber()), QString::number(xml.columnNumber()), xml.tokenString());
        break;
    }

    if (obj) {
        for (const QXmlStreamAttribute &xmlAttr : attrs) {
            obj->setEntityAttribute(xmlAttr.name().toString(), QVariant::fromValue(xmlAttr.value().toString()));
        }
        d->addObject(obj);
        d->m_currentObject = obj;
        if (xml.isEndElement()) {
            processTagClose(xml);
        }
    }
}

void DVXMLReader::processTagClose(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    switch (meta::Props::token(tagName)) {
    case meta::Props::Token::Partition:
        if (d->m_currentObject) {
            if (auto cpu = d->m_currentObject->parentObject()) {
                d->m_currentObject = cpu->parentObject();
                break;
            }
        }
        d->m_currentObject = nullptr;
        break;
    case meta::Props::Token::Function:
    case meta::Props::Token::Node:
    case meta::Props::Token::Device:
    case meta::Props::Token::Connection: {
        d->m_currentObject = d->m_currentObject ? d->m_currentObject->parentObject() : nullptr;
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

DVProcessor *DVXMLReader::createProcessor(const QString &name)
{
    if (auto node = qobject_cast<DVNode *>(d->m_currentObject)) {
        DVProcessor *cpu = d->m_processors.value(name);
        if (!cpu) {
            cpu = new DVProcessor(node);
            cpu->setTitle(name);
            node->setProcessor(cpu);
            d->addObject(cpu);
            d->m_processors.insert(name, cpu);
        }
        return cpu;
    }
    return nullptr;
}

} // namespace dvm
