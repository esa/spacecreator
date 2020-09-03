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

#include "contextparametersmodel.h"

#include "aadlcommonprops.h"
#include "aadlobject.h"
#include "aadlobjectfunction.h"
#include "asn1modelstorage.h"
#include "commandsstack.h"
#include "interface/commands/cmdentityattributechange.h"
#include "interface/commands/cmdentitypropertychange.h"
#include "interface/commands/cmdentitypropertycreate.h"
#include "interface/commands/commandsfactory.h"
#include "interface/properties/dynamicproperty.h"
#include "interface/properties/dynamicpropertyconfig.h"

#include <QDebug>
#include <algorithm>

namespace aadlinterface {

ContextParametersModel::ContextParametersModel(QObject *parent)
    : PropertiesModelBase(parent)
{
}

ContextParametersModel::~ContextParametersModel() { }

void ContextParametersModel::createNewRow(const aadl::ContextParameter &param, int row)
{
    QStandardItem *titleItem = new QStandardItem(param.name());
    QStandardItem *typeItem = new QStandardItem(param.paramTypeName());
    QStandardItem *valueItem = new QStandardItem();
    valueItem->setData(param.defaultValue(), Qt::EditRole);

    setItem(row, ColumnName, titleItem);
    setItem(row, ColumnType, typeItem);
    setItem(row, ColumnValue, valueItem);

    m_params.insert(row, param);
}

void ContextParametersModel::setDataObject(aadl::AADLObject *obj)
{
    clear();
    m_params.clear();
    m_dataObject = obj;

    if (!m_dataObject)
        return;

    if (auto func = qobject_cast<aadl::AADLObjectFunctionType *>(m_dataObject)) {
        const int paramsCount = func->contextParams().size();

        beginInsertRows(QModelIndex(), 0, paramsCount);

        for (int i = 0; i < paramsCount; ++i) {
            auto param = func->contextParams().at(i);
            createNewRow(param, i);
        }

        endInsertRows();
    }
}

const aadl::AADLObject *ContextParametersModel::dataObject() const
{
    return m_dataObject;
}

void ContextParametersModel::setDataTypes(Asn1Acn::Asn1ModelStorage *dataTypes)
{
    m_dataTypes = dataTypes;
}

int ContextParametersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_dataObject ? m_params.size() : 0;
}

int ContextParametersModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant ContextParametersModel::data(const QModelIndex &index, int role) const
{
    const QVariant &res = QStandardItemModel::data(index, role);
    if (!index.isValid())
        return res;

    auto param = m_params.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole: {
        switch (index.column()) {
        case ColumnType:
            return param.paramTypeName();
        case ColumnValue:
            return param.defaultValue();
        default:
            return param.name();
        }
    }
    }

    return res;
}

bool ContextParametersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || data(index, role) == value)
        return false;

    if (role == Qt::EditRole) {
        auto paramOld = m_params.at(index.row());
        aadl::ContextParameter paramNew(paramOld);

        switch (index.column()) {
        case ColumnName: {
            if (!paramNew.setName(value.toString()))
                return false;
            break;
        }
        case ColumnType: {
            if (!paramNew.setParamTypeName(value.toString()))
                return false;
            break;
        }
        case ColumnValue: {
            if (!paramNew.setDefaultValue(m_dataTypes.data(), value))
                return false;
            break;
        }
        default:
            return false;
        }

        if (const auto attributesCmd = cmd::CommandsFactory::create(cmd::ChangeContextParameter,
                    { QVariant::fromValue(m_dataObject), QVariant::fromValue(paramOld),
                            QVariant::fromValue(paramNew) })) {

            cmd::CommandsStack::current()->push(attributesCmd);
            m_params.replace(index.row(), paramNew);
        }
    }

    QStandardItemModel::setData(index, value, role);
    Q_EMIT dataChanged(index, index, { role, Qt::DisplayRole });
    return true;
}

bool ContextParametersModel::createProperty(const QString &propName)
{
    bool res(false);

    aadl::ContextParameter param(propName);
    param.setParamType(aadl::BasicParameter::Type::Timer);

    const auto propsCmd = cmd::CommandsFactory::create(
            cmd::CreateContextParameter, { QVariant::fromValue(m_dataObject), QVariant::fromValue(param) });
    if (propsCmd) {
        const int rows = rowCount();
        beginInsertRows(QModelIndex(), rows, rows);

        cmd::CommandsStack::current()->push(propsCmd);
        createNewRow(param, rows);
        res = true;

        endInsertRows();
    }

    return res;
}

bool ContextParametersModel::removeProperty(const QModelIndex &index)
{
    bool res(false);
    if (!index.isValid())
        return res;

    const int row(index.row());
    const auto propsCmd = cmd::CommandsFactory::create(
            cmd::RemoveContextParameter, { QVariant::fromValue(m_dataObject), QVariant::fromValue(row) });
    if (propsCmd) {
        cmd::CommandsStack::current()->push(propsCmd);
        removeRow(row);
        m_params.removeAt(row);

        res = true;
    }

    return res;
}

bool ContextParametersModel::isAttr(const QModelIndex & /*id*/) const
{
    return false;
}

bool ContextParametersModel::isProp(const QModelIndex & /*id*/) const
{
    return true;
}

Qt::ItemFlags ContextParametersModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QStandardItemModel::flags(index);
    if (index.column() == ColumnValue && m_params.at(index.row()).paramType() != aadl::BasicParameter::Type::Other)
        flags = flags & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;

    if (!m_dataObject)
        return flags;

    if (flags.testFlag(Qt::ItemIsEditable) || flags.testFlag(Qt::ItemIsEnabled)) {
        switch (m_dataObject->aadlType()) {
        case aadl::AADLObject::Type::Function: {
            if (auto fn = m_dataObject->as<const aadl::AADLObjectFunction *>())
                if (fn->inheritsFunctionType())
                    flags = flags & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;

            break;
        }
        default:
            break;
        }
    }

    return flags;
}

QVariant ContextParametersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case ColumnName:
            return tr("Name");
        case ColumnType:
            return tr("Type");
        case ColumnValue:
            return tr("Value");
        }
    }
    return QVariant();
}

}
