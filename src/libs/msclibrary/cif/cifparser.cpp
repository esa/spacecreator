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

#include "cif/ciflines.h"
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
        return readCifLineAction(line);
    case CifLine::CifType::Call:
        return readCifLineCall(line);
    case CifLine::CifType::Comment:
        return readCifLineComment(line);
    case CifLine::CifType::Condition:
        return readCifLineCondition(line);
    case CifLine::CifType::Create:
        return readCifLineCreate(line);
    case CifLine::CifType::Collapsed:
        return readCifLineCollapsed(line);
    case CifLine::CifType::End:
        return readCifLineEnd(line);
    case CifLine::CifType::HyperLink:
        return readCifLineHyperLink(line);
    case CifLine::CifType::Instance:
        return readCifLineInstance(line);
    case CifLine::CifType::Import:
        return readCifLineImport(line);
    case CifLine::CifType::Keep:
        return readCifLineKeep(line);
    case CifLine::CifType::LastModified:
        return readCifLineLastModified(line);
    case CifLine::CifType::Message:
        return readCifLineMessage(line);
    case CifLine::CifType::MscDocument:
        return readCifLineMscDocument(line);
    case CifLine::CifType::Modified:
        return readCifLineModified(line);
    case CifLine::CifType::MscPageSize:
        return readCifLineMscPageSize(line);
    case CifLine::CifType::Nested:
        return readCifLineNested(line);
    case CifLine::CifType::Position:
        return readCifLinePosition(line);
    case CifLine::CifType::Preview:
        return readCifLinePreview(line);
    case CifLine::CifType::Reset:
        return readCifLineReset(line);
    case CifLine::CifType::Set:
        return readCifLineSet(line);
    case CifLine::CifType::Stop:
        return readCifLineStop(line);
    case CifLine::CifType::Submsc:
        return readCifLineSubmsc(line);
    case CifLine::CifType::Specific:
        return readCifLineSpecific(line);
    case CifLine::CifType::Text:
        return readCifLineText(line);
    case CifLine::CifType::Timeout:
        return readCifLineTimeout(line);
    case CifLine::CifType::TextMode:
        return readCifLineTextMode(line);
    case CifLine::CifType::TextName:
        return readCifLineTextName(line);
    default: {
        qDebug() << (QString("Unknown CIF entry: %1").arg(line));
        return {};
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
            if (to.last()->lineType() == CifLine::CifType::End)
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
                if (CifLineShared cifLine = readCifLineGlobalComment(line)) {
                    if (!addLine(block(), cifLine))
                        qWarning() << QString("Can't add the \"CIF\" line with text");
                } else {
                    qWarning() << QString("Can't create the \"CIF\" line to put the text in");
                }
            } else if (const CifLineShared &cifLine = readCifLine(line)) {
                if (!addLine(block(), cifLine)) {
                    addLine(newBlock(), cifLine);
                    isText = false;
                    continue;
                }

                if (!isText)
                    isText = cifLine->lineType() == CifLine::CifType::Text;
                else if (cifLine->lineType() == CifLine::CifType::End)
                    isText = false;
            }
        }
    }

    return rawCifBlocks;
}

CifLineShared CifParser::readCifLineAction(const QString &from) const
{
    CifLineShared cif(new CifLineAction());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineCall(const QString &from) const
{
    CifLineShared cif(new CifLineCall());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineComment(const QString &from) const
{
    CifLineShared cif(new CifLineComment());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineCondition(const QString &from) const
{
    CifLineShared cif(new CifLineCondition());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineCreate(const QString &from) const
{
    CifLineShared cif(new CifLineCreate());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineCollapsed(const QString &from) const
{
    CifLineShared cif(new CifLineCollapsed());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineEnd(const QString &from) const
{
    CifLineShared cif(new CifLineEnd());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineHyperLink(const QString &from) const
{
    CifLineShared cif(new CifLineHyperLink());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineInstance(const QString &from) const
{
    CifLineShared cif(new CifLineInstance());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineImport(const QString &from) const
{
    CifLineShared cif(new CifLineImport());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineKeep(const QString &from) const
{
    CifLineShared cif(new CifLineKeep());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineLastModified(const QString &from) const
{
    CifLineShared cif(new CifLineLastModified());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineMessage(const QString &from) const
{
    CifLineShared cif(new CifLineMessage());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineMscDocument(const QString &from) const
{
    CifLineShared cif(new CifLineMscDocument());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineModified(const QString &from) const
{
    CifLineShared cif(new CifLineModified());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineMscPageSize(const QString &from) const
{
    CifLineShared cif(new CifLineMscPageSize());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineNested(const QString &from) const
{
    CifLineShared cif(new CifLineNested());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLinePosition(const QString &from) const
{
    CifLineShared cif(new CifLinePosition());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLinePreview(const QString &from) const
{
    CifLineShared cif(new CifLinePreview());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineReset(const QString &from) const
{
    CifLineShared cif(new CifLineReset());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineSet(const QString &from) const
{
    CifLineShared cif(new CifLineSet());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineStop(const QString &from) const
{
    CifLineShared cif(new CifLineStop());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineSubmsc(const QString &from) const
{
    CifLineShared cif(new CifLineSubmsc());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineSpecific(const QString &from) const
{
    CifLineShared cif(new CifLineSpecific());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineText(const QString &from) const
{
    CifLineShared cif(new CifLineText());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineTimeout(const QString &from) const
{
    CifLineShared cif(new CifLineTimeout());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineTextMode(const QString &from) const
{
    CifLineShared cif(new CifLineTextMode());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineTextName(const QString &from) const
{
    CifLineShared cif(new CifLineTextName());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

CifLineShared CifParser::readCifLineGlobalComment(const QString &from) const
{
    CifLineShared cif(new CifLineGlobalComment());
    if (!cif->initFrom(from)) {
        cif.reset();
    }
    return cif;
}

} // ns cif
} // ns msc
