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

#include "cifblock.h"

#include <QCryptographicHash>
#include <QDebug>

namespace msc {
namespace cif {

CifBlock::~CifBlock() {}

QVector<CifLineShared> CifBlock::lines() const
{
    return m_lines;
}

void CifBlock::setLines(const QVector<CifLineShared> &lines)
{
    m_lines = lines;
    updateHashKey();
}

QString CifBlock::hashKey() const
{
    return m_hashKey;
}

bool CifBlock::isPeculiar() const
{
    return hasPayloadFor(CifLine::CifType::Text) || hasPayloadFor(CifLine::CifType::End);
}

QVariant CifBlock::payload(CifLine::CifType forType) const
{
    CifLine::CifType targetType = (forType == CifLine::CifType::Unknown) ? blockType() : forType;
    return hasPayloadFor(targetType) ? m_linesByType.value(targetType)->payload() : QVariant();
}

QVariant &CifBlock::payload(CifLine::CifType forType)
{
    return m_linesByType.value(forType)->payload();
}

bool CifBlock::hasPayloadFor(CifLine::CifType forType) const
{
    return m_linesByType.contains(forType);
}

void CifBlock::setPayload(const QVariant &p, CifLine::CifType forType)
{
    CifLine::CifType targetType = (forType == CifLine::CifType::Unknown) ? blockType() : forType;

    if (targetType != CifLine::CifType::Unknown) {
        if (hasPayloadFor(targetType))
            m_linesByType.value(targetType)->setPayload(p);
        else {
            qWarning() << Q_FUNC_INFO << "Line for type not found:" << targetType;
        }
    } else {
        qWarning() << Q_FUNC_INFO << "Attempt to update payload for CIF block with Unkonw type.";
    }
}

void CifBlock::updateHashKey()
{
    m_hashKey.clear();
    m_linesByType.clear();

    QString strings;
    for (const CifLineShared &line : lines()) {
        strings.append(line->sourceLine());
        m_linesByType.insert(line->lineType(), line);
    }

    if (!strings.isEmpty())
        m_hashKey = QCryptographicHash::hash(strings.toUtf8(), QCryptographicHash::Md5);
}

void CifBlock::addLine(const CifLineShared &line)
{
    if (m_lines.contains(line) || hasPayloadFor(line->lineType()))
        return;

    m_lines.append(line);
    updateHashKey();
}

QString CifBlock::toString(int tabsSize) const
{
    QString result;
    for (const CifLineShared &line : m_lines)
        result += line->toString(tabsSize);

    return result;
}

} // ns cif
} // ns msc
