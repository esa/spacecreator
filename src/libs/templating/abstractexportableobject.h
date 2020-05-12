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

namespace templating {

/**
 * @brief The AbstractExportableObject class is a base class
 *        for exporting of QObject's successors by the string template engine
 */
class AbstractExportableObject
{
public:
    AbstractExportableObject(const QObject *exportedObject)
        : m_exportedObject(exportedObject)
    {
    }

    virtual ~AbstractExportableObject() = default;

    /**
     * @brief groupName is a logical name of a group of objects which given object belongs to.
     * Exportable objects are groupped by the group name and kept in QVariantList.
     * Then the group name is used in the string template to retrive all groupped objects.
     * @return name of the group
     */
    virtual QString groupName() const = 0;

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

}
