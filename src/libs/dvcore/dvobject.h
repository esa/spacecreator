/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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
#include "dvcommonprops.h"
#include "veobject.h"

#include <QObject>
#include <memory>

namespace dvm {
class DVModel;

struct DVObjectPrivate;
class DVObject : public shared::VEObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QVariantList attributes READ attributes)
    Q_PROPERTY(QVariantList properties READ properties)

public:
    enum class Type
    {
        Unknown = 0,

        Node,
        Partition,
        Function,
        Device,
        Connection,
        Message,
        Bus,

        Board,
        Port,
        SystemFunction,
        SystemInterface,
    };
    Q_ENUM(Type)

    explicit DVObject(const DVObject::Type t, const QString &title = QString(), QObject *parent = nullptr,
            const shared::Id &id = shared::InvalidId);
    virtual ~DVObject() override;

    using shared::VEObject::isEqual;

    QString title() const override;
    QString titleUI() const override;

    DVObject::Type type() const;

    QVector<qint32> coordinates() const override;
    void setCoordinates(const QVector<qint32> &coordinates) override;

    DVObject *parentObject() const override;

    DVModel *model() const;

    bool postInit() override;
    bool aboutToBeRemoved() override;

    virtual bool isEqual(DVObject *other) const;

    QVariantList attributes() const;
    QVariantList properties() const;

    QStringList requestsIDs() const;

    void setRequestsIDs(const QStringList &requestIDs);


Q_SIGNALS:
    void urlChanged(const QString &title);
    void titleChanged(const QString &title);
    void coordinatesChanged(const QVector<qint32> &coordinates);

public Q_SLOTS:
    bool setTitle(const QString &title);
    bool setParentObject(DVObject *parentObject);

protected:
    QVariantList generateProperties(bool isProperty) const override;
    void setAttributeImpl(const QString &name, const QVariant &value, EntityAttribute::Type type) override;

private:
    const std::unique_ptr<DVObjectPrivate> d;
};

inline uint qHash(const DVObject::Type &key, uint seed)
{
    return ::qHash(static_cast<uint>(key), seed);
}

} // namespace dvm

Q_DECLARE_METATYPE(dvm::DVObject *)
