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

#include "ivmodel.h"
#include "ivfunction.h"

namespace ive {

struct IVEditAttributesModel::Private
{
    ivm::IVModel *model;
    ObjectType objectType;
    QList<ivm::IVObject*> objects;
    QStringList attributeNames;

    template<typename Type>
    void gatherData() {
        for (shared::VEObject *object : model->objects()) {
            if (auto interface = qobject_cast<Type*>(object)) {
                objects.append(interface);
            }
        }
        if (!objects.isEmpty()) {
            attributeNames.append("name"); // Ensure it's added at the start
            for (auto attribute : objects.first()->entityAttributes()) {
                if (attribute.type() == EntityAttribute::Type::Attribute) {
                    const QString name = attribute.name();
                    if (name != QLatin1String("name"))
                        attributeNames.append(name);
                }
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
    return d->attributeNames.size();
}

QVariant IVEditAttributesModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index) || role != Qt::DisplayRole)
        return QVariant();

    const QString key = d->attributeNames.at(index.column());
    return d->objects.at(index.row())->entityAttributeValue(key);
}

QVariant IVEditAttributesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 0 || section >= d->attributeNames.size() || orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    return d->attributeNames.at(section);
}

}

