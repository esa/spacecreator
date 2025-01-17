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
#include "dvfunction.h"
#include "dvmessage.h"
#include "dvnode.h"
#include "dvobject.h"
#include "dvpartition.h"
#include "dvsystemfunction.h"
#include "dvsysteminterface.h"
#include "errorhub.h"
#include "uireader.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QVariant>
#include <QVector>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>

namespace dvm {

struct DVXMLReaderPrivate {
    DVXMLReaderPrivate()
        : m_uiReader(new shared::UIReader)
    {
    }

    QVector<DVObject *> m_allObjects {};
    DVObject *m_currentObject { nullptr };
    QScopedPointer<shared::UIReader> m_uiReader { nullptr };

    void addObject(DVObject *obj)
    {
        if (obj && !m_allObjects.contains(obj)) {
            m_allObjects.append(obj);
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

QString DVXMLReader::uiFileNameTag() const
{
    return d->m_uiReader ? d->m_uiReader->filePathAttributeName() : QString();
}

QHash<shared::Id, EntityAttributes> DVXMLReader::externalAttributes() const
{
    return d->m_uiReader ? d->m_uiReader->data() : QHash<shared::Id, EntityAttributes>();
}

bool DVXMLReader::processTagOpen(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    EntityAttributes attrs = attributes(xml.attributes());
    const EntityAttribute &idAttr = attrs.take(QLatin1String("id"));

    if (d->m_uiReader && d->m_uiReader->rootElementName() == tagName) {
        auto fileIt = attrs.constFind(d->m_uiReader->filePathAttributeName());
        if (fileIt != attrs.constEnd()) {
            processUIFile(fileIt->value<QString>());
        } else if (d->m_uiReader->readSection(xml)) {
            processUIData();
        }
        return true;
    }

    DVObject *obj { nullptr };
    const meta::Props::Token t = meta::Props::token(tagName);
    switch (t) {
    case meta::Props::Token::Partition: {
        if (auto node = qobject_cast<dvm::DVNode *>(d->m_currentObject)) {
            auto partition = new dvm::DVPartition(node, idAttr.value<QUuid>());
            node->addPartition(partition);
            obj = partition;
        }
    } break;
    case meta::Props::Token::Node: {
        obj = new dvm::DVNode(d->m_currentObject, idAttr.value<QUuid>());
    } break;
    case meta::Props::Token::Device: {
        if (auto node = qobject_cast<DVNode *>(d->m_currentObject)) {
            auto dev = new dvm::DVDevice(d->m_currentObject, idAttr.value<QUuid>());
            node->addDevice(dev);
            obj = dev;
        }
    } break;
    case meta::Props::Token::Function: {
        if (auto partition = qobject_cast<dvm::DVPartition *>(d->m_currentObject)) {
            auto fn = new dvm::DVFunction(d->m_currentObject, idAttr.value<QUuid>());
            if (tagName == "function") {
                // old version
                fn->setTitle(xml.readElementText());
            }
            partition->addFunction(fn);
            obj = fn;
        }
    } break;
    case meta::Props::Token::Connection: {
        obj = new dvm::DVConnection(d->m_currentObject, idAttr.value<QUuid>());
    } break;
    case meta::Props::Token::Message: {
        obj = new dvm::DVMessage(d->m_currentObject, idAttr.value<QUuid>());
    } break;
    case meta::Props::Token::Property: {
        if (d->m_currentObject) {
            d->m_currentObject->setEntityProperty(
                    attrs.value(meta::Props::token(meta::Props::Token::name)).value<QString>(),
                    attrs.value(meta::Props::token(meta::Props::Token::value)).value());
        }
        break;
    }
    case meta::Props::Token::System_Function: {
        Q_ASSERT(d->m_currentObject != nullptr);
        if (qobject_cast<DVNode *>(d->m_currentObject)) {
            obj = new dvm::DVSystemFunction(d->m_currentObject, idAttr.value<QUuid>());
        }
    } break;
    case meta::Props::Token::Provided_Interface:
    case meta::Props::Token::Required_Interface: {
        Q_ASSERT(d->m_currentObject != nullptr);
        if (qobject_cast<DVSystemFunction *>(d->m_currentObject)) {
            auto interface = new dvm::DVSystemInterface(d->m_currentObject, idAttr.value<QUuid>());
            obj = interface;
            interface->setInterfaceType(t == meta::Props::Token::Provided_Interface
                            ? dvm::DVSystemInterface::InterfaceType::Provided
                            : dvm::DVSystemInterface::InterfaceType::Required);
            break;
        }
        break;
    }
    case meta::Props::Token::Output_Parameter:
    case meta::Props::Token::Input_Parameter: {
        auto iface = qobject_cast<DVSystemInterface *>(d->m_currentObject);
        Q_ASSERT(iface != nullptr);
        const EntityAttributes attrs = attributes(xml.attributes());
        const shared::InterfaceParameter param = addIfaceParameter(attrs,
                t == meta::Props::Token::Input_Parameter ? shared::InterfaceParameter::Direction::IN
                                                         : shared::InterfaceParameter::Direction::OUT);
        iface->addParam(param);
        break;
    }
    default:
        static const QString msg = tr("The '%1' is unknown/unexpected here: %2@%3 %4");
        shared::ErrorHub::addError(shared::ErrorItem::Warning,
                msg.arg(tagName, QString::number(xml.lineNumber()), QString::number(xml.columnNumber()),
                        xml.tokenString()),
                file(), xml.lineNumber());
        break;
    }

    if (obj) {
        for (const EntityAttribute &attr : qAsConst(attrs)) {
            obj->setEntityAttribute(attr);
        }
        d->addObject(obj);
        d->m_currentObject = obj;
        if (xml.isEndElement()) {
            processTagClose(xml);
        }
    }
    return true;
}

bool DVXMLReader::processTagClose(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    switch (meta::Props::token(tagName)) {
    case meta::Props::Token::DeploymentView: {
        const QVariantMap &metaInfo = metaData();
        auto it = metaInfo.constFind(d->m_uiReader->filePathAttributeName());
        if (it != metaInfo.constEnd()) {
            processUIFile(it->toString());
        }
    } break;
    case meta::Props::Token::Partition:
    case meta::Props::Token::Function:
    case meta::Props::Token::Node:
    case meta::Props::Token::Device:
    case meta::Props::Token::Connection:
    case meta::Props::Token::Message:
    case meta::Props::Token::System_Function:
    case meta::Props::Token::Provided_Interface:
    case meta::Props::Token::Required_Interface: {
        d->m_currentObject = d->m_currentObject ? d->m_currentObject->parentObject() : nullptr;
        break;
    }
    default:
        break;
    }
    return true;
}

QString DVXMLReader::rootElementName() const
{
    return meta::Props::token(meta::Props::Token::DeploymentView);
}

void DVXMLReader::processUIData()
{
    const QHash<shared::Id, EntityAttributes> attrs = d->m_uiReader->data();
    for (auto attrIt = attrs.constBegin(); attrIt != attrs.constEnd(); ++attrIt) {
        auto objIt = std::find_if(d->m_allObjects.begin(), d->m_allObjects.end(),
                [id = attrIt.key()](DVObject *obj) { return obj->id() == id; });
        if (objIt != d->m_allObjects.end()) {
            for (const EntityAttribute &entityAttr : attrIt.value()) {
                (*objIt)->setEntityAttribute(entityAttr);
            }
        }
    }
}

void DVXMLReader::processUIFile(const QString &relFileName)
{
    const QFileInfo fi(file());
    const QString uiFile = fi.absoluteDir().filePath(relFileName);
    if (d->m_uiReader->readFile(uiFile)) {
        processUIData();
    }
}

} // namespace dvm
