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
#include "ivconnection.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivobject.h"
#include "ivpropertytemplateconfig.h"
#include "ivxmlreader.h"

#include <QDirIterator>
#include <QFileSystemWatcher>

namespace ive {

struct ComponentModelPrivate {
    struct Component {
        ~Component() { qDeleteAll(objects); }

        QList<shared::Id> rootIds;
        QString componentPath;
        QVector<ivm::IVObject *> objects;
    };

    QString modelName;
    QString libraryPath;
    QHash<shared::Id, QSharedPointer<Component>> components;
    QFileSystemWatcher watcher;
};

ComponentModel::ComponentModel(const QString &modelName, QObject *parent)
    : QStandardItemModel(parent)
    , d(new ComponentModelPrivate)
{
    d->modelName = modelName;
}

QString ComponentModel::objectPath(const shared::Id &id) const
{
    auto it = d->components.constFind(id);
    if (it != d->components.constEnd())
        return (*it)->componentPath;

    return {};
}

QString ComponentModel::libraryPath() const
{
    return d->libraryPath;
}

ivm::IVObject *ComponentModel::getObject(const shared::Id &id)
{
    auto it = d->components.constFind(id);
    if (it != d->components.constEnd()) {
        auto objIt = std::find_if((*it)->objects.constBegin(), (*it)->objects.constEnd(),
                [&id](ivm::IVObject *obj) { return obj->id() == id; });
        if (objIt != (*it)->objects.constEnd())
            return *objIt;
    }
    return nullptr;
}

void ComponentModel::load(const QString &path)
{
    clear();
    d->libraryPath = path;
    d->components.clear();

    auto headerItem = new QStandardItem(d->modelName);
    headerItem->setTextAlignment(Qt::AlignCenter);
    setHorizontalHeaderItem(0, headerItem);

    QDirIterator importableIt(path, QDir::Dirs | QDir::NoDotAndDotDot);
    while (importableIt.hasNext()) {
        if (auto item = loadComponent(
                    importableIt.next() + QDir::separator() + shared::kDefaultInterfaceViewFileName)) {
            appendRow(item);
        }
    }
}

static inline QStandardItem *processObject(ivm::IVObject *ivObject)
{
    if (!ivObject)
        return nullptr;

    QStandardItem *item = new QStandardItem;
    item->setEditable(false);
    item->setDragEnabled(true);
    item->setData(ivObject->id(), ComponentModel::IdRole);
    item->setData(QVariant::fromValue(ivObject->type()), ComponentModel::TypeRole);

    QString title = ivm::IVNameValidator::decodeName(ivObject->type(), ivObject->title());
    QPixmap pix;
    QFont font;
    QPixmap dragPix;
    switch (ivObject->type()) {
    case ivm::IVObject::Type::Unknown:
        break;
    case ivm::IVObject::Type::RequiredInterface: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/ri.svg")).pixmap(16, 16);
        pix = icon;
    } break;

    case ivm::IVObject::Type::ProvidedInterface: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/pi.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::Connection: {
        static const QPixmap iconHidden =
                QIcon(QLatin1String(":/toolbar/icns/connection.svg")).pixmap(16, 16, QIcon::Disabled);
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/connection.svg")).pixmap(16, 16);
        pix = ivObject->isGrouped() ? iconHidden : icon;

        if (auto connectionPtr = qobject_cast<ivm::IVConnection *>(ivObject)) {
            const QString sourceName =
                    ivm::IVNameValidator::decodeName(ivm::IVObject::Type::Function, connectionPtr->sourceName());
            const QString sourceInterfaceName = ivm::IVNameValidator::decodeName(
                    ivm::IVObject::Type::RequiredInterface, connectionPtr->sourceInterfaceName());
            const QString targetName =
                    ivm::IVNameValidator::decodeName(ivm::IVObject::Type::Function, connectionPtr->targetName());
            const QString targetInterfaceName = ivm::IVNameValidator::decodeName(
                    ivm::IVObject::Type::ProvidedInterface, connectionPtr->targetInterfaceName());
            title = QStringLiteral("%1.%2 -> %3.%4")
                            .arg(sourceName, sourceInterfaceName, targetName, targetInterfaceName);
        }
    } break;
    case ivm::IVObject::Type::Function: {
        static const QPixmap dragIcon = QIcon(QLatin1String(":/toolbar/icns/function.svg")).pixmap(128, 128);
        dragPix = dragIcon;

        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/function.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::FunctionType: {
        static const QPixmap dragIcon = QIcon(QLatin1String(":/toolbar/icns/function_type.svg")).pixmap(128, 128);
        dragPix = dragIcon;

        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/function_type.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::Comment: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/comment.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::ConnectionGroup: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/connection_group.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::ConnectionLayer: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/connection_layer.svg")).pixmap(16, 16);
        pix = icon;
    } break;

    default:
        break;
    }

    QColor color;
    if (ivObject->isGrouped()) {
        color = QColor(Qt::darkGray);
        font.setItalic(true);
    } else if (!ivObject->isVisible()) {
        color = QColor(Qt::lightGray);
        font.setItalic(true);
        font.setWeight(QFont::Light);
    } else {
        color = QColor(Qt::black);
    }
    item->setData(dragPix, ComponentModel::CursorPixmapRole);
    item->setData(color, Qt::ForegroundRole);
    item->setData(font, Qt::FontRole);
    item->setData(title, Qt::DisplayRole);
    item->setData(pix, Qt::DecorationRole);

    for (auto child : ivObject->descendants()) {
        if (auto childItem = processObject(child->as<ivm::IVObject *>())) {
            item->appendRow(childItem);
        }
    }
    return item;
}

QStandardItem *ComponentModel::loadComponent(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        /// Just skip other folders mught be located in that library not related to component model
        // shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Invalid path"), path);
        return nullptr;
    }

    shared::ErrorHub::setCurrentFile(path);
    ivm::IVXMLReader parser;
    if (!parser.readFile(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, parser.errorString(), path);
        shared::ErrorHub::clearCurrentFile();
        return nullptr;
    }

    ivm::IVModel model(ivm::IVPropertyTemplateConfig::instance());
    model.initFromObjects(parser.parsedObjects(), parser.externalAttributes());

    const QHash<shared::Id, shared::VEObject *> &modelObjects = model.objects();
    QList<shared::Id> ids;
    QVector<ivm::IVObject *> objects;
    std::for_each(modelObjects.constBegin(), modelObjects.constEnd(), [&objects, &ids](shared::VEObject *veObj) {
        if (!veObj->parentObject()) {
            objects.append(veObj->as<ivm::IVObject *>());
            ids.append(veObj->id());
        }
    });
    ivm::IVObject::sortObjectList(objects);
    QList<QStandardItem *> items;
    std::for_each(objects.constBegin(), objects.constEnd(), [&items](ivm::IVObject *ivObj) {
        if (auto item = processObject(ivObj)) {
            items << item;
        }
    });
    shared::ErrorHub::clearCurrentFile();
    std::for_each(objects.begin(), objects.end(), [&path, &model](ivm::IVObject *ivObj) {
        if (ivObj->parent() == &model) {
            ivObj->setParent(nullptr);
        }
    });

    if (items.isEmpty())
        return nullptr;

    QSharedPointer<ComponentModelPrivate::Component> component { new ComponentModelPrivate::Component };
    component->componentPath = path;
    component->objects = objects;
    component->rootIds = ids;
    for (auto id : qAsConst(ids))
        d->components.insert(id, component);

    if (items.size() == 1)
        return items.front();

    QStandardItem *item = new QStandardItem;
    item->appendRows(items);
    return item;
}

} // namespace ive
