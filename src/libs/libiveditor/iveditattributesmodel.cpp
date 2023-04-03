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

namespace ive {

struct IVEditAttributesModel::Private
{
    ivm::IVModel *model;
    ObjectType objectType;
    QList<ivm::IVObject*> objects;
    QStringList headerNames;
    QStringList propertyNames;

    template<typename Type>
    void gatherData() {
        for (shared::VEObject *object : model->objects()) {
            if (auto concrete = qobject_cast<Type*>(object)) {
                // Only add interfaces on functions, not function types.
                if constexpr (std::is_same_v<Type, ivm::IVInterface>) {
                    if (concrete->function()->type() != ivm::IVObject::Type::Function) {
                        continue;
                    }
                }
                objects.append(concrete);
            }
        }

        if (!objects.isEmpty()) {
            // Special case for the interfaces. The first column to show is not
            // actually a property, but the name of the function where the
            // interface is attached to. Just store an empty string as "marker".
            if constexpr (std::is_same_v<Type, ivm::IVInterface>) {
                headerNames.append(tr("Function"));
                propertyNames.append(QString());
            }

            shared::PropertyTemplateConfig *config = model->dynPropConfig();
            auto propertyTemplates = config->propertyTemplatesForObject(objects.first());
            for (shared::PropertyTemplate *propertyTemplate : propertyTemplates) {
                if (!propertyTemplate->isVisible() || propertyTemplate->label().isEmpty())
                    continue;
                headerNames.append(propertyTemplate->label());
                propertyNames.append(propertyTemplate->name());
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
    return d->headerNames.size();
}

QVariant IVEditAttributesModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index) || role != Qt::DisplayRole)
        return QVariant();

    ivm::IVObject* object = d->objects.at(index.row());
    const QString key = d->propertyNames.at(index.column());

    // If the key is empty, it's the special case for the interfaces. Return the
    // name of the function where it is attached to.
    if (key.isEmpty()) {
        Q_ASSERT(qobject_cast<ivm::IVInterface*>(object));
        auto interface = qobject_cast<ivm::IVInterface*>(object);
        return interface->function()->entityAttributeValue("name");
    }
    return d->objects.at(index.row())->entityAttributeValue(key);
}

QVariant IVEditAttributesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 0 || section >= d->headerNames.size() || orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    const QString header = d->headerNames.at(section);
    return header.isEmpty() ? d->propertyNames.at(section) : header;
}

}

