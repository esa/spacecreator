/*
  Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commands/cmdentityattributeschange.h"
#include "commands/cmdentitypropertycreate.h"
#include "commands/cmdentitypropertyremove.h"
#include "commands/cmdentitypropertyrename.h"
#include "commandsstackbase.h"
#include "propertytemplateconfig.h"
#include "vemodel.h"
#include "veobject.h"

#include <QApplication>
#include <QDebug>
#include <QRegularExpression>

namespace shared {

QString PropertiesListModel::tokenNameFromIndex(const QModelIndex &index)
{
    QString name;
    if (index.column() == PropertiesListModel::Column::Name) {
        name = index.data(PropertiesListModel::DataRole).toString();
    } else {
        const QModelIndex titleIndex = index.siblingAtColumn(PropertiesListModel::Column::Name);
        if (titleIndex.isValid()) {
            name = titleIndex.data(PropertiesListModel::DataRole).toString();
        }
    }
    return name;
}

bool PropertiesListModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
        const QModelIndex &destinationParent, int destinationChild)
{
    if (PropertiesModelBase::moveRows(sourceParent, sourceRow, count, destinationParent, destinationChild)) {
        for (int idx = 0; idx < count; ++idx)
            std::swap(m_names[sourceRow + idx], m_names[destinationChild + idx]);
        return true;
    }
    return false;
}

PropertiesListModel::PropertiesListModel(
        cmd::CommandsStackBase::Macro *macro, shared::PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : PropertiesModelBase(parent)
    , m_cmdMacro(macro)
    , m_propTemplatesConfig(dynPropConfig)
{
}

PropertiesListModel::~PropertiesListModel() { }

void PropertiesListModel::updateRow(const RowData &data)
{
    int row = data.row;
    if (row == -1 || row >= rowCount()) {
        row = rowCount();
        createNewRow(row, data.name);
    }

    QStandardItem *titleItem = item(row, Column::Name);
    Q_ASSERT(titleItem);
    if (!titleItem) {
        return;
    }
    titleItem->setData(data.label, Qt::DisplayRole);
    titleItem->setData(data.name, DataRole);

    QFont font = qApp->font();
    font.setBold(true);
    font.setItalic(data.info == shared::PropertyTemplate::Info::Property);
    titleItem->setData(font, Qt::FontRole);

    QStandardItem *valueItem = item(row, Column::Value);
    Q_ASSERT(valueItem);
    if (!valueItem) {
        return;
    }
    valueItem->setData(data.value, DataRole);
    valueItem->setData(data.editValue.isValid() ? data.editValue : data.value, EditRole);
    valueItem->setData(data.rxValidator, ValidatorRole);
    valueItem->setData(static_cast<int>(data.info), InfoRole);
}

QStringList PropertiesListModel::sortedKeys(const QList<shared::PropertyTemplate *> &templates) const
{
    QStringList templateKeys;
    for (shared::PropertyTemplate *propTemplate : templates) {
        templateKeys.append(propTemplate->name());
    }
    QStringList keys { m_dataObject->entityAttributes().keys() };
    std::sort(keys.begin(), keys.end(),
            [](const QString &s1, const QString &s2) { return s1.compare(s2, Qt::CaseInsensitive) < 0; });
    templateKeys << keys;
    templateKeys.removeDuplicates();
    return templateKeys;
}

void PropertiesListModel::createNewRow(int row, const QString &name)
{
    if (name.isEmpty() || m_names.contains(name)) {
        return;
    }

    m_names.append(name);

    QStandardItem *titleItem = new QStandardItem();
    setItem(row, Column::Name, titleItem);

    QStandardItem *valueItem = new QStandardItem();
    setItem(row, Column::Value, valueItem);
}

void PropertiesListModel::invalidateAttributes(const QString &propName)
{
    Q_UNUSED(propName);

    if (!m_dataObject)
        return;

    setDataObject(m_dataObject);
}

bool PropertiesListModel::isValueValid(const QString &name, const QString &value) const
{
    if (shared::PropertyTemplate *propTemplate = m_propTemplatesConfig->propertyTemplateForObject(m_dataObject, name)) {
        const QString valuePattern = propTemplate->valueValidatorPattern();
        if (!valuePattern.isEmpty()) {
            const QRegularExpression rx(valuePattern);
            const QRegularExpressionMatch match = rx.match(value);
            return match.capturedLength() == value.length();
        }
    }
    return true;
}

void PropertiesListModel::updateRows(const QList<shared::PropertyTemplate *> &templates)
{
    auto init = [this](const QStringList &keys) {
        int row = -1;
        for (const QString &key : keys) {
            shared::PropertyTemplate *propTemplate =
                    m_propTemplatesConfig->propertyTemplateForObject(m_dataObject, key);
            if (propTemplate && (!propTemplate->isVisible() || !propTemplate->validate(m_dataObject)))
                continue;

            RowData rd;
            rd.row = ++row;
            rd.name = key;
            const shared::PropertyTemplate::Type type =
                    propTemplate ? propTemplate->type() : shared::PropertyTemplate::Type::String;
            if (propTemplate) {
                rd.rxValidator = propTemplate->valueValidatorPattern();
                rd.label = propTemplate->label().isEmpty() ? key : propTemplate->label();
                rd.editValue = shared::PropertyTemplate::convertData(propTemplate->value(), type);
            } else {
                rd.label = key;
                rd.editValue = QVariant(QVariant::String);
            }

            const EntityAttribute attr = m_dataObject->entityAttribute(key);
            if (!attr.isValid() && propTemplate) {
                rd.info = propTemplate->info();
                rd.value = propTemplate->defaultValue().value<QString>();
            } else {
                rd.value = attr.value();
                rd.info = attr.isProperty() ? shared::PropertyTemplate::Info::Property
                                            : shared::PropertyTemplate::Info::Attribute;
            }

            updateRow(rd);
        }
    };

    init(sortedKeys(templates));
}

void PropertiesListModel::setDataObject(VEObject *obj)
{
    removeRows(0, rowCount());
    m_names.clear();

    if (m_dataObject) {
        disconnect(m_dataObject, &shared::VEObject::attributeChanged, this, &PropertiesListModel::invalidateAttributes);
    }

    m_dataObject = obj;

    if (!m_dataObject)
        return;

    connect(m_dataObject, &shared::VEObject::attributeChanged, this, &PropertiesListModel::invalidateAttributes,
            Qt::UniqueConnection);

    updateRows(m_propTemplatesConfig->propertyTemplatesForObject(m_dataObject));
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

QVariant PropertiesListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case Column::Name:
            return tr("Name");
        case Column::Value:
            return tr("Value");
        default:
            break;
        }
    }
    return QVariant();
}

bool PropertiesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    const QVariant currentValue = data(index, role);
    if (currentValue == value) {
        if (role != DataRole || !currentValue.isNull()) {
            return false;
        }
    }

    if (role == DataRole || role == Qt::EditRole) {
        const QString &name = tokenNameFromIndex(index);
        if (isAttr(index) && index.column() == Column::Value) {
            if (m_dataObject->entityAttributeValue(name) == value) {
                return false;
            }

            if (!isValueValid(name, value.toString())) {
                return false;
            }

            const QList<EntityAttribute> attributes = { EntityAttribute(
                    name, QVariant::fromValue(value), EntityAttribute::Type::Attribute) };
            auto attributesCmd =
                    new shared::cmd::CmdEntityAttributesChange(m_propTemplatesConfig, m_dataObject, attributes);
            m_cmdMacro->push(attributesCmd);
            return true;
        } else if (isProp(index)) {
            switch (index.column()) {
            case Column::Value: {
                if (m_dataObject->entityAttributeValue(name) == value) {
                    return false;
                }

                if (!isValueValid(name, value.toString())) {
                    return false;
                }

                const QList<EntityAttribute> props = { EntityAttribute(name, value, EntityAttribute::Type::Property) };
                auto propsCmd = new shared::cmd::CmdEntityAttributesChange(m_propTemplatesConfig, m_dataObject, props);
                m_cmdMacro->push(propsCmd);
                return true;
            }
            case Column::Name: {
                const QString &newName = value.toString();
                if (newName.isEmpty() || m_names.contains(newName))
                    return false;
                const QHash<QString, QString> props = { { name, newName } };
                auto propsCmd = new shared::cmd::CmdEntityPropertyRename(m_dataObject, props);
                m_cmdMacro->push(propsCmd);
                const int idx = m_names.indexOf(name);
                if (idx >= 0) {
                    m_names.replace(idx, newName);
                }
                return true;
            }
            default: {
                qWarning() << "Invalid column:" << index.column();
                break;
            }
            }
        }
    }

    QStandardItemModel::setData(index, value, role);
    return true;
}

bool PropertiesListModel::createProperty(const QString &propName)
{
    if (m_names.contains(propName)) {
        return false;
    }

    const QVariantHash props = { { propName, QString() } };
    auto propsCmd = new shared::cmd::CmdEntityPropertyCreate(m_dataObject, props);
    return m_cmdMacro->push(propsCmd);
}

bool PropertiesListModel::removeProperty(const QModelIndex &index)
{
    if (!index.isValid()) {
        return false;
    }

    const int row(index.row());
    const QModelIndex &propId = this->index(row, Column::Name);
    if (isAttr(propId))
        return false;

    const QString &propName = propId.data().toString();
    const QStringList props { propName };
    auto propsCmd = new shared::cmd::CmdEntityPropertyRemove(m_dataObject, props);
    removeRow(row);
    m_names.removeAt(row);
    m_cmdMacro->push(propsCmd);

    return true;
}

bool PropertiesListModel::isAttr(const QModelIndex &id) const
{
    return id.isValid() && static_cast<int>(shared::PropertyTemplate::Info::Attribute) == id.data(InfoRole).toInt();
}

bool PropertiesListModel::isProp(const QModelIndex &id) const
{
    return id.isValid() && static_cast<int>(shared::PropertyTemplate::Info::Property) == id.data(InfoRole).toInt();
}

bool PropertiesListModel::isEditable(const QModelIndex &idx) const
{
    const QString propName = m_names.value(idx.row());
    if (idx.column() == Column::Name) {
        if (m_propTemplatesConfig->hasPropertyTemplateForObject(m_dataObject, propName)) {
            return false;
        }
        return index(idx.row(), Column::Value).data(DataRole).isNull();
    } else if (idx.column() == Column::Value) {
        if (const PropertyTemplate *propTemplate =
                        m_propTemplatesConfig->propertyTemplateForObject(m_dataObject, propName)) {
            return propTemplate->isEditable();
        }
    }

    return true;
}

Qt::ItemFlags PropertiesListModel::flags(const QModelIndex &index) const
{
    const bool editable = isEditable(index);
    Qt::ItemFlags flags = QStandardItemModel::flags(index);
    for (Qt::ItemFlag f : { Qt::ItemIsEditable, Qt::ItemIsEnabled })
        if (flags.testFlag(f) != editable)
            flags.setFlag(f, editable);

    return flags;
}

} // namespace shared
