/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ciflines.h"

#include "cifparser.h"

#include <QDebug>
#include <QPoint>
#include <QRegularExpression>
#include <QVector>

namespace msc {
namespace cif {

CifLine::CifType CifLineAction::lineType() const
{
    return CifLine::CifType::Action;
}

CifLine::CifType CifLineCall::lineType() const
{
    return CifLine::CifType::Call;
}

CifLine::CifType CifLineComment::lineType() const
{
    return CifLine::CifType::Comment;
}

CifLine::CifType CifLineCondition::lineType() const
{
    return CifLine::CifType::Condition;
}

CifLine::CifType CifLineCreate::lineType() const
{
    return CifLine::CifType::Create;
}

CifLine::CifType CifLineCollapsed::lineType() const
{
    return CifLine::CifType::Collapsed;
}
bool CifLineCollapsed::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLineCollapsed::payload() const
{
    return CifLine::payload();
}
void CifLineCollapsed::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLineEnd::lineType() const
{
    return CifLine::CifType::End;
}
bool CifLineEnd::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLineEnd::payload() const
{
    return CifLine::payload();
}
void CifLineEnd::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

QString CifLineEnd::payloadToString() const
{
    return payload().toString();
}

CifLine::CifType CifLineHyperLink::lineType() const
{
    return CifLine::CifType::HyperLink;
}
bool CifLineHyperLink::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLineHyperLink::payload() const
{
    return CifLine::payload();
}
void CifLineHyperLink::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLineInstance::lineType() const
{
    return CifLine::CifType::Instance;
}

CifLine::CifType CifLineImport::lineType() const
{
    return CifLine::CifType::Import;
}
bool CifLineImport::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLineImport::payload() const
{
    return CifLine::payload();
}
void CifLineImport::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLineKeep::lineType() const
{
    return CifLine::CifType::Keep;
}
bool CifLineKeep::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLineKeep::payload() const
{
    return CifLine::payload();
}
void CifLineKeep::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLineLastModified::lineType() const
{
    return CifLine::CifType::LastModified;
}
bool CifLineLastModified::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLineLastModified::payload() const
{
    return CifLine::payload();
}
void CifLineLastModified::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLineMessage::lineType() const
{
    return CifLine::CifType::Message;
}

CifLine::CifType CifLineMscDocument::lineType() const
{
    return CifLine::CifType::MscDocument;
}

CifLine::CifType CifLineModified::lineType() const
{
    return CifLine::CifType::Modified;
}
bool CifLineModified::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLineModified::payload() const
{
    return CifLine::payload();
}
void CifLineModified::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLineMscPageSize::lineType() const
{
    return CifLine::CifType::MscPageSize;
}

CifLine::CifType CifLineNested::lineType() const
{
    return CifLine::CifType::Nested;
}
bool CifLineNested::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLineNested::payload() const
{
    return CifLine::payload();
}
void CifLineNested::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLinePosition::lineType() const
{
    return CifLine::CifType::Position;
}

CifLine::CifType CifLinePreview::lineType() const
{
    return CifLine::CifType::Preview;
}
bool CifLinePreview::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLinePreview::payload() const
{
    return CifLine::payload();
}
void CifLinePreview::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLineReset::lineType() const
{
    return CifLine::CifType::Reset;
}

CifLine::CifType CifLineSet::lineType() const
{
    return CifLine::CifType::Set;
}

CifLine::CifType CifLineStop::lineType() const
{
    return CifLine::CifType::Stop;
}

CifLine::CifType CifLineSubmsc::lineType() const
{
    return CifLine::CifType::Submsc;
}
bool CifLineSubmsc::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLineSubmsc::payload() const
{
    return CifLine::payload();
}
void CifLineSubmsc::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLineSpecific::lineType() const
{
    return CifLine::CifType::Specific;
}
bool CifLineSpecific::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLineSpecific::payload() const
{
    return CifLine::payload();
}
void CifLineSpecific::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLineText::lineType() const
{
    return CifLine::CifType::Text;
}

CifLine::CifType CifLineTimeout::lineType() const
{
    return CifLine::CifType::Timeout;
}

CifLine::CifType CifLineTextMode::lineType() const
{
    return CifLine::CifType::TextMode;
}
bool CifLineTextMode::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return !m_sourceLine.isEmpty();
}
QVariant CifLineTextMode::payload() const
{
    return CifLine::payload();
}
void CifLineTextMode::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLineTextName::lineType() const
{
    return CifLine::CifType::TextName;
}
bool CifLineTextName::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;

    if (m_sourceLine.isEmpty())
        return false;

    const QString line = QString(sourceLine).remove(CifParser::CifLineTag).remove(nameForType(lineType()));
    setPayload(line.trimmed());

    return !line.isEmpty();
}
QVariant CifLineTextName::payload() const
{
    return CifLine::payload();
}
void CifLineTextName::setPayload(const QVariant &p)
{
    return CifLine::setPayload(p);
}

CifLine::CifType CifLineGlobalComment::lineType() const
{
    return CifLine::CifType::GlobalComment;
}

bool CifLineGlobalComment::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;

    const QString text = sourceLine.trimmed();
    setPayload(text);
    return !text.isEmpty();
}
QString CifLineGlobalComment::payloadToString() const
{
    return payload().toString();
}

QString CifLineGlobalComment::toString(int tabsSize) const
{
    const QString tabs(4 * (tabsSize > 0 ? tabsSize - 1 : tabsSize), ' ');
    const QString payload = payloadToString();
    return payload.isEmpty() ? QString() : QString("%1/* %2 */\n").arg(tabs, payload);
}

} // ns cif
} // ns msc
