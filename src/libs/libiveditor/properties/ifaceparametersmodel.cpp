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

#include "commands/cmdifaceparamchange.h"
#include "commands/cmdifaceparamcreate.h"
#include "commands/cmdifaceparamremove.h"
#include "commandsstack.h"
#include "ivinterface.h"
#include "ivobject.h"
#include "ivpropertytemplate.h"
#include "propertytemplateconfig.h"

#include <QApplication>
#include <QMessageBox>
#include <algorithm>

namespace ive {

IfaceParametersModel::IfaceParametersModel(
        cmd::CommandsStack::Macro *macro, const QStringList &asn1Names, QObject *parent)
    : shared::PropertiesModelBase(parent)
    , m_cmdMacro(macro)
    , m_asn1Names(asn1Names)
{
}

IfaceParametersModel::~IfaceParametersModel() {}

void IfaceParametersModel::createNewRow(const shared::InterfaceParameter &param, int row)
{
    m_params.insert(row, param);

    QStandardItem *titleItem = new QStandardItem(row, Column::Name);
    titleItem->setData(param.name(), Qt::DisplayRole);
    titleItem->setData(param.name(), DataRole);
    setItem(row, Column::Name, titleItem);

    QStandardItem *typeItem = new QStandardItem(row, Column::Type);
    typeItem->setData(param.paramTypeName(), DataRole);
    typeItem->setData(m_asn1Names, EditRole);
    setItem(row, Column::Type, typeItem);

    QStandardItem *encodingItem = new QStandardItem(row, Column::Encoding);
    encodingItem->setData(param.encoding(), DataRole);
    encodingItem->setData(QStringList { tr("NATIVE"), tr("UPER"), tr("ACN") }, EditRole); // TODO: is it configurable?
    setItem(row, Column::Encoding, encodingItem);

    QStandardItem *directionItem = new QStandardItem(row, Column::Direction);
    directionItem->setData(shared::typeName(param.direction()), DataRole);
    directionItem->setData(QStringList { shared::typeName(shared::InterfaceParameter::Direction::IN),
                                   shared::typeName(shared::InterfaceParameter::Direction::OUT) },
            EditRole);
    setItem(row, Column::Direction, directionItem);
}

void IfaceParametersModel::setDataObject(shared::VEObject *obj)
{
    clear();
    m_params.clear();
    m_dataObject = obj;

    if (!m_dataObject)
        return;

    if (auto iface = qobject_cast<ivm::IVInterface *>(m_dataObject)) {
        const QVector<shared::InterfaceParameter> &params(iface->params());
        const int paramsCount = params.size();

        for (int i = 0; i < paramsCount; ++i) {
            const shared::InterfaceParameter &param = params.at(i);
            createNewRow(param, i);
        }
    }
}

int IfaceParametersModel::columnCount(const QModelIndex &) const
{
    static const QMetaEnum &me = QMetaEnum::fromType<Column>();
    return me.keyCount();
}

bool IfaceParametersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || data(index, role) == value || !m_dataObject)
        return false;

    if (role == DataRole || role == Qt::EditRole) {
        const shared::InterfaceParameter &paramOld = m_params.value(index.row());
        shared::InterfaceParameter paramNew(paramOld);

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
            auto dir = shared::typeFromName<shared::InterfaceParameter::Direction>(value.toString());
            if (auto iface = m_dataObject->as<ivm::IVInterface *>()) {
                if (iface->kind() == ivm::IVInterface::OperationKind::Sporadic
                        && dir == shared::InterfaceParameter::Direction::OUT) {
                    QMessageBox::warning(qApp->activeWindow(), tr("Direction change"),
                            tr("It's not possible to set direction to \"OUT\" for Sporadic Interface"));
                    return false;
                }
            }
            if (!paramNew.setDirection(dir))
                return false;
            break;
        }
        default:
            return false;
        }

        auto attributesCmd = new cmd::CmdIfaceParamChange(entity(), paramOld, paramNew);
        m_cmdMacro->push(attributesCmd);
        m_params.replace(index.row(), paramNew);
    }

    return QStandardItemModel::setData(index, value, role);
}

bool IfaceParametersModel::createProperty(const QString &propName)
{
    shared::InterfaceParameter param(propName);
    if (!m_asn1Names.isEmpty()) {
        param.setParamTypeName(m_asn1Names.front());
    }

    auto propsCmd = new cmd::CmdIfaceParamCreate(entity(), param);
    const int rows = rowCount();

    createNewRow(param, rows);
    m_cmdMacro->push(propsCmd);

    return true;
}

bool IfaceParametersModel::removeProperty(const QModelIndex &index)
{
    if (!index.isValid()) {
        return false;
    }

    const int row(index.row());
    auto propsCmd = new cmd::CmdIfaceParamRemove(entity(), m_params.value(row));
    removeRow(row);
    m_params.removeAt(row);
    m_cmdMacro->push(propsCmd);

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
        if (auto iface = m_dataObject->as<const ivm::IVInterface *>()) {
            if (iface->isClone()) {
                flags = flags & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;
            } else if (auto ri = iface->as<const ivm::IVInterfaceRequired *>()) {
                if (ri->hasPrototypePi())
                    flags = flags & ~Qt::ItemIsEditable & ~Qt::ItemIsEnabled;
            }
        }
    return flags;
}

ivm::IVInterface *IfaceParametersModel::entity() const
{
    return qobject_cast<ivm::IVInterface *>(PropertiesModelBase::entity());
}

bool IfaceParametersModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
        const QModelIndex &destinationParent, int destinationChild)
{
    if (PropertiesModelBase::moveRows(sourceParent, sourceRow, count, destinationParent, destinationChild)) {
        for (int idx = 0; idx < count; ++idx)
            std::swap(m_params[sourceRow + idx], m_params[destinationChild + idx]);
        return true;
    }
    return false;
}

}
