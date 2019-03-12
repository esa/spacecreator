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

#include "cifline.h"

#include <QDebug>
#include <QMetaEnum>
#include <QRegularExpression>

namespace msc {
namespace cif {

CifLine::CifLine(QObject *parent)
    : QObject(parent)
{
}

CifLine::~CifLine() {}

QString CifLine::nameForType(CifLine::CifType t)
{
    auto initTypes = []() {
        QMap<CifLine::CifType, QString> res;

        const QMetaEnum &e = QMetaEnum::fromType<CifLine::CifType>();
        for (int i = 0; i < e.keyCount(); ++i)
            res.insert(CifLine::CifType(e.value(i)), QString(e.key(i)).toUpper());

        return res;
    };

    static const QMap<CifLine::CifType, QString> namesByType = initTypes();
    return namesByType.contains(t) ? namesByType.value(t) : namesByType.value(CifLine::CifType::Unknown);
}

CifLine::CifType CifLine::typeForName(const QString &name)
{
    auto initNames = []() {
        QMap<QString, CifLine::CifType> res;

        const QMetaEnum &e = QMetaEnum::fromType<CifLine::CifType>();
        for (int i = 0; i < e.keyCount(); ++i)
            res.insert(QString(e.key(i)).toUpper(), CifLine::CifType(e.value(i)));

        return res;
    };
    static const QMap<QString, CifLine::CifType> typesByName = initNames();

    return typesByName.contains(name) ? typesByName.value(name)
                                      : typesByName.value(nameForType(CifLine::CifType::Unknown));
}

bool CifLine::operator==(const CifLine &other) const
{
    return lineType() == other.lineType() && m_sourceLine == other.m_sourceLine;
}

QString CifLine::sourceLine() const
{
    return m_sourceLine;
}

QVariant CifLine::payload() const
{
    return m_payload;
}
void CifLine::setPayload(const QVariant &p)
{
    if (m_payload != p)
        m_payload = p;
}

void CifLine::setPayloadPoints(const QVector<QPoint> &points)
{
    setPayload(QVariant::fromValue(points));
}

QPoint CifLine::stringToPoint(const QString &from, bool *ok)
{
    QPoint res;
    const QStringList &coordinates = from.split(",", QString::SkipEmptyParts);
    if (coordinates.size() == 2) {
        res.rx() = coordinates.first().trimmed().toInt(ok);
        if (ok && *ok)
            res.ry() = coordinates.last().trimmed().toInt(ok);
    }
    return res;
};

CifLinePointsHolder::CifLinePointsHolder(int pointsCount)
    : CifLine()
    , m_pointsCount(pointsCount)
{
}

bool CifLinePointsHolder::initFrom(const QString &sourceLine)
{
    m_sourceLine = sourceLine;
    return initPoints(sourceLine);
}

int CifLinePointsHolder::targetPointsCount() const
{
    return m_pointsCount;
}

bool CifLinePointsHolder::initPoints(const QString &line)
{
    if (line.isEmpty())
        return false;

    // It seems the reals are not used in CIF, but the negative ints are in use.
    //    const QRegularExpression rxPoint("\\((-?\\d+\\.?\\d*, -?\\d+\\.?\\d*)\\)");
    const QRegularExpression rxPoint("\\((-?\\d+, -?\\d+)\\)");
    QRegularExpressionMatchIterator m = rxPoint.globalMatch(line);
    QVector<QPoint> points;
    while (m.hasNext()) {
        QRegularExpressionMatch match = m.next();
        bool parsed(false);
        const QPoint &point = stringToPoint(match.captured(1), &parsed);
        if (!parsed) {
            qWarning() << "Failed to parse point at" << match.capturedStart() << "in" << line;
            return false;
        }
        points.append(point);
    }

    if (points.size() == m_pointsCount || (CifLinePointsHolder::AllPoints == m_pointsCount && !points.isEmpty())) {
        setPayloadPoints(points);
        return true;
    } else {
        qWarning() << QString("Expected amount of points is %1, the matched one: %2. (%3)")
                              .arg(m_pointsCount)
                              .arg(points.size())
                              .arg(m_sourceLine);
    }

    return false;
}

} // ns cif
} // ns msc
