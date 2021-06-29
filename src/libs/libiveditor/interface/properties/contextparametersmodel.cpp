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

#include "asn1/file.h"
#include "baseitems/common/ivutils.h"
#include "commandsstack.h"
#include "interface/commands/cmdcontextparameterchange.h"
#include "interface/commands/cmdcontextparametercreate.h"
#include "interface/commands/cmdcontextparameterremove.h"
#include "ivcommonprops.h"
#include "ivfunction.h"
#include "ivnamevalidator.h"
#include "ivobject.h"
#include "ivpropertytemplate.h"
#include "propertytemplateconfig.h"

#include <algorithm>

namespace ive {

ContextParametersModel::ContextParametersModel(cmd::CommandsStack::Macro *macro, QObject *parent)
    : PropertiesModelBase(parent)
    , m_cmdMacro(macro)
{
}

ContextParametersModel::~ContextParametersModel() { }

void ContextParametersModel::createNewRow(const ivm::ContextParameter &param, int row)
{
    m_params.insert(row, param);

    const QString name = ivm::IVNameValidator::decodeName(m_dataObject->type(), param.name());
    QStandardItem *titleItem = new QStandardItem(row, Column::Name);
    titleItem->setData(name, Qt::DisplayRole);
    titleItem->setData(name, DataRole);
    setItem(row, Column::Name, titleItem);

    QStandardItem *typeItem = new QStandardItem(row, Column::Type);
    typeItem->setData(param.paramTypeName(), DataRole);
    QStringList typesList = { m_asn1Names };
    typesList.append(shared::typeName(ivm::BasicParameter::Type::Timer));
    typeItem->setData(typesList, EditRole);
    setItem(row, Column::Type, typeItem);

    QStandardItem *valueItem = new QStandardItem(row, Column::Value);
    valueItem->setData(param.defaultValue(), Qt::DisplayRole);
    valueItem->setData(param.defaultValue(), DataRole);
    setItem(row, Column::Value, valueItem);
}

void ContextParametersModel::setDataObject(ivm::IVObject *obj)
{
    clear();
    m_params.clear();
    m_dataObject = obj;

    if (!m_dataObject)
        return;

    if (auto func = qobject_cast<ivm::IVFunctionType *>(m_dataObject)) {
        const int paramsCount = func->contextParams().size();

        for (int i = 0; i < paramsCount; ++i) {
            auto param = func->contextParams().at(i);
            createNewRow(param, i);
        }
    }
}

void ContextParametersModel::setDataTypes(const QSharedPointer<Asn1Acn::File> &dataTypes)
{
    if (m_dataTypes == dataTypes)
        return;

    m_dataTypes = dataTypes;
    if (m_dataTypes) {
        m_asn1Names = asn1Names(m_dataTypes.get());
    }
}

int ContextParametersModel::columnCount(const QModelIndex &) const
{
    static const QMetaEnum &me = QMetaEnum::fromType<Column>();
    return me.keyCount();
}

bool ContextParametersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || data(index, role) == value || !m_dataObject
            || !(m_dataObject->isFunction() || m_dataObject->isFunctionType())) {
        return false;
    }

    QStandardItem *item = itemFromIndex(index);
    if (!item) {
        return false;
    }
    const QString stringValue = value.toString();

    if (role == DataRole || role == Qt::EditRole) {
        const ivm::ContextParameter &paramOld = m_params.at(index.row());
        ivm::ContextParameter paramNew(paramOld);

        switch (index.column()) {
        case Column::Name: {
            if (!paramNew.setName(ivm::IVNameValidator::encodeName(m_dataObject->type(), stringValue)))
                return false;
            item->setData(stringValue, Qt::DisplayRole);
            item->setData(stringValue, DataRole);
            break;
        }
        case Column::Type: {
            if (!paramNew.setParamTypeName(stringValue))
                return false;
            item->setData(stringValue, DataRole);
            break;
        }
        case Column::Value: {
            if (!m_dataTypes) {
                return false;
            }
            const Asn1Acn::Types::Type *basicDataType = m_dataTypes->typeFromName(paramNew.paramTypeName());
            if (!paramNew.setDefaultValue(basicDataType, value)) {
                return false;
            }
            item->setData(value, Qt::DisplayRole);
            item->setData(value, DataRole);
            break;
        }
        default:
            return false;
        }

        if (auto entity = qobject_cast<ivm::IVFunctionType *>(m_dataObject)) {
            auto attributesCmd = new cmd::CmdContextParameterChange(entity, paramOld, paramNew);
            m_cmdMacro->push(attributesCmd);
            m_params.replace(index.row(), paramNew);
        }
    }
    return QStandardItemModel::setData(index, value, role);
}

bool ContextParametersModel::createProperty(const QString &propName)
{
    bool res(false);

    ivm::ContextParameter param(propName);
    param.setParamType(ivm::BasicParameter::Type::Timer);

    if (auto entity = qobject_cast<ivm::IVFunctionType *>(m_dataObject)) {
        auto propsCmd = new cmd::CmdContextParameterCreate(entity, param);
        const int rows = rowCount();

        createNewRow(param, rows);
        m_cmdMacro->push(propsCmd);
        res = true;
    }

    return res;
}

bool ContextParametersModel::removeProperty(const QModelIndex &index)
{
    bool res(false);
    if (!index.isValid())
        return res;

    const int row(index.row());
    if (auto entity = qobject_cast<ivm::IVFunctionType *>(m_dataObject)) {
        auto propsCmd = new cmd::CmdContextParameterRemove(entity, row);
        removeRow(row);
        m_params.removeAt(row);
        m_cmdMacro->push(propsCmd);

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
    if (index.column() == Column::Value && m_params.at(index.row()).paramType() != ivm::BasicParameter::Type::Other)
        flags = flags & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;

    if (!m_dataObject)
        return flags;

    if (flags.testFlag(Qt::ItemIsEditable) || flags.testFlag(Qt::ItemIsEnabled)) {
        switch (m_dataObject->type()) {
        case ivm::IVObject::Type::Function: {
            if (auto fn = m_dataObject->as<const ivm::IVFunction *>())
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
        case Column::Name:
            return tr("Name");
        case Column::Type:
            return tr("Type");
        case Column::Value:
            return tr("Value");
        }
    }
    return QVariant();
}
}
