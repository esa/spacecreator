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

#pragma once

#include <QObject>
#include <QPointer>

namespace taste3 {
namespace templating {

/**
 * @brief The GenericExportedObject class is a generic class
 *        to export QObject's successors by the string template engine
 */
class GenericExportedObject
{
public:
    GenericExportedObject(const QObject *object)
        : m_exportedObject(object) {}

protected:
    /**
     *  @brief exportedObject() casts the internal exported object to required type
     */
    template<class T>
    const T *exportedObject() const
    {
        const T *o = qobject_cast<const T *>(m_exportedObject);
        Q_ASSERT(o);
        return o;
    }
private:
    QPointer<const QObject> m_exportedObject;
};

} // ns templating
} // ns taste3

#define DECLARE_EXPORTED_TYPE(TYPE) \
    Q_DECLARE_METATYPE(taste3::templating::TYPE) \
    Q_DECLARE_TYPEINFO(taste3::templating::TYPE, Q_MOVABLE_TYPE);
