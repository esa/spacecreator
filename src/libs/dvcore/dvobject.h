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
#include "prop.h"

#include <QObject>
#include <QVariant>
#include <QVector>
#include <memory>

namespace dvm {

class DVModel;

struct DVObjectPrivate;
class DVObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(shared::Id id READ id CONSTANT)

public:
    enum class Type
    {
        Unknown = 0,
        Node,
        Processor,
        Partition,
        Binding,
        Device,
        Bus,
        Link
    };
    Q_ENUM(Type)

    explicit DVObject(const DVObject::Type t, const QString &title = QString(), QObject *parent = nullptr,
            const shared::Id &id = shared::InvalidId);
    virtual ~DVObject();

    QString title() const;
    shared::Id id() const;

    DVObject::Type deployType() const;

    QVector<qint32> coordinates() const;
    void setCoordinates(const QVector<qint32> &coordinates);

    DVObject *parentObject() const;

    // "attributes" - payload data in the opening XML tag,
    QHash<QString, QVariant> attrs() const;
    void setAttrs(const QHash<QString, QVariant> &attrs);
    QVariant attr(const QString &name, const QVariant &defaultValue = QVariant()) const;
    virtual void setAttr(const QString &name, const QVariant &val);
    void removeAttr(const QString &name);
    bool hasAttribute(const QString &attributeName, const QVariant &value) const;
    bool hasAttributes(const QHash<QString, QVariant> &attrs) const;

    // "properties" - XML children <Property>
    QHash<QString, QVariant> props() const;
    void setProps(const QHash<QString, QVariant> &props);
    QVariant prop(const QString &name, const QVariant &defaultValue = QVariant()) const;
    virtual void setProp(const QString &name, const QVariant &val);
    void removeProp(const QString &name);
    bool hasProperty(const QString &propertyName, const QVariant &value = QVariant()) const;
    bool hasProperties(const QHash<QString, QVariant> &props) const;

    void setObjectsModel(DVModel *model);
    DVModel *objectsModel() const;

    template<class T>
    inline T as()
    {
        return qobject_cast<T>(this);
    }

    template<class T>
    inline const T as() const
    {
        return qobject_cast<const T>(this);
    }

    static QVector<qint32> coordinatesFromString(const QString &strCoordinates);
    static QString coordinatesToString(const QVector<qint32> &coordinates);

Q_SIGNALS:
    void titleChanged(const QString &title);
    void coordinatesChanged(const QVector<qint32> &coordinates);
    void attributeChanged(const QString &name);
    void propertyChanged(const QString &name);
    void attributeChanged(meta::Props::Token attr = meta::Props::Token::Unknown);
    void propertyChanged(meta::Props::Token prop = meta::Props::Token::Unknown);

public Q_SLOTS:
    bool setTitle(const QString &title);
    bool setParentObject(DVObject *parentObject);

private:
    const std::unique_ptr<DVObjectPrivate> d;
};

inline uint qHash(const DVObject::Type &key, uint seed)
{
    return ::qHash(static_cast<uint>(key), seed);
}

} // namespace deploy
