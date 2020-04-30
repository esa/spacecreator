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

#include "cifentity.h"

#include <QDebug>
#include <QMetaEnum>

namespace msc {
namespace cif {

CifEntity::CifEntity(CifType type, QObject *parent)
    : QObject(parent)
    , m_type(type)
{
}

CifEntity::CifType CifEntity::entityType() const
{
    return m_type;
}

/*!
  \brief CifEntity::initFrom
  Parse concrete instance of derived class from a \a sourceLine.
  \return true in case of successful parsing.
  Default implementation prints warning and returns false.
*/
bool CifEntity::initFrom(const QString &sourceLine)
{
    qWarning() << Q_FUNC_INFO << "Base implimentation call!" << sourceLine;
    return false;
}

QString CifEntity::nameForType(CifEntity::CifType t)
{
    auto initTypes = []() {
        QMap<CifEntity::CifType, QString> res;

        const QMetaEnum &e = QMetaEnum::fromType<CifEntity::CifType>();
        for (int i = 0; i < e.keyCount(); ++i)
            res.insert(CifEntity::CifType(e.value(i)), QString(e.key(i)).toUpper());

        return res;
    };

    static const QMap<CifEntity::CifType, QString> namesByType = initTypes();
    return namesByType.contains(t) ? namesByType.value(t) : namesByType.value(CifEntity::CifType::Unknown);
}

CifEntity::CifType CifEntity::typeForName(const QString &name)
{
    auto initNames = []() {
        QMap<QString, CifEntity::CifType> res;

        const QMetaEnum &e = QMetaEnum::fromType<CifEntity::CifType>();
        for (int i = 0; i < e.keyCount(); ++i)
            res.insert(QString(e.key(i)).toUpper(), CifEntity::CifType(e.value(i)));

        return res;
    };
    static const QMap<QString, CifEntity::CifType> typesByName = initNames();

    return typesByName.contains(name) ? typesByName.value(name)
                                      : typesByName.value(nameForType(CifEntity::CifType::Unknown));
}

} // ns cif
} // ns msc
