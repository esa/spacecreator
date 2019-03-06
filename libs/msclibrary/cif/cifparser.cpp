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

#include "cifblockfactory.h"
#include "exceptions.h"

#include <QDebug>

namespace msc {
namespace cif {

const QLatin1String CifParser::CifLineTag("CIF");

const QString CifParser::m_typePattern = QString("CIF\\s(\\w+).*");
const QRegularExpression CifParser::m_typeRx = QRegularExpression(m_typePattern);

CifParser::CifParser(QObject *parent)
    : QObject(parent)
{
}

CifLine::CifType CifParser::readCifType(const QString &from) const
{
    if (!from.isEmpty()) {
        QRegularExpressionMatch matched = m_typeRx.match(from);
        if (matched.hasMatch()) {
            const QString &typeName = matched.captured(1).toUpper();
            return CifLine::typeForName(typeName);
        }
    }

    return CifLine::CifType::Unknown;
}

CifLineShared CifParser::readCifLine(const QString &line) const
{
    const CifLine::CifType requestedType = readCifType(line);
    switch (requestedType) {
    case CifLine::CifType::Action:
        return readEntityAction(line);
    case CifLine::CifType::Call:
        return readEntityCall(line);
    case CifLine::CifType::Comment:
        return readEntityComment(line);
    case CifLine::CifType::Condition:
        return readEntityCondition(line);
    case CifLine::CifType::Create:
        return readEntityCreate(line);
    case CifLine::CifType::Collapsed:
        return readEntityCollapsed(line);
    case CifLine::CifType::End:
        return readEntityEnd(line);
    case CifLine::CifType::HyperLink:
        return readEntityHyperLink(line);
    case CifLine::CifType::Instance:
        return readEntityInstance(line);
    case CifLine::CifType::Import:
        return readEntityImport(line);
    case CifLine::CifType::Keep:
        return readEntityKeep(line);
    case CifLine::CifType::LastModified:
        return readEntityLastModified(line);
    case CifLine::CifType::Message:
        return readEntityMessage(line);
    case CifLine::CifType::MscDocument:
        return readEntityMscDocument(line);
    case CifLine::CifType::Modified:
        return readEntityModified(line);
    case CifLine::CifType::MscPageSize:
        return readEntityMscPageSize(line);
    case CifLine::CifType::Nested:
        return readEntityNested(line);
    case CifLine::CifType::Position:
        return readEntityPosition(line);
    case CifLine::CifType::Preview:
        return readEntityPreview(line);
    case CifLine::CifType::Reset:
        return readEntityReset(line);
    case CifLine::CifType::Set:
        return readEntitySet(line);
    case CifLine::CifType::Stop:
        return readEntityStop(line);
    case CifLine::CifType::Submsc:
        return readEntitySubmsc(line);
    case CifLine::CifType::Specific:
        return readEntitySpecific(line);
    case CifLine::CifType::Text:
        return readEntityText(line);
    case CifLine::CifType::Timeout:
        return readEntityTimeout(line);
    case CifLine::CifType::TextMode:
        return readEntityTextMode(line);
    case CifLine::CifType::TextName:
        return readEntityTextName(line);
    default: {
        throw ParserException(QString("Unknow CIF entry: %1").arg(line));
    }
    }
}

QVector<CifBlockShared> CifParser::readCifBlocks(const QStringList &lines)
{
    const QVector<CifParser::LinesCollection> &preparedCifLines = prepareCifLines(lines);
    return CifBlockFactory::createBlocks(preparedCifLines);
}

QVector<CifParser::LinesCollection> CifParser::prepareCifLines(const QStringList &lines) const
{
    QVector<LinesCollection> rawCifBlocks;

    auto newBlock = [&]() -> LinesCollection & {
        rawCifBlocks.append(LinesCollection());
        return rawCifBlocks.last();
    };

    auto block = [&]() -> LinesCollection & { return rawCifBlocks.isEmpty() ? newBlock() : rawCifBlocks.last(); };

    bool isText(false);
    auto isCifComment = [](const QString &line) { return line.startsWith(cif::CifParser::CifLineTag); };

    auto isAcceptableLine = [&isText, &isCifComment](const QString &line) { return isText || isCifComment(line); };

    auto addLine = [&](LinesCollection &to, const CifLineShared &line) {
        if (!to.isEmpty())
            if (to.last()->entityType() == CifLine::CifType::End)
                return false;

        if (!to.contains(line)) {
            to.append(line);
            return true;
        }

        return false;
    };

    for (const QString &line : lines) {
        if (isAcceptableLine(line)) {
            if (isText && !isCifComment(line)) {
                QVector<CifLineShared> &currLines = block();
                CifLineShared &lastLine = currLines.last();
                QStringList txt = lastLine->payload().toStringList();
                txt.append(line);
                lastLine->setPayload(txt);
                block() = currLines;
            } else if (const CifLineShared &cifLine = readCifLine(line)) {
                if (!addLine(block(), cifLine)) {
                    addLine(newBlock(), cifLine);
                    isText = false;
                    continue;
                }

                if (!isText)
                    isText = cifLine->entityType() == CifLine::CifType::Text;
                else if (cifLine->entityType() == CifLine::CifType::End)
                    isText = false;
            }
        }
    }

    return rawCifBlocks;
}

CifLineShared CifParser::readEntityAction(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Action));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityCall(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Call));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityComment(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Comment));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityCondition(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Condition));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityCreate(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Create));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityCollapsed(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Collapsed));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityEnd(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::End));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityHyperLink(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::HyperLink));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityInstance(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Instance));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityImport(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Import));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityKeep(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Keep));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityLastModified(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::LastModified));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityMessage(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Message));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityMscDocument(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::MscDocument));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityModified(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Modified));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityMscPageSize(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::MscPageSize));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityNested(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Nested));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityPosition(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Position));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityPreview(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Preview));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityReset(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Reset));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntitySet(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Set));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityStop(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Stop));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntitySubmsc(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Submsc));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntitySpecific(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Specific));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityText(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Text));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityTimeout(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::Timeout));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityTextMode(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::TextMode));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readEntityTextName(const QString &from) const
{
    CifLineShared cif(new CifLine(CifLine::CifType::TextName));
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

} // ns cif
} // ns msc
