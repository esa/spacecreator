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

#include "aadliface.h"
#include "aadlobject.h"
#include "commandsstack.h"
#include "interface/commands/cmdifaceparamchange.h"
#include "interface/commands/cmdifaceparamcreate.h"
#include "interface/commands/cmdifaceparamremove.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"

#include <QDebug>
#include <algorithm>

namespace ive {

IfaceParametersModel::IfaceParametersModel(cmd::CommandsStack::Macro *macro, QObject *parent)
    : PropertiesModelBase(parent)
    , m_cmdMacro(macro)
{
}

IfaceParametersModel::~IfaceParametersModel() { }

void IfaceParametersModel::createNewRow(const ivm::IfaceParameter &param, int row)
{
    QStandardItem *titleItem = new QStandardItem(param.name());
    QStandardItem *typeItem = new QStandardItem(param.paramTypeName());
    QStandardItem *encodingItem = new QStandardItem(param.encoding());
    QStandardItem *directionItem = new QStandardItem(ivm::IfaceParameter::directionName(param.direction()));

    setItem(row, Column::Name, titleItem);
    setItem(row, Column::Type, typeItem);
    setItem(row, Column::Encoding, encodingItem);
    setItem(row, Column::Direction, directionItem);

    m_params.insert(row, param);
}

void IfaceParametersModel::setDataObject(ivm::AADLObject *obj)
{
    clear();
    m_params.clear();
    m_dataObject = obj;

    if (!m_dataObject)
        return;

    if (auto iface = qobject_cast<ivm::AADLIface *>(m_dataObject)) {
        const QVector<ivm::IfaceParameter> &params(iface->params());
        const int paramsCount = params.size();

        beginInsertRows(QModelIndex(), 0, paramsCount);

        for (int i = 0; i < paramsCount; ++i) {
            const ivm::IfaceParameter &param = params.at(i);
            createNewRow(param, i);
        }

        endInsertRows();
    }
}

const ivm::AADLObject *IfaceParametersModel::dataObject() const
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

    const ivm::IfaceParameter &param = m_params.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole: {
        switch (index.column()) {
        case Column::Name:
            return param.name();
        case Column::Type:
            return param.paramTypeName();
        case Column::Encoding:
            return param.encoding();
        case Column::Direction:
            return ivm::IfaceParameter::directionName(param.direction());
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
        const ivm::IfaceParameter &paramOld = m_params.at(index.row());
        ivm::IfaceParameter paramNew(paramOld);

        switch (index.column()) {
        case Column::Name: {
            if (!paramNew.setName(value.toString()))
                return false;
            break;
        }
        case Column::Type: {
            if (!paramNew.setParamTypeName(value.toString()))
                return false;
            break;
        }
        case Column::Encoding: {
            if (!paramNew.setEncoding(value.toString()))
                return false;
            break;
        }
        case Column::Direction: {
            if (!paramNew.setDirection(ivm::IfaceParameter::directionFromName(value.toString())))
                return false;
            break;
        }
        default:
            return false;
        }

        auto attributesCmd = new cmd::CmdIfaceParamChange(m_dataObject, paramOld, paramNew);
        m_cmdMacro->push(attributesCmd);
        m_params.replace(index.row(), paramNew);
    }

    QStandardItemModel::setData(index, value, role);
    Q_EMIT dataChanged(index, index, { role, Qt::DisplayRole });
    return true;
}

bool IfaceParametersModel::createProperty(const QString &propName)
{
    ivm::IfaceParameter param(propName);

    auto propsCmd = new cmd::CmdIfaceParamCreate(m_dataObject, param);
    const int rows = rowCount();
    beginInsertRows(QModelIndex(), rows, rows);

    m_cmdMacro->push(propsCmd);
    createNewRow(param, rows);

    endInsertRows();

    return true;
}

bool IfaceParametersModel::removeProperty(const QModelIndex &index)
{
    if (!index.isValid()) {
        return false;
    }

    const int row(index.row());
    auto propsCmd = new cmd::CmdIfaceParamRemove(m_dataObject, m_params.value(row));
    m_cmdMacro->push(propsCmd);
    removeRow(row);
    m_params.removeAt(row);

    return true;
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
        case Column::Name:
            return tr("Name");
        case Column::Type:
            return tr("Type");
        case Column::Encoding:
            return tr("Encoding Protocol");
        case Column::Direction:
            return tr("Direction");
        }
    }
    return QVariant();
}

Qt::ItemFlags IfaceParametersModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = PropertiesModelBase::flags(index);
    if (m_dataObject)
        if (auto iface = m_dataObject->as<const ivm::AADLIface *>()) {
            if (iface->isClone()) {
                flags = flags & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;
            } else if (auto ri = iface->as<const ivm::AADLIfaceRequired *>()) {
                if (ri->hasPrototypePi())
                    flags = flags & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;
            }
        }
    return flags;
}

}
