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

#include "dvhwlibraryreader.h"

#include "dvboard.h"
#include "dvcommonprops.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvfunction.h"
#include "dvobject.h"
#include "dvpartition.h"
#include "dvport.h"
#include "dvsystemfunction.h"
#include "dvsysteminterface.h"
#include "errorhub.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QVariant>
#include <QVector>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>

namespace dvm {

struct DVBoardReaderPrivate {
    QVector<DVObject *> m_allObjects {};
    DVObject *m_currentObject { nullptr };
    void addObject(DVObject *obj)
    {
        if (obj && !m_allObjects.contains(obj)) {
            m_allObjects.append(obj);
        }
    }
};

DVHWLibraryReader::DVHWLibraryReader(QObject *parent)
    : shared::XmlReader(parent)
    , d(new DVBoardReaderPrivate)
{
}

DVHWLibraryReader::~DVHWLibraryReader() { }

QVector<DVObject *> DVHWLibraryReader::parsedObjects() const
{
    return d->m_allObjects;
}

void DVHWLibraryReader::processTagOpen(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    const QXmlStreamAttributes attrs = xml.attributes();

    DVObject *obj { nullptr };
    const meta::Props::Token t = meta::Props::token(tagName);
    switch (t) {
    case meta::Props::Token::Board: {
        obj = new dvm::DVBoard();
    } break;
    case meta::Props::Token::Port: {
        if (auto board = qobject_cast<DVBoard *>(d->m_currentObject)) {
            auto port = new dvm::DVPort(d->m_currentObject);
            board->addPort(port);
            obj = port;
        }
    } break;
    case meta::Props::Token::System_Function: {
        Q_ASSERT(d->m_currentObject != nullptr);
        if (auto board = qobject_cast<DVBoard *>(d->m_currentObject)) {
            obj = new dvm::DVSystemFunction(d->m_currentObject);
        }
    } break;
    case meta::Props::Token::Provided_Interface:
    case meta::Props::Token::Required_Interface: {
        Q_ASSERT(d->m_currentObject != nullptr);
        if (auto board = qobject_cast<DVSystemFunction *>(d->m_currentObject)) {
            auto interface = new dvm::DVSystemInterface(d->m_currentObject);
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

void DVHWLibraryReader::processTagClose(QXmlStreamReader &xml)
{
    Q_UNUSED(xml)
    d->m_currentObject = d->m_currentObject ? d->m_currentObject->parentObject() : nullptr;
}

QString DVHWLibraryReader::rootElementName() const
{
    return meta::Props::token(meta::Props::Token::Boards);
}

} // namespace dvm
