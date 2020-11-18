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
#include "interface/properties/dynamicpropertyconfig.h"

#include <QDebug>
#include <algorithm>

namespace aadlinterface {

aadl::meta::Props::Token tokenFromIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return aadl::meta::Props::Token::Unknown;

    const QString name = index.model()->index(index.row(), PropertiesListModel::ColumnTitle).data().toString();
    return aadl::meta::Props::token(name);
}

PropertiesListModel::PropertiesListModel(cmd::CommandsStack::Macro *macro, QObject *parent)
    : PropertiesModelBase(parent)
    , m_cmdMacro(macro)
{
}

PropertiesListModel::~PropertiesListModel() {}

void PropertiesListModel::createNewRow(
        int row, const QString &title, DynamicProperty::Info info, const QVariant &value, const QVariant &editValue)
{
    m_names.append(title);
    QStandardItem *titleItem = new QStandardItem(title);

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

    static const QMap<aadl::AADLObject::Type, DynamicProperty::Scope> kScopeMappings = {
        { aadl::AADLObject::Type::Function, DynamicProperty::Scope::Function },
        { aadl::AADLObject::Type::RequiredInterface, DynamicProperty::Scope::Required_Interface },
        { aadl::AADLObject::Type::ProvidedInterface, DynamicProperty::Scope::Provided_Interface },
        { aadl::AADLObject::Type::Comment, DynamicProperty::Scope::Comment },
        { aadl::AADLObject::Type::Connection, DynamicProperty::Scope::Connection },
    };
    const auto attrs = DynamicPropertyConfig::attributesForObject(m_dataObject);
    QStringList attrsAboutToBeRemoved, attrsAboutToBeAdded;
    /// TBD:
}

void PropertiesListModel::invalidateAttributes(const QString &attrName)
{
    Q_UNUSED(attrName)

    /// TBD:
}

static QVariant convertData(const QVariant &value, DynamicProperty::Type type)
{
    QVariant typedValue;
    switch (type) {
    case DynamicProperty::Type::Boolean: {
        const bool falseValue = QString::compare(value.toString(), QLatin1String("false")) == 0;
        const bool trueValue = QString::compare(value.toString(), QLatin1String("true")) == 0;
        if (falseValue) {
            typedValue = false;
        } else if (trueValue) {
            typedValue = true;
        } else {
            return QVariant(QVariant::Bool);
        }
    } break;
    case DynamicProperty::Type::Integer: {
        bool ok;
        typedValue = value.toString().toInt(&ok);
        if (!ok)
            return QVariant(QVariant::Int);
    } break;
    case DynamicProperty::Type::Real: {
        bool ok;
        typedValue = value.toString().toDouble(&ok);
        if (!ok)
            return QVariant(QVariant::Double);
    } break;
    case DynamicProperty::Type::String: {
        if (value.isValid())
            typedValue = value.toString();
        else
            typedValue = QVariant(QVariant::String);
    } break;
    case DynamicProperty::Type::Enumeration: {
        if (value.isValid()) {
            QStringList typedList;
            for (const auto &dataItem : value.toList()) {
                typedList.append(dataItem.toString());
            }
            typedValue = typedList;
        } else {
            typedValue = QVariant(QVariant::StringList);
        }
    } break;
    default:
        break;
    }
    return typedValue;
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

    auto initRows = [this](const QHash<QString, QVariant> &vals, DynamicProperty::Info info, int offset,
                            const QHash<QString, DynamicProperty *> &dynProps) {
        QList<QString> keys(vals.keys());
        std::sort(keys.begin(), keys.end());
        for (int i = 0; i < keys.size(); ++i) {
            auto propertyPtr = dynProps.value(keys[i]);
            if (propertyPtr && !propertyPtr->isVisible())
                continue;

            const QVariant dynPropValues = propertyPtr ? propertyPtr->valuesList() : QVariant();
            const DynamicProperty::Type type = propertyPtr ? propertyPtr->type() : DynamicProperty::Type::String;
            createNewRow(i + offset, keys[i], info, vals[keys[i]],
                    propertyPtr ? convertData(dynPropValues, type) : QVariant(QVariant::String));
        }
    };

    const auto attrs = DynamicPropertyConfig::attributesForObject(m_dataObject);

    beginResetModel();
    initRows(m_dataObject->attrs(), DynamicProperty::Info::Attribute, rowCount(), attrs);
    initRows(m_dataObject->props(), DynamicProperty::Info::Property, rowCount(), attrs);

    for (auto attr : attrs) {
        if (!m_names.contains(attr->name()) && attr->isVisible()) {
            QVariant value;
            if (attr->type() == DynamicProperty::Type::Unknown) {
                continue;
            } else if (attr->type() == DynamicProperty::Type::Enumeration) {
                value = attr->valuesList();
            } else { // Shouldn't be here
                const auto dataList = attr->valuesList();
                value = dataList.isEmpty() ? QVariant() : dataList.first();
            }
            createNewRow(rowCount(), attr->name(), attr->info(), {}, convertData(value, attr->type()));
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
            return title;

        return isAttr(index) ? m_dataObject->attr(title) : m_dataObject->prop(title);
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
            QVariant attributeValue = value;
            switch (tokenFromIndex(index)) {
            case aadl::meta::Props::Token::name: {
                if (!aadl::AADLNameValidator::isAcceptableName(m_dataObject, value.toString()))
                    return false; // TODO: move to editor's validator
                attributeValue = aadl::AADLNameValidator::encodeName(m_dataObject->aadlType(), value.toString());
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

    createNewRow(rowCount(), propName, DynamicProperty::Info::Property, {}, {});

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
    return id.isValid() && static_cast<int>(DynamicProperty::Info::Attribute) == id.data(PropertyInfoRole).toInt();
}

bool PropertiesListModel::isProp(const QModelIndex &id) const
{
    return id.isValid() && static_cast<int>(DynamicProperty::Info::Property) == id.data(PropertyInfoRole).toInt();
}

bool PropertiesListModel::isEditable(const QModelIndex &index) const
{
    return index.column() == ColumnTitle ? this->index(index.row(), ColumnValue).data().isNull() : true;
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

FunctionPropertiesListModel::FunctionPropertiesListModel(cmd::CommandsStack::Macro *macro, QObject *parent)
    : PropertiesListModel(macro, parent)
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
        if (dataObject()->isFunctionType())
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

InterfacePropertiesListModel::InterfacePropertiesListModel(cmd::CommandsStack::Macro *macro, QObject *parent)
    : PropertiesListModel(macro, parent)
{
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
