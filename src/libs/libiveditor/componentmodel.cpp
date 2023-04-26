/*
  Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "componentmodel.h"

#include "common.h"
#include "errorhub.h"
#include "ivobject.h"
#include "ivxmlreader.h"

#include <QDirIterator>

struct ComponentModelPrivate {
    struct Component {
        shared::Id rootId;
        QString componentPath;
        QVector<ivm::IVObject *> objects;
    };

    QString libraryPath;
    QHash<shared::Id, Component> components;
};

ComponentModel::ComponentModel(QObject *parent)
    : QStandardItemModel(parent)
    , d(new ComponentModelPrivate)
{
}

IVObject *ComponentModel::getObject(const shared::Id &id) { }

bool ComponentModel::load(const QString &path)
{
    bool result;
    QDirIterator importableIt(path, QDir::Dirs | QDir::NoDotAndDotDot);
    while (importableIt.hasNext()) {
        result = result
                && loadComponent(importableIt.next() + QDir::separator() + shared::kDefaultInterfaceViewFileName);
    }
    return result;
}

bool ComponentModel::loadComponent(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        /// Just skip other folders mught be located in that library not related to component model
        // shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Invalid path"), path);
        return false;
    }

    shared::ErrorHub::setCurrentFile(path);
    ivm::IVXMLReader parser;
    if (!parser.readFile(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, parser.errorString(), path);
        shared::ErrorHub::clearCurrentFile();
        return false;
    }

    auto objects = parser.parsedObjects();
    ivm::IVObject::sortObjectList(objects);
    if (pathMappings) {
        std::for_each(objects.constBegin(), objects.constEnd(), [pathMappings, &path](ivm::IVObject *ivObj) {
            if (!ivObj->parentObject())
                pathMappings->insert(ivObj->id(), QFileInfo(path).absolutePath());
        });
    }

    model->setExtAttributes(parser.externalAttributes());
    model->addObjects(objects);
    shared::ErrorHub::clearCurrentFile();
    return true;
