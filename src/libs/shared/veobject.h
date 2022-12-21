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

#include <QObject>
#include <memory>

namespace shared {
class VEModel;

struct VEObjectPrivate;
class VEObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(shared::Id id READ id CONSTANT)
    Q_PROPERTY(QString name READ title)

public:
    explicit VEObject(const shared::Id &id = shared::InvalidId, QObject *parent = nullptr);
    virtual ~VEObject() override;

    shared::Id id() const;
    void setModel(VEModel *model);

    virtual QString title() const = 0;
    virtual QString titleUI() const = 0;

    virtual QVector<qint32> coordinates() const = 0;
    virtual void setCoordinates(const QVector<qint32> &coordinates) = 0;

    void setEntityProperty(const QString &name, const QVariant &value);
    void setEntityAttribute(const QString &name, const QVariant &value);
    void setEntityAttribute(const EntityAttribute &attribute);
    void setEntityAttributes(const EntityAttributes &attributes);

    // "attributes" - payload data in the opening XML tag,
    // like "name" and "kind" below:
    // <Required_Interface name="run_forrest" kind="Sporadic">
    EntityAttributes entityAttributes() const;
    EntityAttribute entityAttribute(const QString &name) const;
    /// TODO: template
    template<typename T>
    T entityAttributeValue(const QString &name, const T &defaultValue = T()) const
    {
        const EntityAttribute attr = entityAttribute(name);
        return attr.isNull() ? defaultValue : attr.value<T>();
    }

    QVariant entityAttributeValue(const QString &name, const QVariant &defaultValue = QVariant()) const;

    void removeEntityAttribute(const QString &attributeName);
    bool hasEntityAttribute(const QString &attributeName, const QVariant &value = QVariant()) const;
    bool hasEntityAttribute(const EntityAttribute &attribute) const;
    bool hasEntityAttributes(const EntityAttributes &attrs) const;

    virtual VEObject *parentObject() const;
    QVector<VEObject *> descendants() const;

    /// Return all children of type T
    template<typename T>
    QList<T *> typedChildren() const
    {
        QList<T *> typedChildren;
        for (QObject *obj : children()) {
            if (auto child = qobject_cast<T *>(obj)) {
                typedChildren.append(child);
            }
        }
        return typedChildren;
    }

    VEModel *model() const;
    virtual bool postInit() = 0;
    virtual bool aboutToBeRemoved() = 0;

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
    static bool isNullCoordinates(const QVariant &coordinates);

    virtual bool isEqual(const VEObject *other) const;

    virtual QVariantList generateProperties(bool isProperty) const = 0;

    // Perform updates/actions directly before the data is saved
    virtual void updateForExport() { }

    void setAttributeExportable(const QString &attrName, bool isExportable);

    template<typename T>
    static bool hasDuplicates(QVector<T *> &objects)
    {
        for (int i = 0; i < objects.size() - 1; ++i) {
            for (int j = i + 1; j < objects.size(); ++j) {
                if (*objects[i] == *objects[j]) {
                    return true;
                }
            }
        }

        return false;
    }

Q_SIGNALS:
    void attributeChanged(const QString &name);

public Q_SLOTS:
    bool setParentObject(shared::VEObject *parentObject);

protected:
    virtual QList<EntityAttribute> sortedAttributesValues(const EntityAttributes &attrs);
    virtual void setAttributeImpl(const QString &name, const QVariant &value, EntityAttribute::Type type);
    void clearAttributes();

private:
    const std::unique_ptr<VEObjectPrivate> d;
};

QString toString(VEObject *object);

}

Q_DECLARE_METATYPE(shared::VEObject *)
