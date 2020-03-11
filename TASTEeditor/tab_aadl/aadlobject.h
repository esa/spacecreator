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
#include "tab_aadl/aadlcommonprops.h"

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
    Q_PROPERTY(common::Id id READ id)
    Q_PROPERTY(QVariantList properties READ templateProperties) //!< Property list for string templates
    Q_PROPERTY(QVariantList attributes READ templateAttributes) //!< Attribute list for string templates

public:
    enum class Type
    {
        Unknown = 0,

        Function,
        FunctionType,
        Interface,
        Comment,
        Connection,
    };
    Q_ENUM(Type)

    explicit AADLObject(const AADLObject::Type t, const QString &title = QString(), QObject *parent = nullptr,
                        const common::Id &id = common::InvalidId);
    virtual ~AADLObject();

    QString title() const;
    common::Id id() const;

    AADLObject::Type aadlType() const;

    QVector<qint32> coordinates() const;
    void setCoordinates(const QVector<qint32> &coordinates);

    AADLObject *parentObject() const;
    bool isFunction() const;
    bool isFunctionType() const;
    bool isInterface() const;
    bool isComment() const;
    bool isConnection() const;
    bool isNestedInFunction() const;
    bool isNestedInFunctionType() const;
    bool isNested() const;

    // "attributes" - payload data in the opening XML tag,
    // like "name" and "kind" below:
    // <Required_Interface name="run_forrest" kind="SPORADIC_OPERATION">

    QHash<QString, QVariant> attrs() const;
    QVariantList templateAttributes() const;
    void setAttrs(const QHash<QString, QVariant> &attrs);
    QVariant attr(const QString &name, const QVariant &defaultValue = QVariant()) const;
    virtual void setAttr(const QString &name, const QVariant &val);
    void removeAttr(const QString &name);

    // "properties" - XML children <Property>
    QHash<QString, QVariant> props() const;
    QVariantList templateProperties() const;
    void setProps(const QHash<QString, QVariant> &props);
    QVariant prop(const QString &name, const QVariant &defaultValue = QVariant()) const;
    virtual void setProp(const QString &name, const QVariant &val);
    void removeProp(const QString &name);

    void setObjectsModel(AADLObjectsModel *model);
    AADLObjectsModel *objectsModel() const;

    bool isRootObject() const;

    virtual void postInit();

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

Q_SIGNALS:
    void titleChanged(const QString &title);
    void coordinatesChanged(const QVector<qint32> &coordinates);
    void attributeChanged(taste3::aadl::meta::Props::Token attr = taste3::aadl::meta::Props::Token::Unknown) const;
    void propertyChanged(taste3::aadl::meta::Props::Token prop = taste3::aadl::meta::Props::Token::Unknown) const;

public Q_SLOTS:
    bool setTitle(const QString &title);
    bool setParentObject(AADLObject *parentObject);

private:
    const std::unique_ptr<AADLObjectPrivate> d;
    QVector<qint32> coordinatesFromString(const QString &strCoordinates) const;
    QString coordinatesToString(const QVector<qint32> &coordinates) const;
};

} // ns aadl
} // ns taste3
