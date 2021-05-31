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

#include "dvboardreader.h"

#include "dvboard.h"
#include "dvbus.h"
#include "dvcommonprops.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvfunction.h"
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

DVBoardReader::DVBoardReader(QObject *parent)
    : shared::XmlReader(parent)
    , d(new DVBoardReaderPrivate)
{
}

DVBoardReader::~DVBoardReader() { }

QVector<DVObject *> DVBoardReader::parsedObjects() const
{
    return d->m_allObjects;
}

QList<DVBoard *> DVBoardReader::parsedBoards() const
{
    QList<DVBoard *> boards;
    for (DVObject *obj : d->m_allObjects) {
        if (auto board = qobject_cast<DVBoard *>(obj)) {
            boards.append(board);
        }
    }
    return boards;
}

void DVBoardReader::processTagOpen(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    const QXmlStreamAttributes attrs = xml.attributes();

    DVObject *obj { nullptr };
    const meta::Props::Token t = meta::Props::token(tagName);
    switch (t) {
    case meta::Props::Token::Board: {
        obj = new dvm::DVBoard();
    } break;
    case meta::Props::Token::Device: {
        if (auto node = qobject_cast<DVBoard *>(d->m_currentObject)) {
            auto dev = new dvm::DVDevice(d->m_currentObject);
            obj = dev;
        }
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

void DVBoardReader::processTagClose(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    if (meta::Props::token(tagName) == meta::Props::Token::Device) {
        d->m_currentObject = d->m_currentObject ? d->m_currentObject->parentObject() : nullptr;
    } else {
        d->m_currentObject = nullptr;
    }
}

QString DVBoardReader::rootElementName() const
{
    return meta::Props::token(meta::Props::Token::Boards);
}

} // namespace dvm
