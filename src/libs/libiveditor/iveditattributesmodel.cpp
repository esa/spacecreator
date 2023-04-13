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

#include "ivfunction.h"
#include "ivmodel.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"

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
    ivm::IVModel *model;
    ObjectType objectType;
    QList<ivm::IVObject*> objects;
    QStringList functionTypeNames = { QString() }; // Add an initial empty string for "no type".

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

                Entry entry { propertyTemplate->label(), propertyTemplate->name() };
                entry.validation = propertyTemplate->valueValidatorPattern();

                const QVariant candidate = propertyTemplate->value();

                if (entry.property == QLatin1String("instance_of"))
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

IVEditAttributesModel::IVEditAttributesModel(ivm::IVModel *model, ObjectType objectType)
    : d(new Private)
{
    d->model = model;
    d->objectType = objectType;

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
    if (role == EditRole)
        return entry.values;

    if (role != Qt::DisplayRole && role != DataRole)
        return QVariant();

    ivm::IVObject* object = d->objects.at(index.row());
    const QString key = entry.property;

    // If the key is empty, it's the special case for the interfaces. Return the
    // name of the function where it is attached to.
    if (key.isEmpty()) {
        auto interface = qobject_cast<ivm::IVInterface*>(object);
        Q_ASSERT(interface);
        return interface->function()->entityAttributeValue(QLatin1String("name"));
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

    // If the key is empty, it's the special case for the interfaces. We don't allow to edit this.
    const QString& key = d->entries.at(index.column()).property;
    if (index.isValid() && !key.isEmpty())
        result |= Qt::ItemIsEditable;
    return result;
}

bool IVEditAttributesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!checkIndex(index))
        return false;

    if (role != DataRole && role != EditRole)
        return false;

    const Entry& entry = d->entries.at(index.column());
    const QString& key = entry.property;
    if (key.isEmpty())
        return false;

    if (entry.values.typeId() == QMetaType::QStringList &&
        entry.values.toStringList().indexOf(value.toString()) == -1)
        return false;

    d->objects.at(index.row())->setEntityAttribute(key, value);
    return true;
}

}

