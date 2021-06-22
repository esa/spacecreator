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

#include "common.h"
#include "entityattribute.h"
#include "ivcommonprops.h"
#include "veobject.h"

#include <QObject>
#include <QVariant>
#include <QVector>
#include <memory>

namespace ivm {
class IVModel;
struct IVObjectPrivate;
class IVObject : public shared::VEObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    enum class Type
    {
        Unknown = 0,

        Function,
        FunctionType,
        RequiredInterface,
        ProvidedInterface,
        InterfaceGroup,
        Comment,
        Connection,
        ConnectionGroup,
    };
    Q_ENUM(Type)

    explicit IVObject(const IVObject::Type t, const QString &title = QString(), QObject *parent = nullptr,
            const shared::Id &id = shared::InvalidId);
    virtual ~IVObject() override;

    QString title() const override;
    QString titleUI() const override;

    IVObject::Type type() const;

    QVector<qint32> coordinates() const override;
    void setCoordinates(const QVector<qint32> &coordinates) override;
    meta::Props::Token coordinatesType() const;

    QStringList path() const;
    static QStringList path(const IVObject *obj);

    IVObject *parentObject() const override;
    bool isFunction() const;
    bool isFunctionType() const;
    bool isInterfaceGroup() const;
    bool isRequiredInterface() const;
    bool isProvidedInterface() const;
    bool isInterface() const;
    bool isComment() const;
    bool isConnection() const;
    bool isConnectionGroup() const;
    bool isNestedInFunction() const;
    bool isNestedInFunctionType() const;
    bool isNested() const;

    QString groupName() const;
    void setGroupName(const QString &groupName);

    IVModel *model() const;

    bool isRootObject() const;

    bool isGrouped() const;

    void setVisible(bool isVisible);
    bool isVisible() const;

    bool postInit() override;
    bool aboutToBeRemoved() override;

    static void sortObjectList(QList<ivm::IVObject *> &objects);

Q_SIGNALS:
    void titleChanged(const QString &title);
    void coordinatesChanged(const QVector<qint32> &coordinates);
    void visibilityChanged(bool visible);
    void groupChanged(const QString &groupName);

public Q_SLOTS:
    bool setTitle(const QString &title);

protected:
    void setAttributeImpl(const QString &name, const QVariant &value, EntityAttribute::Type type) override;
    QList<EntityAttribute> sortedAttributesValues(const EntityAttributes &attributes) override;

private:
    const std::unique_ptr<IVObjectPrivate> d;
};

inline uint qHash(const IVObject::Type &key, uint seed)
{
    return ::qHash(static_cast<uint>(key), seed);
}

}
