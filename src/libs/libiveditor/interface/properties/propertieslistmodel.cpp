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

#include "aadlcommonprops.h"
#include "aadlfunction.h"
#include "aadliface.h"
#include "aadlnamevalidator.h"
#include "aadlobject.h"
#include "commandsstack.h"
#include "interface/commands/cmdentityattributechange.h"
#include "interface/commands/cmdentitypropertychange.h"
#include "interface/commands/cmdentitypropertycreate.h"
#include "interface/commands/cmdentitypropertyremove.h"
#include "interface/commands/cmdentitypropertyrename.h"
#include "propertytemplateconfig.h"

#include <QDebug>
#include <QRegularExpression>
#include <algorithm>

namespace ive {

ivm::meta::Props::Token tokenFromIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return ivm::meta::Props::Token::Unknown;

    QString name;
    if (index.column() == PropertiesListModel::ColumnTitle) {
        name = index.data(PropertiesListModel::PropertyNameRole).toString();
    } else {
        const QModelIndex titleIndex = index.sibling(index.row(), PropertiesListModel::ColumnTitle);
        if (titleIndex.isValid()) {
            name = titleIndex.data(PropertiesListModel::PropertyNameRole).toString();
        }
    }
    return ivm::meta::Props::token(name);
}

PropertiesListModel::PropertiesListModel(
        cmd::CommandsStack::Macro *macro, ivm::PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : PropertiesModelBase(parent)
    , m_cmdMacro(macro)
    , m_propTemplatesConfig(dynPropConfig)
{
}

PropertiesListModel::~PropertiesListModel() { }

void PropertiesListModel::updateRow(int row, const QString &label, const QString &name,
        ivm::PropertyTemplate::Info info, const QVariant &value, const QVariant &editValue, const QVariant &defaulValue)
{
    Q_UNUSED(defaulValue);

    const QString title = label.isEmpty() ? name : label;
    if (row == -1 || row >= rowCount()) {
        row = rowCount();
        createNewRow(row, name);
    }

    QStandardItem *titleItem = item(row, ColumnTitle);
    Q_ASSERT(titleItem);
    if (!titleItem) {
        return;
    }
    titleItem->setData(title, Qt::DisplayRole);
    titleItem->setData(name, Qt::EditRole);
    titleItem->setData(name, PropertyNameRole);

    QStandardItem *valueItem = item(row, ColumnValue);
    Q_ASSERT(valueItem);
    if (!valueItem) {
        return;
    }
    valueItem->setData(value, Qt::DisplayRole);
    valueItem->setData(value, Qt::EditRole);
    valueItem->setData(editValue, PropertyDataRole);
    valueItem->setData(static_cast<int>(info), PropertyInfoRole);
}

void PropertiesListModel::createNewRow(int row, const QString &name)
{
    if (name.isEmpty() || m_names.contains(name)) {
        return;
    }

    m_names.append(name);

    QStandardItem *titleItem = new QStandardItem();
    setItem(row, ColumnTitle, titleItem);

    QStandardItem *valueItem = new QStandardItem();
    setItem(row, ColumnValue, valueItem);
}

void PropertiesListModel::invalidateAttributes(const QString &propName)
{
    Q_UNUSED(propName);

    if (!m_dataObject)
        return;

    setDataObject(m_dataObject);
}

void PropertiesListModel::updateRows(const QHash<QString, ivm::PropertyTemplate *> &templates)
{
    auto initRows = [this](const QHash<QString, QVariant> &vals, ivm::PropertyTemplate::Info info,
                            const QHash<QString, ivm::PropertyTemplate *> &templates) {
        QList<QString> keys(vals.keys());
        std::sort(keys.begin(), keys.end());
        for (int i = 0; i < keys.size(); ++i) {
            const QString key = keys.at(i);
            const ivm::PropertyTemplate *propertyPtr = templates.value(key);
            if (propertyPtr && (!propertyPtr->isVisible() || !propertyPtr->validate(m_dataObject)))
                continue;

            const QVariant propTemplatesValues = propertyPtr ? propertyPtr->valuesList() : QVariant();
            const ivm::PropertyTemplate::Type type =
                    propertyPtr ? propertyPtr->type() : ivm::PropertyTemplate::Type::String;
            const QString title = propertyPtr ? propertyPtr->label() : key;

            const int row = m_names.indexOf(key);
            updateRow(row, title, key, info, vals[key],
                    propertyPtr ? ivm::PropertyTemplate::convertData(propTemplatesValues, type)
                                : QVariant(QVariant::String),
                    propertyPtr ? propertyPtr->defaultValue() : QVariant(QVariant::String));
        }
    };

    initRows(m_dataObject->attrs(), ivm::PropertyTemplate::Info::Attribute, templates);
    initRows(m_dataObject->props(), ivm::PropertyTemplate::Info::Property, templates);

    for (ivm::PropertyTemplate *propertyPtr : templates) {
        if (propertyPtr && !m_names.contains(propertyPtr->name()) && propertyPtr->isVisible()
                && propertyPtr->validate(m_dataObject)) {
            const QString key = propertyPtr->name();
            QVariant value;
            if (propertyPtr->type() == ivm::PropertyTemplate::Type::Unknown) {
                continue;
            } else if (propertyPtr->type() == ivm::PropertyTemplate::Type::Enumeration) {
                value = propertyPtr->valuesList();
            } else {
                value = propertyPtr->info() == ivm::PropertyTemplate::Info::Property ? m_dataObject->prop(key)
                                                                                     : m_dataObject->attr(key);
            }
            updateRow(rowCount(), propertyPtr->label(), key, propertyPtr->info(), value,
                    ivm::PropertyTemplate::convertData(propertyPtr->valuesList(), propertyPtr->type()),
                    propertyPtr->defaultValue());
        }
    }
}

void PropertiesListModel::setDataObject(ivm::AADLObject *obj)
{
    clear();
    m_names.clear();

    if (m_dataObject) {
        disconnect(m_dataObject, qOverload<const QString &>(&ivm::AADLObject::propertyChanged), this,
                &PropertiesListModel::invalidateAttributes);
        disconnect(m_dataObject, qOverload<const QString &>(&ivm::AADLObject::attributeChanged), this,
                &PropertiesListModel::invalidateAttributes);
    }

    m_dataObject = obj;

    if (!m_dataObject)
        return;

    connect(m_dataObject, qOverload<const QString &>(&ivm::AADLObject::propertyChanged), this,
            &PropertiesListModel::invalidateAttributes, Qt::UniqueConnection);
    connect(m_dataObject, qOverload<const QString &>(&ivm::AADLObject::attributeChanged), this,
            &PropertiesListModel::invalidateAttributes, Qt::UniqueConnection);

    const auto templates = m_propTemplatesConfig->propertyTemplatesForObject(m_dataObject);

    updateRows(templates);
}

const ivm::AADLObject *PropertiesListModel::dataObject() const
{
    return m_dataObject;
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
        const QString &title = m_names.at(index.row());

        if (index.column() == ColumnTitle)
            return res;

        if (isAttr(index)) {
            QVariant value = m_dataObject->attr(title);
            if (title == ivm::meta::Props::token(ivm::meta::Props::Token::name)) {
                return QVariant::fromValue(
                        ivm::AADLNameValidator::decodeName(m_dataObject->aadlType(), value.toString()));
            }
            return value;
        } else {
            m_dataObject->prop(title);
        }
    }
    }

    return res;
}

