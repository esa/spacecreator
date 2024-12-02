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

#include "ivcomponentmodel.h"

#include "ivconnectiongroup.h"
#include "ivfunctiontype.h"

#include <QDirIterator>
#include <QFileInfo>
#include <QSharedPointer>
#include <errorhub.h>
#include <ivcomponentlibrary.h>
#include <ivconnection.h>
#include <ivmodel.h>
#include <ivnamevalidator.h>
#include <ivpropertytemplateconfig.h>
#include <ivxmlreader.h>

namespace ive {

IVComponentModel::IVComponentModel(Type type, const QString &modelName, QObject *parent)
    : m_type(type)
{
    auto path = (type == ComponentLibrary) ? shared::componentsLibraryPath() : shared::sharedTypesPath();
    m_compLibrary = std::make_unique<ivm::IVComponentLibrary>(path, modelName);
    connect(m_compLibrary.get(), &ivm::IVComponentLibrary::componentUpdated, [this](const shared::Id &id) {
        if (auto item = itemById(id)) {
            item->setData(true, shared::ComponentRoles::UpdateRole);
            reloadComponent(id);
        }
    });
    connect(m_compLibrary.get(), &ivm::IVComponentLibrary::componentsToBeLoaded,
            [this](const QSet<QString> &componentsPaths) {
                for (const QString &path : std::as_const(componentsPaths)) {
                    if (auto item = loadComponent(path)) {
                        appendRow(item);
                    }
                }
            });
    connect(m_compLibrary.get(), &ivm::IVComponentLibrary::componentsToBeRemovedFromModel,
            [this](const QList<shared::Id> &componentsPaths) {
                for (const shared::Id &id : std::as_const(componentsPaths)) {
                    removeComponentFromModel(id);
                }
            });
    connect(this, &QAbstractItemModel::rowsAboutToBeRemoved, this,
            [this](const QModelIndex &parent, int first, int last) {
                if (parent == indexFromItem(invisibleRootItem())) {
                    for (auto idx = first; idx <= last; ++idx) {
                        const shared::Id id = index(idx, 0, parent).data(shared::ComponentRoles::IdRole).toUuid();
                        m_compLibrary->removeComponent(id);
                    }
                }
            });
    connect(m_compLibrary.get(), &ivm::IVComponentLibrary::componentExported, [this](const QString &filepath, bool ok) {
        if (ok) {
            if (auto item = loadComponent(filepath)) {
                appendRow(item);
            }
        }
    });
}

ivm::IVObject *IVComponentModel::getObject(const shared::Id &id)
{
    auto comp = m_compLibrary->component(id);

    if (!comp.isNull()) {
        auto objIt = std::find(comp->rootIds.constBegin(), comp->rootIds.constEnd(), id);
        if (!objIt->isNull() && objIt != comp->rootIds.constEnd())
            return comp->model->getObject(id);
    }
    return nullptr;
}

void IVComponentModel::removeComponent(const shared::Id &id)
{
    removeComponentFromModel(id);
    m_compLibrary->removeComponent(id);
}

QString IVComponentModel::componentPath(const shared::Id &id)
{
    return m_compLibrary->componentPath(id);
}

QStringList IVComponentModel::asn1Files(const shared::Id &id) const
{
    return m_compLibrary->asn1Files(id);
}

QString IVComponentModel::libraryPath() const
{
    return m_compLibrary->libraryPath();
}

void IVComponentModel::loadAvailableComponents()
{
    clear();

    auto headerItem = new QStandardItem(m_compLibrary->modelName());
    headerItem->setTextAlignment(Qt::AlignCenter);
    setHorizontalHeaderItem(0, headerItem);

    QDirIterator importableIt(m_compLibrary->libraryPath(), QDir::Dirs | QDir::NoDotAndDotDot);
    while (importableIt.hasNext()) {
        if (auto item = loadComponent(
                    importableIt.next() + QDir::separator() + shared::kDefaultInterfaceViewFileName)) {
            appendRow(item);
        }
    }
}

bool IVComponentModel::exportComponent(const QString &targetPath, const QList<ivm::IVObject *> objects,
        const QString &projectDir, QStringList asn1FilesPaths, QStringList externAsns,
        ivm::ArchetypeModel *archetypesModel)
{
    return m_compLibrary->exportComponent(targetPath, objects, projectDir, asn1FilesPaths, externAsns, archetypesModel);
}
void IVComponentModel::unWatchComponentPath(const QString &componentPath)
{
    m_compLibrary->unWatchComponent(componentPath);
}

QSharedPointer<ivm::IVComponentLibrary::Component> IVComponentModel::component(const shared::Id &id) const
{
    return m_compLibrary->component(id);
}

QList<shared::Id> IVComponentModel::componentIDs() const
{
    return m_compLibrary->componentsIds();
}

QStandardItem *IVComponentModel::processObject(ivm::IVObject *ivObject)
{
    if (!ivObject || ivObject->type() == ivm::IVObject::Type::InterfaceGroup) {
        return nullptr;
    }

    QStandardItem *item = new QStandardItem;
    item->setEditable(false);
    item->setDragEnabled(true);
    item->setData(ivObject->id(), shared::ComponentRoles::IdRole);
    item->setData(QVariant::fromValue(ivObject->type()), shared::ComponentRoles::TypeRole);
    if (m_type == IVComponentModel::Type::ComponentLibrary) {
        item->setData(QVariant::fromValue(shared::DropData::Type::ImportableType), shared::ComponentRoles::DropRole);
    } else if (m_type == IVComponentModel::Type::SharedTypesLibrary) {
        item->setData(
                QVariant::fromValue(shared::DropData::Type::InstantiatableType), shared::ComponentRoles::DropRole);
    }

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
    item->setData(dragPix, shared::ComponentRoles::CursorPixmapRole);
    item->setData(color, Qt::ForegroundRole);
    item->setData(font, Qt::FontRole);
    item->setData(title, Qt::DisplayRole);
    item->setData(pix, Qt::DecorationRole);
    item->setSelectable(!ivObject->parentObject());

    if (auto fn = ivObject->as<ivm::IVFunctionType *>()) {
        for (ivm::IVObject *child : fn->children()) {
            if (QStandardItem *childItem = processObject(child)) {
                item->appendRow(childItem);
            }
        }
    } else if (auto conGroup = ivObject->as<ivm::IVConnectionGroup *>()) {
        for (ivm::IVConnection *connection : conGroup->groupedConnections()) {
            if (QStandardItem *childItem = processObject(connection)) {
                item->appendRow(childItem);
            }
        }
    }
    return item;
}
QStandardItem *IVComponentModel::itemFromComponent(QSharedPointer<ivm::IVComponentLibrary::Component> component)
{
    if (!component.isNull()) {
        QVector<ivm::IVObject *> objects = m_compLibrary->rootObjects(component->model->ivobjects().values());
        ivm::IVObject::sortObjectList(objects);

        QList<QStandardItem *> items;
        std::for_each(objects.constBegin(), objects.constEnd(), [this, &items](ivm::IVObject *ivObj) {
            if (auto item = processObject(ivObj)) {
                items << item;
            }
        });
        if (items.size() == 1) {
            return items.front();
        }

        QStandardItem *item = new QStandardItem;
        item->appendRows(items);
        return item;
    }
    return nullptr;
}

QStandardItem *IVComponentModel::itemById(const shared::Id &id)
{
    for (int idx = 0; idx < rowCount(); ++idx) {
        if (item(idx)->data(shared::ComponentRoles::IdRole) == id) {
            return item(idx);
        }
    }
    return nullptr;
}

void IVComponentModel::reloadComponent(const shared::Id &id)
{
    auto item = itemById(id);
    if (!item)
        return;

    if (!item->index().isValid())
        return;

    const int row = item->index().row();
    const QString path = m_compLibrary->componentPath(id);
    removeComponent(id);
    if ((item = loadComponent(path))) {
        insertRow(row, item);
    }
}

void IVComponentModel::removeComponentFromModel(const shared::Id &id)
{
    if (auto item = itemById(id)) {
        removeRow(item->row());
    }
}
QStandardItem *IVComponentModel::loadComponent(const QString &path)
{
    auto component = m_compLibrary->loadComponent(path);
    return itemFromComponent(component);
}

} // namespace ive
