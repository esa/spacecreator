/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cifparser.h"

#include "exceptions.h"

#include <QDebug>

namespace msc {
namespace cif {

const QLatin1String CifParser::CifLineTag("CIF");

//"    Line 99: /* CIF MSCDOCUMENT (20, 20), (1521, 1552) */"
const QString CifParser::m_typePattern = QString("/*\\sCIF\\s(\\w+)\\s.*\\*/");
const QRegularExpression CifParser::m_typeRx = QRegularExpression(m_typePattern);

CifParser::CifParser(QObject *parent)
    : QObject(parent)
{
}

CifEntity::CifType CifParser::readCifType(const QString &from) const
{
    if (!from.isEmpty()) {
        QRegularExpressionMatch matched = m_typeRx.match(from);
        if (matched.hasMatch()) {
            const QString &typeName = matched.captured(1).toUpper();
            return CifEntity::typeForName(typeName);
        }
    }

    return CifEntity::CifType::Unknown;
}

CifEntityShared CifParser::readCIF(const QString &line) const
{
    const CifEntity::CifType requestedType = readCifType(line);
    switch (requestedType) {
    case CifEntity::CifType::Action:
        return readEntityAction(line);
    case CifEntity::CifType::Call:
        return readEntityCall(line);
    case CifEntity::CifType::Comment:
        return readEntityComment(line);
    case CifEntity::CifType::Condition:
        return readEntityCondition(line);
    case CifEntity::CifType::Create:
        return readEntityCreate(line);
    case CifEntity::CifType::Collapsed:
        return readEntityCollapsed(line);
    case CifEntity::CifType::End:
        return readEntityEnd(line);
    case CifEntity::CifType::HyperLink:
        return readEntityHyperLink(line);
    case CifEntity::CifType::Instance:
        return readEntityInstance(line);
    case CifEntity::CifType::Import:
        return readEntityImport(line);
    case CifEntity::CifType::Keep:
        return readEntityKeep(line);
    case CifEntity::CifType::LastModified:
        return readEntityLastModified(line);
    case CifEntity::CifType::Message:
        return readEntityMessage(line);
    case CifEntity::CifType::MscDocument:
        return readEntityMscDocument(line);
    case CifEntity::CifType::Modified:
        return readEntityModified(line);
    case CifEntity::CifType::MscPageSize:
        return readEntityMscPageSize(line);
    case CifEntity::CifType::Nested:
        return readEntityNested(line);
    case CifEntity::CifType::Position:
        return readEntityPosition(line);
    case CifEntity::CifType::Preview:
        return readEntityPreview(line);
    case CifEntity::CifType::Reset:
        return readEntityReset(line);
    case CifEntity::CifType::Set:
        return readEntitySet(line);
    case CifEntity::CifType::Stop:
        return readEntityStop(line);
    case CifEntity::CifType::Submsc:
        return readEntitySubmsc(line);
    case CifEntity::CifType::Specific:
        return readEntitySpecific(line);
    case CifEntity::CifType::Text:
        return readEntityText(line);
    case CifEntity::CifType::Timeout:
        return readEntityTimeout(line);
    case CifEntity::CifType::TextMode:
        return readEntityTextMode(line);
    case CifEntity::CifType::TextName:
        return readEntityTextName(line);
    default: {
        throw ParserException(QString("Unknow CIF entry: %1").arg(line));
    }
    }
}

CifEntityShared CifParser::readEntityAction(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Action));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityCall(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Call));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityComment(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Comment));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityCondition(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Condition));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityCreate(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Create));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityCollapsed(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Collapsed));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityEnd(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::End));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityHyperLink(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::HyperLink));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityInstance(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Instance));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityImport(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Import));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityKeep(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Keep));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityLastModified(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::LastModified));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityMessage(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Message));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityMscDocument(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::MscDocument));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityModified(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Modified));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityMscPageSize(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::MscPageSize));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityNested(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Nested));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityPosition(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Position));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityPreview(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Preview));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityReset(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Reset));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntitySet(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Set));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityStop(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Stop));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntitySubmsc(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Submsc));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntitySpecific(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Specific));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityText(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Text));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityTimeout(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::Timeout));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityTextMode(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::TextMode));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifEntityShared CifParser::readEntityTextName(const QString &from) const
{
    CifEntityShared cif(new CifEntity(CifEntity::CifType::TextName));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

} // ns cif
} // ns msc
