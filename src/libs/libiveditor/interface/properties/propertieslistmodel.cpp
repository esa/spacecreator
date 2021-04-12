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
#include "aadlmodel.h"
#include "aadlnamevalidator.h"
#include "aadlobject.h"
#include "commandsstack.h"
#include "interface/commands/cmdentityattributechange.h"
#include "interface/commands/cmdentitypropertychange.h"
#include "interface/commands/cmdentitypropertycreate.h"
#include "interface/commands/cmdentitypropertyremove.h"
#include "interface/commands/cmdentitypropertyrename.h"
#include "propertytemplateconfig.h"

#include <QApplication>
#include <QDebug>
#include <QRegularExpression>
#include <algorithm>

namespace ive {

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

ivm::meta::Props::Token PropertiesListModel::tokenFromIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return ivm::meta::Props::Token::Unknown;

    return ivm::meta::Props::token(tokenNameFromIndex(index));
}

PropertiesListModel::PropertiesListModel(
        cmd::CommandsStack::Macro *macro, ivm::PropertyTemplateConfig *dynPropConfig, QObject *parent)
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
    font.setItalic(data.info == ivm::PropertyTemplate::Info::Property);
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

QStringList PropertiesListModel::sortedKeys(const QList<ivm::PropertyTemplate *> &templates) const
{
    QStringList templateKeys;
    for (ivm::PropertyTemplate *propTemplate : templates) {
        templateKeys.append(propTemplate->name());
    }
    QStringList keys { m_dataObject->attrs().keys() << m_dataObject->props().keys() };
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

void PropertiesListModel::updateRows(const QList<ivm::PropertyTemplate *> &templates)
{
    auto init = [this](const QStringList &keys) {
        int row = -1;
        for (const QString &key : keys) {
            ivm::PropertyTemplate *propTemplate = m_propTemplatesConfig->propertyTemplateForObject(m_dataObject, key);
            if (propTemplate && (!propTemplate->isVisible() || !propTemplate->validate(m_dataObject)))
                continue;

            RowData rd;
            rd.row = ++row;
            rd.name = key;
            const ivm::PropertyTemplate::Type type =
                    propTemplate ? propTemplate->type() : ivm::PropertyTemplate::Type::String;
            const QString decodedKey = ivm::AADLNameValidator::decodeName(m_dataObject->aadlType(), key);
            if (propTemplate) {
                rd.rxValidator = propTemplate->valueValidatorPattern();
                rd.label = propTemplate->label().isEmpty() ? decodedKey : propTemplate->label();
                rd.editValue = ivm::PropertyTemplate::convertData(propTemplate->value(), type);
            } else {
                rd.label = decodedKey;
                rd.editValue = QVariant(QVariant::String);
            }

            if (m_dataObject->hasAttribute(key)) {
                rd.value = m_dataObject->attr(key);
                rd.info = ivm::PropertyTemplate::Info::Attribute;
            } else if (m_dataObject->hasProperty(key)) {
                rd.value = m_dataObject->prop(key);
                rd.info = ivm::PropertyTemplate::Info::Property;
            } else if (propTemplate) {
                rd.info = propTemplate->info();
            }
            updateRow(rd);
        }
    };

    init(sortedKeys(templates));
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
        auto isValueValid = [this](const QString &name, const QString &value) {
            if (ivm::PropertyTemplate *propTemplate =
                            m_propTemplatesConfig->propertyTemplateForObject(m_dataObject, name)) {
                const QString valuePattern = propTemplate->valueValidatorPattern();
                if (!valuePattern.isEmpty()) {
                    const QRegularExpression rx(valuePattern);
                    const QRegularExpressionMatch match = rx.match(value);
                    return match.capturedLength() == value.length();
                }
            }
            return true;
        };

        if (isAttr(index) && index.column() == Column::Value) {
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
            case Column::Value: {
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
            case Column::Name: {
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
    const QModelIndex &propId = this->index(row, Column::Name);
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
    return id.isValid() && static_cast<int>(ivm::PropertyTemplate::Info::Attribute) == id.data(InfoRole).toInt();
}

bool PropertiesListModel::isProp(const QModelIndex &id) const
{
    return id.isValid() && static_cast<int>(ivm::PropertyTemplate::Info::Property) == id.data(InfoRole).toInt();
}

bool PropertiesListModel::isEditable(const QModelIndex &idx) const
{
    if (idx.column() == Column::Name) {
        const QString propName = m_names.value(idx.row());
        if (m_propTemplatesConfig->hasPropertyTemplateForObject(m_dataObject, propName)) {
            return false;
        }
        return index(idx.row(), Column::Value).data().isNull();
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

QVariant FunctionPropertiesListModel::data(const QModelIndex &index, int role) const
{
    if (index.column() == Column::Value && role == EditRole
            && tokenFromIndex(index) == ivm::meta::Props::Token::instance_of) {
        QStringList availableFnTypes { QString() };
        if (auto fn = m_dataObject->as<const ivm::AADLFunction *>()) {
            availableFnTypes << fn->objectsModel()->getAvailableFunctionTypes(fn).keys();
        }
        return availableFnTypes;
    }

    return PropertiesListModel::data(index, role);
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
        if (dataObject()->isFunctionType() || index.column() == Column::Name)
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
    if ((role == PropertiesListModel::DataRole || role == PropertiesListModel::EditRole)
            && index.column() == Column::Value) {
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

    if (auto iface = m_dataObject->as<const ivm::AADLIface *>()) {
        const bool isClone = iface->isClone();
        switch (tokenFromIndex(index)) {
        case ivm::meta::Props::Token::name:
        case ivm::meta::Props::Token::InheritPI:
            return !isClone;
        default:
            if (iface->isRequired()) {
                if (auto ri = iface->as<const ivm::AADLIfaceRequired *>()) {
                    return !isClone && !ri->hasPrototypePi();
                }
            }
            return !isClone;
        }
    }

    return true;
}

}
