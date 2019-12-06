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

#include "propertieslistmodel.h"

#include "app/commandsstack.h"
#include "tab_aadl/aadlcommonprops.h"
#include "tab_aadl/aadlobject.h"
#include "tab_interface/commands/cmdentityattributechange.h"
#include "tab_interface/commands/cmdentitypropertychange.h"
#include "tab_interface/commands/cmdentitypropertycreate.h"
#include "tab_interface/commands/commandsfactory.h"
#include "tab_interface/properties/dynamicproperty.h"
#include "tab_interface/properties/dynamicpropertyconfig.h"

#include <QDebug>
#include <algorithm>

namespace taste3 {
namespace aadl {

PropertiesListModel::PropertiesListModel(QObject *parent)
    : PropertiesModelBase(parent)
{
}

PropertiesListModel::~PropertiesListModel() {}

void PropertiesListModel::createNewRow(const QString &title, const QVariant &value, ItemType type, int row)
{
    m_names.append(title);
    QStandardItem *titleItem = new QStandardItem(title);
    titleItem->setData(type, ItemTypeRole);
    QStandardItem *valueItem = new QStandardItem();
    valueItem->setData(value, Qt::EditRole);
    valueItem->setData(type, ItemTypeRole);

    setItem(row, ColumnTitle, titleItem);
    setItem(row, ColumnValue, valueItem);
}

void PropertiesListModel::setDataObject(AADLObject *obj)
{
    clear();
    m_names.clear();
    m_dataObject = obj;

    if (!m_dataObject)
        return;

    auto initRows = [this](const QHash<QString, QVariant> &vals, ItemType type, int offset) {
        QList<QString> keys(vals.keys());
        std::sort(keys.begin(), keys.end());
        for (int i = 0; i < keys.size(); ++i)
            createNewRow(keys[i], vals[keys[i]], type, i + offset);
    };

    beginInsertRows(QModelIndex(), 0, m_dataObject->attrs().size() + m_dataObject->props().size());
    initRows(m_dataObject->attrs(), ItemType::Attribute, rowCount());
    initRows(m_dataObject->props(), ItemType::Property, rowCount());

    QHash<QString, QVariant> customProps;
    for (auto attr : DynamicPropertyConfig::attributesForObject(m_dataObject)) {
        if (!m_names.contains(attr->name()))
            customProps.insert(attr->name(), QVariant::fromValue(attr));
    }
    initRows(customProps, ItemType::Property, rowCount());
    endInsertRows();
}

int PropertiesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_dataObject ? m_names.size() : 0;
}

int PropertiesListModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant PropertiesListModel::data(const QModelIndex &index, int role) const
{
    QVariant res = QStandardItemModel::data(index, role);
    if (!index.isValid())
        return res;

    switch (role) {
    case Qt::FontRole: {
        if (index.column() == ColumnTitle) {
            QFont font = res.value<QFont>();
            font.setBold(true);
            font.setItalic(isProp(index));

            res.setValue(font);
        }
        break;
    }
    case Qt::DisplayRole:
    case Qt::EditRole: {
        const bool itsAttr = isAttr(index);
        const QString &title = m_names.at(index.row());

        if (index.column() == ColumnTitle)
            return title;

        return itsAttr ? m_dataObject->attr(title) : m_dataObject->prop(title);
    }
    }

    return res;
}

bool PropertiesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || data(index, role) == value)
        return false;

    if (role == Qt::EditRole) {
        const QString &name = this->index(index.row(), ColumnTitle).data().toString();
        if (isAttr(index) && index.column() == ColumnValue) {
            const QVariantMap attributes = { { name, value } };
            const auto attributesCmd = cmd::CommandsFactory::create(
                    cmd::ChangeEntityAttributes, { qVariantFromValue(m_dataObject), qVariantFromValue(attributes) });
            taste3::cmd::CommandsStack::current()->push(attributesCmd);
        } else if (isProp(index)) {
            switch (index.column()) {
            case ColumnValue: {
                const QVariantMap props = { { name, value } };
                const auto propsCmd = cmd::CommandsFactory::create(
                        cmd::ChangeEntityProperty, { qVariantFromValue(m_dataObject), qVariantFromValue(props) });
                taste3::cmd::CommandsStack::current()->push(propsCmd);

                break;
            }
            case ColumnTitle: {
                const QString &newName = value.toString();
                if (m_names.contains(newName))
                    return false;
                const QHash<QString, QString> props = { { name, newName } };
                const auto propsCmd = cmd::CommandsFactory::create(
                        cmd::RenameEntityProperty, { qVariantFromValue(m_dataObject), qVariantFromValue(props) });
                taste3::cmd::CommandsStack::current()->push(propsCmd);
                m_names.replace(m_names.indexOf(name), newName);
                break;
            }
            default: {
                qWarning() << "Invalid column:" << index.column();
                break;
            }
            }
        }
    }

    QStandardItemModel::setData(index, value, role);
    emit dataChanged(index, index, { role, Qt::DisplayRole });
    return true;
}

bool PropertiesListModel::createProperty(const QString &propName)
{
    bool res(false);
    if (m_names.contains(propName))
        return res;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    const QVariantMap props = { { propName, QString() } };
    const auto propsCmd = cmd::CommandsFactory::create(cmd::CreateEntityProperty,
                                                       { qVariantFromValue(m_dataObject), qVariantFromValue(props) });
    if (propsCmd) {
        taste3::cmd::CommandsStack::current()->push(propsCmd);
        res = true;
    }

    createNewRow(propName, QString(), ItemType::Property, rowCount());

    endInsertRows();

    return res;
}

bool PropertiesListModel::removeProperty(const QModelIndex &index)
{
    bool res(false);
    if (!index.isValid())
        return res;

    const int row(index.row());
    const QModelIndex &propId = this->index(row, ColumnTitle);
    if (isAttr(propId))
        return res;

    const QString &propName = propId.data().toString();
    const QStringList props { propName };
    const auto propsCmd = cmd::CommandsFactory::create(cmd::RemoveEntityProperty,
                                                       { qVariantFromValue(m_dataObject), qVariantFromValue(props) });
    if (propsCmd) {
        taste3::cmd::CommandsStack::current()->push(propsCmd);
        removeRow(row);
        m_names.removeAt(row);

        res = true;
    }

    return res;
}

bool PropertiesListModel::isAttr(const QModelIndex &id) const
{
    return id.isValid() && ItemType::Attribute == id.data(ItemTypeRole).toInt();
}

bool PropertiesListModel::isProp(const QModelIndex &id) const
{
    return id.isValid() && ItemType::Property == id.data(ItemTypeRole).toInt();
}

Qt::ItemFlags PropertiesListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QStandardItemModel::flags(index);
    if (isAttr(index) && index.column() == ColumnTitle) {
        flags = flags & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;
    }

    return flags;
}

} // namespace aadl
} // namespace taste3
