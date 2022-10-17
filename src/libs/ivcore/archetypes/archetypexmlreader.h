/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#pragma once

#include "xmlreader.h"
#include "archetypeobject.h"
#include "functionarchetype.h"
#include "interfacearchetype.h"
#include "parameterarchetype.h"
#include "layerarchetype.h"
#include "archetypecommonprops.h"

#include <QObject>
#include <QVariantMap>
#include <QVector>
#include <QPointer>
#include <memory>

class QIODevice;
class QXmlStreamReader;

namespace ivm {

struct CurrentArchetypeObjectHolder {
    void set(ArchetypeObject *object);

    QPointer<ArchetypeObject> getObject() { return m_object; }
    QPointer<FunctionArchetype> getFunctionObject() { return m_function; }
    QPointer<InterfaceArchetype> getInterfaceObject() { return m_iface; }
    QPointer<ParameterArchetype> getParameterObject() { return m_parameter; }

private:
    QPointer<ArchetypeObject> m_object { nullptr };
    QPointer<FunctionArchetype> m_function { nullptr };
    QPointer<InterfaceArchetype> m_iface { nullptr };
    QPointer<ParameterArchetype> m_parameter { nullptr };
};

class ArchetypeXMLReader : public shared::XmlReader
{
    Q_OBJECT
public:
    explicit ArchetypeXMLReader(QObject *parent = nullptr);
    ~ArchetypeXMLReader() override;

    QVector<ArchetypeObject *> parsedObjects() const;

protected:
    void processTagOpen(QXmlStreamReader &xml) override;
    void processTagClose(QXmlStreamReader &xml) override;
    QString rootElementName() const override;

private:
    InterfaceArchetype *createInterface(const QString &objectName, const EntityAttributes &attrs);
    ParameterArchetype *createParameter(const QString &objectName, const EntityAttributes &attrs);

    void addCurrentObject();

private:
    QVector<ArchetypeObject *> m_allObjects {};
    CurrentArchetypeObjectHolder m_currentObject;
};

}
