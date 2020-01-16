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

#include "app/common.h"

#include <QObject>
#include <QVariant>
#include <memory>

namespace taste3 {
namespace aadl {

struct AADLObjectPrivate;
class AADLObjectsModel;
class AADLObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(common::Id id READ id WRITE setId NOTIFY idChanged)

public:
    enum class AADLObjectType
    {
        AADLUnknown = 0,

        AADLFunctionType,
        AADLFunction,
        AADLIface,
        AADLComment,
        AADLConnection,
    };
    Q_ENUM(AADLObjectType)

    explicit AADLObject(const QString &title = QString(), QObject *parent = nullptr);
    virtual ~AADLObject();

    QString title() const;
    common::Id id() const;

    virtual AADLObjectType aadlType() const = 0;

    QVector<qint32> coordinates() const;
    void setCoordinates(const QVector<qint32> &coordinates);

    AADLObject *parentObject() const;

    // "attributes" - payload data in the opening XML tag,
    // like "name" and "kind" below:
    // <Required_Interface name="run_forrest" kind="SPORADIC_OPERATION">

    QHash<QString, QVariant> attrs() const;
    void setAttrs(const QHash<QString, QVariant> &attrs);
    QVariant attr(const QString &name, const QVariant &defaultValue = QVariant()) const;
    virtual void setAttr(const QString &name, const QVariant &val);
    void removeAttr(const QString &name);

    // "properties" - XML children <Property>
    QHash<QString, QVariant> props() const;
    void setProps(const QHash<QString, QVariant> &props);
    QVariant prop(const QString &name, const QVariant &defaultValue = QVariant()) const;
    void setProp(const QString &name, const QVariant &val);
    void removeProp(const QString &name);

    void setObjectsModel(AADLObjectsModel *model);
    AADLObjectsModel *objectsModel() const;

Q_SIGNALS:
    void titleChanged(const QString &title);
    void idChanged(const taste3::common::Id &id);
    void coordinatesChanged(const QVector<qint32> &coordinates);
    void attributesChanged();
    void propertiesChanged();

public Q_SLOTS:
    bool setTitle(const QString &title);
    bool setId(const common::Id &id);
    bool setParentObject(AADLObject *parentObject);

private:
    const std::unique_ptr<AADLObjectPrivate> d;
    QVector<qint32> coordinatesFromString(const QString &strCoordinates) const;
    QString coordinatesToString(const QVector<qint32> &coordinates) const;
};

} // ns aadl
} // ns taste3
