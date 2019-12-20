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

#include "ifaceparametersmodel.h"

#include "app/commandsstack.h"
#include "tab_aadl/aadlobject.h"
#include "tab_aadl/aadlobjectiface.h"
#include "tab_interface/commands/cmdentitypropertycreate.h"
#include "tab_interface/commands/commandsfactory.h"
#include "tab_interface/properties/dynamicproperty.h"
#include "tab_interface/properties/dynamicpropertyconfig.h"

#include <QDebug>
#include <algorithm>

namespace taste3 {
namespace aadl {

IfaceParametersModel::IfaceParametersModel(QObject *parent)
    : PropertiesModelBase(parent)
{
}

IfaceParametersModel::~IfaceParametersModel() {}

void IfaceParametersModel::createNewRow(const IfaceParameter &param, int row)
{
    QStandardItem *titleItem = new QStandardItem(param.name());
    QStandardItem *typeItem = new QStandardItem(param.paramTypeName());
    QStandardItem *encodingItem = new QStandardItem(param.encoding());
    QStandardItem *directionItem = new QStandardItem(IfaceParameter::directionName(param.direction()));

    setItem(row, ColumnName, titleItem);
    setItem(row, ColumnType, typeItem);
    setItem(row, ColumnEncoding, encodingItem);
    setItem(row, ColumnDirection, directionItem);

    m_params.insert(row, param);
}

void IfaceParametersModel::setDataObject(AADLObject *obj)
{
    clear();
    m_params.clear();
    m_dataObject = obj;

    if (!m_dataObject)
        return;

    if (AADLObjectIface *iface = qobject_cast<AADLObjectIface *>(m_dataObject)) {
        const QVector<IfaceParameter> &params(iface->params());
        const int paramsCount = params.size();

        beginInsertRows(QModelIndex(), 0, paramsCount);

        for (int i = 0; i < paramsCount; ++i) {
            const IfaceParameter &param = params.at(i);
            createNewRow(param, i);
        }

        endInsertRows();
    }
}

int IfaceParametersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_dataObject ? m_params.size() : 0;
}

int IfaceParametersModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant IfaceParametersModel::data(const QModelIndex &index, int role) const
{
    const QVariant &res = QStandardItemModel::data(index, role);
    if (!index.isValid())
        return res;

    const IfaceParameter &param = m_params.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole: {
        switch (index.column()) {
        case ColumnName:
            return param.name();
        case ColumnType:
            return param.paramTypeName();
        case ColumnEncoding:
            return param.encoding();
        case ColumnDirection:
            return IfaceParameter::directionName(param.direction());
        }
    }
    }

    return res;
}

bool IfaceParametersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || data(index, role) == value)
        return false;

    if (role == Qt::EditRole) {
        const IfaceParameter &paramOld = m_params.at(index.row());
        IfaceParameter paramNew(paramOld);

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
        case ColumnEncoding: {
            if (!paramNew.setEncoding(value.toString()))
                return false;
            break;
        }
        case ColumnDirection: {
            if (!paramNew.setDirection(IfaceParameter::directionFromName(value.toString())))
                return false;
            break;
        }
        default:
            return false;
        }

        if (const auto attributesCmd = cmd::CommandsFactory::create(
                    cmd::ChangeIfaceParam,
                    { qVariantFromValue(m_dataObject), qVariantFromValue(paramOld), qVariantFromValue(paramNew) })) {

            taste3::cmd::CommandsStack::current()->push(attributesCmd);
            m_params.replace(index.row(), paramNew);
        }
    }

    QStandardItemModel::setData(index, value, role);
    emit dataChanged(index, index, { role, Qt::DisplayRole });
    return true;
}

bool IfaceParametersModel::createProperty(const QString &propName)
{
    bool res(false);

    IfaceParameter param(propName);

    const auto propsCmd = cmd::CommandsFactory::create(cmd::CreateIfaceParam,
                                                       { qVariantFromValue(m_dataObject), qVariantFromValue(param) });
    if (propsCmd) {
        const int rows = rowCount();
        beginInsertRows(QModelIndex(), rows, rows);

        taste3::cmd::CommandsStack::current()->push(propsCmd);
        createNewRow(param, rows);
        res = true;

        endInsertRows();
    }

    return res;
}

bool IfaceParametersModel::removeProperty(const QModelIndex &index)
{
    bool res(false);
    if (!index.isValid())
        return res;

    const int row(index.row());
    const auto propsCmd = cmd::CommandsFactory::create(
            cmd::RemoveIfaceParam, { qVariantFromValue(m_dataObject), qVariantFromValue(m_params.value(row)) });
    if (propsCmd) {
        taste3::cmd::CommandsStack::current()->push(propsCmd);
        removeRow(row);
        m_params.removeAt(row);

        res = true;
    }

    return res;
}

bool IfaceParametersModel::isAttr(const QModelIndex & /*id*/) const
{
    return false;
}

bool IfaceParametersModel::isProp(const QModelIndex & /*id*/) const
{
    return true;
}

QVariant IfaceParametersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case ColumnName:
            return tr("Name");
        case ColumnType:
            return tr("Type");
        case ColumnEncoding:
            return tr("Encoding Protocol");
        case ColumnDirection:
            return tr("Direction");
        }
    }
    return QVariant();
}

} // namespace aadl
} // namespace taste3
