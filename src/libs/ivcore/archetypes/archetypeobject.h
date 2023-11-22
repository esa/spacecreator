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

#include "common.h"
#include "entityattribute.h"
#include "archetypecommonprops.h"
#include "veobject.h"

#include <QObject>
#include <QVariant>
#include <QVector>
#include <memory>

namespace ivm {
class ArchetypeModel;

/*!
 * @namespace ivm
 * @brief The Archetype object code
 */
class ArchetypeObject : public shared::VEObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QVariantList attributes READ attributes)

public:
    enum class Type
    {
        Unknown = 0,
        LayerArchetype,
        ParameterArchetype,
        InterfaceArchetype,
        FunctionArchetype,
        ArchetypeLibrary
    };
    Q_ENUM(Type)

    explicit ArchetypeObject(const QString &title, const ArchetypeObject::Type type, QObject *parent = nullptr,
            const shared::Id &id = shared::InvalidId);
    ~ArchetypeObject();

    QString title() const override;
    QString titleUI() const override;

    ArchetypeObject::Type type() const;

    QVector<qint32> coordinates() const override;
    void setCoordinates(const QVector<qint32> &coordinates) override;

    ArchetypeObject *parentObject() const override;
    bool isParameterArchetype() const;
    bool isInterfaceArchetype() const;
    bool isFunctionArchetype() const;

    ArchetypeModel *model() const;

    bool isRootObject() const;

    /**
     * @brief Returns list of attributes for using in string templates.
     * @return list of attributes.
     */
    QVariantList attributes() const;

    /**
     * @brief Returns list of properties for using in string templates.
     * @return list of properties.
     */
    QVariantList properties() const;

    /**
     * @brief generateProperties generates a variant list sorted by meta::ArchetypeProps::Token.
     * @param isProperty determines if result should be generated for properties or attributes
     * @return sorted QVariantList which can be used in string templates
     */
    virtual QVariantList generateProperties(bool isProperty) const override;

    bool postInit() override;
    bool aboutToBeRemoved() override;

    void resetTitle();

    static QString typeToString(Type t);

    QStringList requestsIDs() const override;

    void setRequestsIDs(const QStringList &requestIDs) override;

Q_SIGNALS:
    void titleChanged(const QString &title);

public Q_SLOTS:
    bool setTitle(const QString &title);

protected:
    QList<EntityAttribute> sortedAttributesValues(const EntityAttributes &attributes) override;

private:
    ArchetypeObject::Type m_type;
};

}
