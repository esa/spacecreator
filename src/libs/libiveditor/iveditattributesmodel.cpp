/*
   Copyright (C) 2022-2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "iveditattributesmodel.h"

#include "commands/cmdfunctionattrchange.h"
#include "commands/cmdifaceattrchange.h"
#include "commandsstack.h"
#include "ivfunction.h"
#include "ivmodel.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"

#include <QJsonObject>

namespace {

// Stores one entry of the "metadata" needed for each column.
struct Entry {
    QString label;      // Label on the property template. Used as a header name.
    QString property;   // The property name, used as "key" in the objects to get their value.
    QVariant values;    // Default values in the template. For delegates.
    QString validation; // For the QValidator on the delegates.
};


}

namespace ive {

struct IVEditAttributesModel::Private
{
    ObjectType objectType;
    ivm::IVModel *model = nullptr;
    cmd::CommandsStack::Macro *macro = nullptr;
    QList<ivm::IVObject*> objects;
    QStringList functionTypeNames = { QString() }; // Add an initial empty string for "no type".
    QHash<ivm::IVObject*, QStringList> implementationNames;

    QVector<Entry> entries;

    template<typename WantedType>
    void gatherData() {
        for (shared::VEObject *object : model->objects()) {
            // If gathering info for functions, get the function types as well.
            if constexpr (std::is_same_v<WantedType, ivm::IVFunction>) {
                if (&ivm::IVFunctionType::staticMetaObject == object->metaObject()) {
                    functionTypeNames.append(object->entityAttributeValue("name").toString());
                }
            }

            if (auto wanted = qobject_cast<WantedType*>(object)) {
                // Only add _provided_ interfaces on functions, not function types.
                if constexpr (std::is_same_v<WantedType, ivm::IVInterface>) {
                    if (wanted->type() != ivm::IVObject::Type::ProvidedInterface ||
                        wanted->function()->type() != ivm::IVObject::Type::Function) {
                        continue;
                    }
                } else {
                    const QList<EntityAttribute> implementations = wanted->implementations();
                    QStringList results;
                    for (auto implementation : implementations) {
                        results.append(implementation.name());
                    }
                    implementationNames.insert(wanted, results);
                }
                objects.append(wanted);
            }
        }

        // If some objects of the right type found, get also the per-column data.
        if (!objects.isEmpty()) {
            // Special case for the interfaces. The first column to show is not
            // actually a property, but the name of the function where the
            // interface is attached to. Just store an empty string as "marker".
            if constexpr (std::is_same_v<WantedType, ivm::IVInterface>) {
                entries.append(Entry{ tr("Function") });
            }

            shared::PropertyTemplateConfig *config = model->dynPropConfig();
            auto propertyTemplates = config->propertyTemplatesForObject(objects.first());
            for (shared::PropertyTemplate *propertyTemplate : propertyTemplates) {
                if (!propertyTemplate->isVisible() || propertyTemplate->label().isEmpty())
                    continue;

                if (propertyTemplate->name() == ivm::meta::Props::token(ivm::meta::Props::Token::type_language)) {
                    Entry entry { tr("Implementation"), QString() };
                    entries.append(entry);
                    continue;
                }

                Entry entry { propertyTemplate->label(), propertyTemplate->name() };
                entry.validation = propertyTemplate->valueValidatorPattern();

                const QVariant candidate = propertyTemplate->value();

                if (entry.property == ivm::meta::Props::token(ivm::meta::Props::Token::instance_of))
                    entry.values = functionTypeNames;
                else if (!candidate.isValid())
                    entry.values = QString();
                else if (candidate.userType() == QMetaType::QVariantList)
                    entry.values = candidate.toStringList();
                else
                    entry.values = candidate;

                entries.append(entry);

            }
        }
    }
};

IVEditAttributesModel::IVEditAttributesModel(ObjectType objectType, ivm::IVModel *model, shared::cmd::CommandsStackBase::Macro *macro)
    : d(new Private)
{
    d->model = model;
    d->objectType = objectType;
    d->macro = macro;

    if (objectType == Interface)
        d->gatherData<ivm::IVInterface>();
    else
        d->gatherData<ivm::IVFunction>();
}

IVEditAttributesModel::~IVEditAttributesModel()
{
    delete d;
    d = nullptr;
}

QJsonArray IVEditAttributesModel::saveAttributes() const
{
    QJsonArray result;
    for (int row = 0, last = d->objects.size(); row != last; ++row) {
        QJsonObject serialized;
        for (int column = 0, end = d->entries.size(); column != end; ++column) {
            const QString& key = d->entries.at(column).property;
            if (key.isEmpty()) // Skips function name in interfaces.
                continue;
            ivm::IVObject* object = d->objects.at(row);
            const QVariant value = object->entityAttributeValue(key);
            if (!value.isNull())
                serialized.insert(key, QJsonValue::fromVariant(value));
        }
        result.append(serialized);
    }
    return result;
}

void IVEditAttributesModel::loadAttributes(const QJsonArray& elements)
{
    // Make an "index" of keys and object names, to be able to look up row/col.
    QStringList keys, names;
    if (d->objectType == Interface)
        keys.append(QString()); // Interfaces have an extra column for the function where they are.
    for (const Entry& entry : d->entries) {
        if (!entry.property.isEmpty())
            keys.append(entry.property);
    }
    for (ivm::IVObject* object : d->objects) {
        names.append(object->property("name").toString());
    }

    for (int element = 0, last = elements.count(); element < last; ++element) {
        const QJsonValue value = elements.at(element);
        if (!value.isObject()) {
            qWarning("Cannot load JSON: element is not an object");
            continue;
        }
        const QJsonObject object = value.toObject();
        const QString objectName = object.value("name").toString();
        if (objectName.isNull())
            continue;

        const int row = names.indexOf(objectName);
        if (row == -1)
            continue;

        for (auto iterator = object.begin(), end = object.end(); iterator != end; ++iterator) {
            const int column = keys.indexOf(iterator.key());
            if (column == -1 || iterator.key().isEmpty())
                continue;

            setData(index(row, column), iterator.value().toVariant(), ive::IVEditAttributesModel::EditRole);
        }
    }
}

int IVEditAttributesModel::rowCount(const QModelIndex &parent) const
{
    return d->objects.size();
}

int IVEditAttributesModel::columnCount(const QModelIndex &parent) const
{
    return d->entries.size();
}

QVariant IVEditAttributesModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index))
        return QVariant();

    const Entry& entry = d->entries.at(index.column());

    if (role == ValidatorRole)
        return entry.validation;

    ivm::IVObject* object = d->objects.at(index.row());

    if (role == EditRole) {
        if (d->objectType == Function && entry.property.isEmpty()) {
            Q_ASSERT(d->implementationNames.contains(object));
            return d->implementationNames.value(object);
        }
        return entry.values;
    }

    if (role != Qt::DisplayRole && role != DataRole)
        return QVariant();

    const QString key = entry.property;

    // If the key is empty, it's the special case for the interfaces. Return the
    // name of the function where it is attached to.
    if (key.isEmpty()) {
        if (d->objectType == Function) {
            auto function = qobject_cast<ivm::IVFunction*>(object);
            Q_ASSERT(function);
            return function->defaultImplementation();
        } else {
            auto interface = qobject_cast<ivm::IVInterface*>(object);
            Q_ASSERT(interface);
            return interface->function()->entityAttributeValue(QLatin1String("name"));
        }
    }

    return object->entityAttributeValue(key);
}

QVariant IVEditAttributesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 0 || section >= d->entries.size() || orientation != Qt::Horizontal)
        return QVariant();

    if (role == Qt::UserRole)
        return d->entries.at(section).property;
    if (role != Qt::DisplayRole)
        return QVariant();

    const Entry& entry = d->entries.at(section);
    return entry.label.isEmpty() ? entry.property : entry.label;
}

Qt::ItemFlags IVEditAttributesModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags result = QAbstractTableModel::flags(index);

    // If the key is empty and the model is for interfaces, it's the special
    // case of function names, so we don't allow to edit this. For functions is
    // a special case as well (implementations), but that's editable.
    const QString& key = d->entries.at(index.column()).property;
    if (index.isValid() && !(key.isEmpty() && d->objectType == Interface))
        result |= Qt::ItemIsEditable;
    return result;
}

bool IVEditAttributesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!checkIndex(index))
        return false;

    if (role != DataRole && role != EditRole)
        return false;

    if (data(index, role) == value)
        return false;

    const Entry& entry = d->entries.at(index.column());
    const QString& key = entry.property;

    // Special case. Interfaces: function, non editable. Functions, the default
    // implementation to set (done below).
    if (key.isEmpty() && d->objectType == Interface)
        return false;

    if (entry.values.typeId() == QMetaType::QStringList &&
        entry.values.toStringList().indexOf(value.toString()) == -1)
        return false;

    auto object = d->objects.at(index.row());

    bool result = true;
    if (d->objectType == Interface) {
        auto interface = qobject_cast<ivm::IVInterface*>(object);
        Q_ASSERT(interface);
        result = d->macro->push(new cmd::CmdIfaceAttrChange(d->model->dynPropConfig(), interface, key, value));
    } else {
        auto function = qobject_cast<ivm::IVFunction*>(object);
        Q_ASSERT(function);
        const QString attrKey = !key.isEmpty() ? key : ivm::meta::Props::token(ivm::meta::Props::Token::default_implementation);
        result = d->macro->push(new cmd::CmdFunctionAttrChange(d->model->dynPropConfig(), function,
                { EntityAttribute { attrKey, value, EntityAttribute::Type::Attribute } }));
    }
    if (result)
        Q_EMIT dataChanged(index, index, {role});
    return result;
}

}

