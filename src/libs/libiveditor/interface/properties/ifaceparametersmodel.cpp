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

#include "aadlobject.h"
#include "aadlobjectiface.h"
#include "commandsstack.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"
#include "interface/commands/cmdentitypropertycreate.h"
#include "interface/commands/commandsfactory.h"

#include <QDebug>
#include <algorithm>

namespace ive {

IfaceParametersModel::IfaceParametersModel(cmd::CommandsStack::Macro *macro, QObject *parent)
    : PropertiesModelBase(parent)
    , m_cmdMacro(macro)
{
}

IfaceParametersModel::~IfaceParametersModel() { }

void IfaceParametersModel::createNewRow(const aadl::IfaceParameter &param, int row)
{
    QStandardItem *titleItem = new QStandardItem(param.name());
    QStandardItem *typeItem = new QStandardItem(param.paramTypeName());
    QStandardItem *encodingItem = new QStandardItem(param.encoding());
    QStandardItem *directionItem = new QStandardItem(aadl::IfaceParameter::directionName(param.direction()));

    setItem(row, ColumnName, titleItem);
    setItem(row, ColumnType, typeItem);
    setItem(row, ColumnEncoding, encodingItem);
    setItem(row, ColumnDirection, directionItem);

    m_params.insert(row, param);
}

void IfaceParametersModel::setDataObject(aadl::AADLObject *obj)
{
    clear();
    m_params.clear();
    m_dataObject = obj;

    if (!m_dataObject)
        return;

    if (auto iface = qobject_cast<aadl::AADLObjectIface *>(m_dataObject)) {
        const QVector<aadl::IfaceParameter> &params(iface->params());
        const int paramsCount = params.size();

        beginInsertRows(QModelIndex(), 0, paramsCount);

        for (int i = 0; i < paramsCount; ++i) {
            const aadl::IfaceParameter &param = params.at(i);
            createNewRow(param, i);
        }

        endInsertRows();
    }
}

const aadl::AADLObject *IfaceParametersModel::dataObject() const
{
    return m_dataObject;
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

    const aadl::IfaceParameter &param = m_params.at(index.row());
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
            return aadl::IfaceParameter::directionName(param.direction());
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
        const aadl::IfaceParameter &paramOld = m_params.at(index.row());
        aadl::IfaceParameter paramNew(paramOld);

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
            if (!paramNew.setDirection(aadl::IfaceParameter::directionFromName(value.toString())))
                return false;
            break;
        }
        default:
            return false;
        }

        if (const auto attributesCmd = cmd::CommandsFactory::create(cmd::ChangeIfaceParam,
                    { QVariant::fromValue(m_dataObject), QVariant::fromValue(paramOld),
                            QVariant::fromValue(paramNew) })) {

            m_cmdMacro->push(attributesCmd);
            m_params.replace(index.row(), paramNew);
        }
    }

    QStandardItemModel::setData(index, value, role);
    Q_EMIT dataChanged(index, index, { role, Qt::DisplayRole });
    return true;
}

bool IfaceParametersModel::createProperty(const QString &propName)
{
    bool res(false);

    aadl::IfaceParameter param(propName);

    const auto propsCmd = cmd::CommandsFactory::create(
            cmd::CreateIfaceParam, { QVariant::fromValue(m_dataObject), QVariant::fromValue(param) });
    if (propsCmd) {
        const int rows = rowCount();
        beginInsertRows(QModelIndex(), rows, rows);

        m_cmdMacro->push(propsCmd);
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
            cmd::RemoveIfaceParam, { QVariant::fromValue(m_dataObject), QVariant::fromValue(m_params.value(row)) });
    if (propsCmd) {
        m_cmdMacro->push(propsCmd);
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

Qt::ItemFlags IfaceParametersModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = PropertiesModelBase::flags(index);
    if (m_dataObject)
        if (auto iface = m_dataObject->as<const aadl::AADLObjectIface *>()) {
            if (iface->isClone()) {
                flags = flags & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;
            } else if (auto ri = iface->as<const aadl::AADLObjectIfaceRequired *>()) {
                if (ri->hasPrototypePi())
                    flags = flags & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;
            }
        }
    return flags;
}

}
