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
#include "ivcoreutils.h"
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
    Q_PROPERTY(QVariantList attributes READ attributes)
    Q_PROPERTY(QVariantList properties READ properties)

public:
    enum class Type
    {
        Unknown = 0,
        Function,
        FunctionType,
        RequiredInterface,
        ProvidedInterface,
        Comment,
        ConnectionGroup,
        Connection,
        InterfaceGroup,
        ConnectionLayer,
        ArchetypeReference,
        ArchetypeLibraryReference,
    };
    Q_ENUM(Type)

    explicit IVObject(const IVObject::Type t, QObject *parent = nullptr, const shared::Id &id = shared::InvalidId);
    virtual ~IVObject() override;

    QString title() const override;
    virtual QString titleUI() const override;

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
    bool isConnectionLayer() const;
    bool isNestedInFunction() const;
    bool isNestedInFunctionType() const;
    bool isNested() const;
    bool isReference() const;
    shared::Id origin() const;
    bool isInstanceDescendant() const;

    QString groupName() const;
    void setGroupName(const QString &groupName);

    IVModel *model() const;

    bool isRootObject() const;

    bool isGrouped() const;

    bool isFixedSystemElement() const;

    /*
     * Visibility is controlled by the EntityAttribute is_visible
     */
    void setVisible(bool isVisible);
    bool isVisible() const;

    bool postInit() override;
    bool aboutToBeRemoved() override;

    void resetTitle();

    //! This sorts the objects on type.
    //! \sa ivm::IVObject::Type
    template<typename C, typename T = std::decay_t<decltype(*std::begin(std::declval<C>()))>,
            typename = std::enable_if_t<std::is_base_of_v<ivm::IVObject, typename std::remove_pointer<T>::type>>>
    static void sortObjectList(C &objects)
    {
        std::stable_sort(std::begin(objects), std::end(objects), [](const T &obj1, const T &obj2) {
            if (utils::nestingLevel(obj1) == utils::nestingLevel(obj2)) {
                return obj1->type() < obj2->type();
            }
            return utils::nestingLevel(obj1) < utils::nestingLevel(obj2);
        });
    }

    //! This sorts the objects on title.
    template<typename C, typename T = std::decay_t<decltype(*std::begin(std::declval<C>()))>,
            typename = std::enable_if_t<std::is_base_of_v<VEObject, typename std::remove_pointer<T>::type>>>
    static void sortObjectListByTitle(C &objects)
    {
        std::stable_sort(std::begin(objects), std::end(objects), [](const T &obj1, const T &obj2) {
            if (utils::nestingLevel(obj1) == utils::nestingLevel(obj2)) {
                return obj1->title() < obj2->title();
            }
            return utils::nestingLevel(obj1) < utils::nestingLevel(obj2);
        });
    }

    static QString typeToString(Type t);

    QVariantList attributes() const;
    QVariantList properties() const;

Q_SIGNALS:
    void urlChanged(const QString &url);
    void titleChanged(const QString &title);
    void coordinatesChanged(const QVector<qint32> &coordinates);
    void visibilityChanged(bool visible);
    void groupChanged(const QString &groupName);

public Q_SLOTS:
    bool setTitle(const QString &title);

protected:
    QVariantList generateProperties(bool isProperty) const override;
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
