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

#include "cifblock.h"

#include <QCryptographicHash>

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

bool CifBlock::addLine(const CifLineShared &line)
{
    if (!m_lines.isEmpty())
        if (m_lines.last()->entityType() == CifLine::CifType::End)
            return false;

    if (!m_lines.contains(line)) {
        m_lines.append(line);
        updateHashKey();
        return true;
    }

    return false;
}

QString CifBlock::hashKey() const
{
    return m_hashKey;
}

bool CifBlock::isPeculiar() const
{
    for (const CifLineShared &line : m_lines) {
        switch (line->entityType()) {
        case CifLine::CifType::Text:
        case CifLine::CifType::End:
            return true;
        default:
            break;
        }
    }

    return false;
}

void CifBlock::updateHashKey()
{
    m_hashKey.clear();

    QString strings;
    for (const CifLineShared &line : lines())
        strings.append(line->sourceLine());

    if (!strings.isEmpty())
        m_hashKey = QCryptographicHash::hash(strings.toUtf8(), QCryptographicHash::Md5);
}

} // ns cif
} // ns msc
