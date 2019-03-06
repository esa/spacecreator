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

#include "cifline.h"

#include <QDebug>
#include <QMetaEnum>

namespace msc {
namespace cif {

CifLine::CifLine(CifType type, QObject *parent)
    : QObject(parent)
    , m_type(type)
{
}

CifLine::~CifLine() {}

CifLine::CifType CifLine::entityType() const
{
    return m_type;
}

/*!
  \brief CifLine::initFrom
  Parse concrete instance of derived class from a \a sourceLine.
  \return true in case of successful parsing.
  Default implementation prints warning and returns false.
*/
bool CifLine::initFrom(const QString &sourceLine)
{
#ifdef QT_DEBUG
    // For initial development/tests only, to be removed
    m_sourceLine = sourceLine;
    return true;
#else
    qWarning() << Q_FUNC_INFO << "Base implimentation call!" << sourceLine;
    m_sourceLine = sourceLine;
    return false;
#endif
}

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
    return m_type == other.m_type && m_sourceLine == other.m_sourceLine;
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
    m_payload = p;
}

} // ns cif
} // ns msc