bool PropertiesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || data(index, role) == value)
        return false;

    if (role == Qt::EditRole) {
        const QString &name = this->index(index.row(), ColumnTitle).data(PropertyNameRole).toString();
        auto isValueValid = [this](const QString &name, const QString &value) {
            const auto templates = m_propTemplatesConfig->propertyTemplatesForObject(m_dataObject);
            if (ivm::PropertyTemplate *propTemplate = templates.value(name)) {
                const QString valuePattern = propTemplate->valueValidatorPattern();
                if (!valuePattern.isEmpty()) {
                    const QRegularExpression rx(valuePattern);
                    const QRegularExpressionMatch match = rx.match(value);
                    return match.capturedLength() == value.length();
                }
            }
            return true;
        };

        if (isAttr(index) && index.column() == ColumnValue) {
            QVariant attributeValue = value;
            switch (tokenFromIndex(index)) {
            case ivm::meta::Props::Token::name: {
                const QString newName = ivm::AADLNameValidator::encodeName(m_dataObject->aadlType(), value.toString());
                if (!ivm::AADLNameValidator::isAcceptableName(m_dataObject, newName)) {
                    return false;
                }
                attributeValue = newName;
                break;
            }
            default:
                break;
            }

            if (m_dataObject->attr(name) == attributeValue) {
                return false;
            }

            if (!isValueValid(name, attributeValue.toString())) {
                return false;
            }

            const QVariantHash attributes = { { name, attributeValue } };
            auto attributesCmd = new cmd::CmdEntityAttributeChange(m_dataObject, attributes);
            m_cmdMacro->push(attributesCmd);
            return true;
        } else if (isProp(index)) {
            switch (index.column()) {
            case ColumnValue: {
                if (m_dataObject->prop(name) == value) {
                    return false;
                }

                if (!isValueValid(name, value.toString())) {
                    return false;
                }

                const QVariantHash props = { { name, value } };
                auto propsCmd = new cmd::CmdEntityPropertyChange(m_dataObject, props);
                m_cmdMacro->push(propsCmd);
                return true;
            }
            case ColumnTitle: {
                const QString &newName = value.toString();
                if (newName.isEmpty() || m_names.contains(newName))
                    return false;
                const QHash<QString, QString> props = { { name, newName } };
                auto propsCmd = new cmd::CmdEntityPropertyRename(m_dataObject, props);
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
    if (role != Qt::DisplayRole) {
        Q_EMIT dataChanged(index, index, { role, Qt::DisplayRole });
    }
    return true;
}

bool PropertiesListModel::createProperty(const QString &propName)
{
    if (m_names.contains(propName)) {
        return false;
    }

    const QVariantHash props = { { propName, QString() } };
    auto propsCmd = new cmd::CmdEntityPropertyCreate(m_dataObject, props);
    return m_cmdMacro->push(propsCmd);
}

bool PropertiesListModel::removeProperty(const QModelIndex &index)
{
    if (!index.isValid()) {
        return false;
    }

    const int row(index.row());
    const QModelIndex &propId = this->index(row, ColumnTitle);
    if (isAttr(propId))
        return false;

    const QString &propName = propId.data().toString();
    const QStringList props { propName };
    auto propsCmd = new cmd::CmdEntityPropertyRemove(m_dataObject, props);
    removeRow(row);
    m_names.removeAt(row);
    m_cmdMacro->push(propsCmd);

    return true;
}

bool PropertiesListModel::isAttr(const QModelIndex &id) const
{
    return id.isValid()
            && static_cast<int>(ivm::PropertyTemplate::Info::Attribute) == id.data(PropertyInfoRole).toInt();
}

bool PropertiesListModel::isProp(const QModelIndex &id) const
{
    return id.isValid() && static_cast<int>(ivm::PropertyTemplate::Info::Property) == id.data(PropertyInfoRole).toInt();
}

bool PropertiesListModel::isEditable(const QModelIndex &idx) const
{
    if (idx.column() == ColumnTitle) {
        const QString propName = m_names.value(idx.row());
        const auto templates = m_propTemplatesConfig->propertyTemplatesForObject(m_dataObject);
        if (templates.contains(propName)) {
            return false;
        }
        return index(idx.row(), ColumnValue).data().isNull();
    }

    return true;
}

Qt::ItemFlags PropertiesListModel::flags(const QModelIndex &index) const
{
    bool editable = isEditable(index);
    if (editable) {
        switch (tokenFromIndex(index)) {
        case ivm::meta::Props::Token::RootCoordinates:
        case ivm::meta::Props::Token::InnerCoordinates:
        case ivm::meta::Props::Token::coordinates: {
            editable = false;
            break;
        }
        default:
            break;
        }
    }

    Qt::ItemFlags flags = QStandardItemModel::flags(index);
    for (Qt::ItemFlag f : { Qt::ItemIsEditable, Qt::ItemIsEnabled })
        if (flags.testFlag(f) != editable)
            flags.setFlag(f, editable);

    return flags;
}

FunctionPropertiesListModel::FunctionPropertiesListModel(
        cmd::CommandsStack::Macro *macro, ivm::PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : PropertiesListModel(macro, dynPropConfig, parent)
{
}

bool FunctionPropertiesListModel::isEditable(const QModelIndex &index) const
{
    if (!dataObject() || !index.isValid() || !PropertiesListModel::isEditable(index))
        return false;

    bool editable = true;
    switch (tokenFromIndex(index)) {
    case ivm::meta::Props::Token::is_type: {
        editable = false;
        break;
    }
    case ivm::meta::Props::Token::name: {
        editable = true;
        break;
    }
    case ivm::meta::Props::Token::instance_of: {
        if (dataObject()->isFunctionType() || index.column() == ColumnTitle)
            editable = false;
        else {
            if (auto fn = dataObject()->as<const ivm::AADLFunction *>()) {
                editable = fn->instanceOf() || fn->interfaces().isEmpty();
            }
        }
        break;
    }
    default:
        if (auto fn = dataObject()->as<const ivm::AADLFunction *>())
            editable = !fn->inheritsFunctionType();
        break;
    }

    return editable;
}

InterfacePropertiesListModel::InterfacePropertiesListModel(
        cmd::CommandsStack::Macro *macro, ivm::PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : PropertiesListModel(macro, dynPropConfig, parent)
{
}

QVariant InterfacePropertiesListModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.column() == ColumnValue) {
        if (m_dataObject->aadlType() == ivm::AADLObject::Type::RequiredInterface
                && tokenFromIndex(index) == ivm::meta::Props::Token::name) {
            return m_dataObject->as<ivm::AADLIfaceRequired *>()->ifaceLabel();
        }
    }
    return PropertiesListModel::data(index, role);
}

bool InterfacePropertiesListModel::isEditable(const QModelIndex &index) const
{
    if (!dataObject() || !index.isValid() || !PropertiesListModel::isEditable(index))
        return false;

    bool editable = true;
    if (auto iface = m_dataObject->as<const ivm::AADLIface *>()) {
        const bool isClone = iface->isClone();
        switch (tokenFromIndex(index)) {
        case ivm::meta::Props::Token::Autonamed: {
            editable = false;
            break;
        }
        case ivm::meta::Props::Token::name:
        case ivm::meta::Props::Token::InheritPI: {
            editable = !isClone;
            break;
        }
        default: {
            bool isInheritedRI = false;
            if (iface->isRequired())
                if (auto ri = iface->as<const ivm::AADLIfaceRequired *>())
                    isInheritedRI = ri->hasPrototypePi();
            editable = !isClone && !isInheritedRI;
            break;
        }
        }
    }

    return editable;
}
}
