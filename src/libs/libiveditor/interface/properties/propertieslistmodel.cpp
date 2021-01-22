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
#include "aadlnamevalidator.h"
#include "aadlobject.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"
#include "commandsstack.h"
#include "interface/commands/cmdentityattributechange.h"
#include "interface/commands/cmdentitypropertychange.h"
#include "interface/commands/cmdentitypropertycreate.h"
#include "interface/commands/commandsfactory.h"
#include "propertytemplateconfig.h"

#include <QDebug>
#include <algorithm>

namespace ive {

aadl::meta::Props::Token tokenFromIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return aadl::meta::Props::Token::Unknown;

    const QString name = index.model()->index(index.row(), PropertiesListModel::ColumnTitle).data().toString();
    return aadl::meta::Props::token(name);
}

PropertiesListModel::PropertiesListModel(
        cmd::CommandsStack::Macro *macro, aadl::PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : PropertiesModelBase(parent)
    , m_cmdMacro(macro)
    , m_propTamplesConfig(dynPropConfig)
{
}

PropertiesListModel::~PropertiesListModel() { }

void PropertiesListModel::createNewRow(int row, const QString &label, const QString &name,
        aadl::PropertyTemplate::Info info, const QVariant &value, const QVariant &editValue,
        const QVariant &defaulValue)
{
    m_names.append(name);
    QStandardItem *titleItem = new QStandardItem(label.isEmpty() ? name : label);
    titleItem->setData(name, PropertyNameRole);

    QStandardItem *valueItem = new QStandardItem();
    valueItem->setData(value, Qt::DisplayRole);
    valueItem->setData(value, Qt::EditRole);
    valueItem->setData(editValue, PropertyDataRole);
    valueItem->setData(static_cast<int>(info), PropertyInfoRole);

    setItem(row, ColumnTitle, titleItem);
    setItem(row, ColumnValue, valueItem);
}

void PropertiesListModel::invalidateProperties(const QString &propName)
{
    Q_UNUSED(propName)

    if (!m_dataObject)
        return;

    static const QMap<aadl::AADLObject::Type, aadl::PropertyTemplate::Scope> kScopeMappings = {
        { aadl::AADLObject::Type::Function, aadl::PropertyTemplate::Scope::Function },
        { aadl::AADLObject::Type::RequiredInterface, aadl::PropertyTemplate::Scope::Required_Interface },
        { aadl::AADLObject::Type::ProvidedInterface, aadl::PropertyTemplate::Scope::Provided_Interface },
        { aadl::AADLObject::Type::Comment, aadl::PropertyTemplate::Scope::Comment },
        { aadl::AADLObject::Type::Connection, aadl::PropertyTemplate::Scope::Connection },
    };
    const auto attrs = m_propTamplesConfig->propertyTemplatesForObject(m_dataObject);
    QStringList attrsAboutToBeRemoved, attrsAboutToBeAdded;
    /// TBD:
}

void PropertiesListModel::invalidateAttributes(const QString &attrName)
{
    Q_UNUSED(attrName)

    /// TBD:
}

void PropertiesListModel::setDataObject(aadl::AADLObject *obj)
{
    clear();
    m_names.clear();

    if (m_dataObject) {
        disconnect(m_dataObject, qOverload<const QString &>(&aadl::AADLObject::propertyChanged), this,
                &PropertiesListModel::invalidateProperties);
        disconnect(m_dataObject, qOverload<const QString &>(&aadl::AADLObject::attributeChanged), this,
                &PropertiesListModel::invalidateAttributes);
    }

    m_dataObject = obj;

    if (!m_dataObject)
        return;

    connect(m_dataObject, qOverload<const QString &>(&aadl::AADLObject::propertyChanged), this,
            &PropertiesListModel::invalidateProperties, Qt::UniqueConnection);
    connect(m_dataObject, qOverload<const QString &>(&aadl::AADLObject::attributeChanged), this,
            &PropertiesListModel::invalidateAttributes, Qt::UniqueConnection);

    auto initRows = [this](const QHash<QString, QVariant> &vals, aadl::PropertyTemplate::Info info, int offset,
                            const QHash<QString, aadl::PropertyTemplate *> &templates) {
        QList<QString> keys(vals.keys());
        std::sort(keys.begin(), keys.end());
        for (int i = 0; i < keys.size(); ++i) {
            auto propertyPtr = templates.value(keys[i]);
            if (propertyPtr && !propertyPtr->isVisible())
                continue;

            const QVariant propTemplatesValues = propertyPtr ? propertyPtr->valuesList() : QVariant();
            const aadl::PropertyTemplate::Type type =
                    propertyPtr ? propertyPtr->type() : aadl::PropertyTemplate::Type::String;
            const QString title = propertyPtr ? propertyPtr->label() : QString();
            createNewRow(i + offset, title, keys[i], info, vals[keys[i]],
                    propertyPtr ? aadl::PropertyTemplate::convertData(propTemplatesValues, type)
                                : QVariant(QVariant::String),
                    propertyPtr ? propertyPtr->defaultValue() : QVariant(QVariant::String));
        }
    };

    const auto templates = m_propTamplesConfig->propertyTemplatesForObject(m_dataObject);

    beginResetModel();
    initRows(m_dataObject->attrs(), aadl::PropertyTemplate::Info::Attribute, rowCount(), templates);
    initRows(m_dataObject->props(), aadl::PropertyTemplate::Info::Property, rowCount(), templates);

    for (auto propTemplate : templates) {
        if (!m_names.contains(propTemplate->name()) && propTemplate->isVisible()) {
            QVariant value;
            if (propTemplate->type() == aadl::PropertyTemplate::Type::Unknown) {
                continue;
            } else if (propTemplate->type() == aadl::PropertyTemplate::Type::Enumeration) {
                value = propTemplate->valuesList();
            }
            createNewRow(rowCount(), propTemplate->label(), propTemplate->name(), propTemplate->info(), {},
                    aadl::PropertyTemplate::convertData(value, propTemplate->type()), propTemplate->defaultValue());
        }
    }

    endResetModel();
}

const aadl::AADLObject *PropertiesListModel::dataObject() const
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
            if (title == aadl::meta::Props::token(aadl::meta::Props::Token::name)) {
                return QVariant::fromValue(
                        aadl::AADLNameValidator::decodeName(m_dataObject->aadlType(), value.toString()));
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
        if (isAttr(index) && index.column() == ColumnValue) {
            QVariant attributeValue = value;
            switch (tokenFromIndex(index)) {
            case aadl::meta::Props::Token::name: {
                const QString newName = aadl::AADLNameValidator::encodeName(m_dataObject->aadlType(), value.toString());
                if (!aadl::AADLNameValidator::isAcceptableName(m_dataObject, newName)) {
                    return false;
                }
                attributeValue = newName;
                break;
            }
            default:
                break;
            }

            const QVariantMap attributes = { { name, attributeValue } };
            const auto attributesCmd = cmd::CommandsFactory::create(cmd::ChangeEntityAttributes,
                    { QVariant::fromValue(m_dataObject), QVariant::fromValue(attributes) });
            m_cmdMacro->push(attributesCmd);
        } else if (isProp(index)) {
            switch (index.column()) {
            case ColumnValue: {
                const QVariantMap props = { { name, value } };
                const auto propsCmd = cmd::CommandsFactory::create(
                        cmd::ChangeEntityProperty, { QVariant::fromValue(m_dataObject), QVariant::fromValue(props) });
                m_cmdMacro->push(propsCmd);

                break;
            }
            case ColumnTitle: {
                const QString &newName = value.toString();
                if (m_names.contains(newName))
                    return false;
                const QHash<QString, QString> props = { { name, newName } };
                const auto propsCmd = cmd::CommandsFactory::create(
                        cmd::RenameEntityProperty, { QVariant::fromValue(m_dataObject), QVariant::fromValue(props) });
                m_cmdMacro->push(propsCmd);
                const int idx = m_names.indexOf(name);
                if (idx >= 0) {
                    m_names.replace(idx, newName);
                }
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
    Q_EMIT dataChanged(index, index, { role, Qt::DisplayRole });
    return true;
}

bool PropertiesListModel::createProperty(const QString &propName)
{
    bool res(false);
    if (m_names.contains(propName))
        return res;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    const QVariantMap props = { { propName, QString() } };
    const auto propsCmd = cmd::CommandsFactory::create(
            cmd::CreateEntityProperty, { QVariant::fromValue(m_dataObject), QVariant::fromValue(props) });
    if (propsCmd) {
        m_cmdMacro->push(propsCmd);
        res = true;
    }

    createNewRow(rowCount(), {}, propName, aadl::PropertyTemplate::Info::Property, QVariant(QVariant::String),
            QVariant(QVariant::String), QVariant(QVariant::String));

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
    const auto propsCmd = cmd::CommandsFactory::create(
            cmd::RemoveEntityProperty, { QVariant::fromValue(m_dataObject), QVariant::fromValue(props) });
    if (propsCmd) {
        m_cmdMacro->push(propsCmd);
        removeRow(row);
        m_names.removeAt(row);

        res = true;
    }

    return res;
}

bool PropertiesListModel::isAttr(const QModelIndex &id) const
{
    return id.isValid()
            && static_cast<int>(aadl::PropertyTemplate::Info::Attribute) == id.data(PropertyInfoRole).toInt();
}

bool PropertiesListModel::isProp(const QModelIndex &id) const
{
    return id.isValid()
            && static_cast<int>(aadl::PropertyTemplate::Info::Property) == id.data(PropertyInfoRole).toInt();
}

bool PropertiesListModel::isEditable(const QModelIndex &idx) const
{
    if (idx.column() == ColumnTitle) {
        const QString propName = m_names.value(idx.row());
        const auto templates = m_propTamplesConfig->propertyTemplatesForObject(m_dataObject);
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
        case aadl::meta::Props::Token::RootCoordinates:
        case aadl::meta::Props::Token::InnerCoordinates:
        case aadl::meta::Props::Token::coordinates: {
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
        cmd::CommandsStack::Macro *macro, aadl::PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : PropertiesListModel(macro, dynPropConfig, parent)
{
}

bool FunctionPropertiesListModel::isEditable(const QModelIndex &index) const
{
    if (!dataObject() || !index.isValid() || !PropertiesListModel::isEditable(index))
        return false;

    bool editable = true;
    switch (tokenFromIndex(index)) {
    case aadl::meta::Props::Token::is_type: {
        editable = false;
        break;
    }
    case aadl::meta::Props::Token::name: {
        editable = true;
        break;
    }
    case aadl::meta::Props::Token::instance_of: {
        if (dataObject()->isFunctionType() || index.column() == ColumnTitle)
            editable = false;
        else {
            if (auto fn = dataObject()->as<const aadl::AADLObjectFunction *>()) {
                editable = fn->instanceOf() || fn->interfaces().isEmpty();
            }
        }
        break;
    }
    default:
        if (auto fn = dataObject()->as<const aadl::AADLObjectFunction *>())
            editable = !fn->inheritsFunctionType();
        break;
    }

    return editable;
}

InterfacePropertiesListModel::InterfacePropertiesListModel(
        cmd::CommandsStack::Macro *macro, aadl::PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : PropertiesListModel(macro, dynPropConfig, parent)
{
}

QVariant InterfacePropertiesListModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.column() == ColumnValue) {
        if (m_dataObject->aadlType() == aadl::AADLObject::Type::RequiredInterface
                && tokenFromIndex(index) == aadl::meta::Props::Token::name) {
            return m_dataObject->as<aadl::AADLObjectIfaceRequired *>()->ifaceLabel();
        }
    }
    return PropertiesListModel::data(index, role);
}

bool InterfacePropertiesListModel::isEditable(const QModelIndex &index) const
{
    if (!dataObject() || !index.isValid() || !PropertiesListModel::isEditable(index))
        return false;

    bool editable = true;
    if (auto iface = m_dataObject->as<const aadl::AADLObjectIface *>()) {
        const bool isClone = iface->isClone();
        switch (tokenFromIndex(index)) {
        case aadl::meta::Props::Token::Autonamed: {
            editable = false;
            break;
        }
        case aadl::meta::Props::Token::name:
        case aadl::meta::Props::Token::InheritPI: {
            editable = !isClone;
            break;
        }
        default: {
            bool isInheritedRI = false;
            if (iface->isRequired())
                if (auto ri = iface->as<const aadl::AADLObjectIfaceRequired *>())
                    isInheritedRI = ri->hasPrototypePi();
            editable = !isClone && !isInheritedRI;
            break;
        }
        }
    }

    return editable;
}
}
