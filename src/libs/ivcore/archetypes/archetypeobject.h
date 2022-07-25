/*
   Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "common.h"
#include "entityattribute.h"
#include "ivcommonprops.h"
#include "veobject.h"

#include <QObject>
#include <QVariant>
#include <QVector>
#include <memory>

namespace ivm {
class ArchetypeModel;
struct ArchetypeObjectPrivate;
class ArchetypeObject : public shared::VEObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QVariantList attributes READ attributes)

public:
    enum class Type
    {
        Unknown = 0,
        ParameterArchetype,
        InterfaceArchetype,
        FunctionArchetype
    };
    Q_ENUM(Type)

    explicit ArchetypeObject(const QString &title, const ArchetypeObject::Type type, QObject *parent = nullptr,
            const shared::Id &id = shared::InvalidId);
    virtual ~ArchetypeObject() override;

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

    void setVisible(bool isVisible);
    bool isVisible() const;

    bool postInit() override;
    bool aboutToBeRemoved() override;

    void resetTitle();

    static QString typeToString(Type t);

    QVariantList attributes() const;

    QVariantList generateProperties(bool isProperty) const override;

Q_SIGNALS:
    void titleChanged(const QString &title);

public Q_SLOTS:
    bool setTitle(const QString &title);

protected:
    QList<EntityAttribute> sortedAttributesValues(const EntityAttributes &attributes) override;

private:
    const std::unique_ptr<ArchetypeObjectPrivate> m_privateObject;
};

}
