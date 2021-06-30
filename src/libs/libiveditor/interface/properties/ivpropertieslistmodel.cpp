/*
  Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivpropertieslistmodel.h"

#include "ivcommonprops.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivpropertytemplateconfig.h"

#include <QDebug>
#include <QRegularExpression>

namespace ive {

ivm::meta::Props::Token IVPropertiesListModel::tokenFromIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return ivm::meta::Props::Token::Unknown;

    return ivm::meta::Props::token(tokenNameFromIndex(index));
}

IVPropertiesListModel::IVPropertiesListModel(
        cmd::CommandsStack::Macro *macro, shared::PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : shared::PropertiesListModel(macro, dynPropConfig, parent)
{
}

IVPropertiesListModel::~IVPropertiesListModel() { }

bool IVPropertiesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    const QVariant currentValue = data(index, role);
    if (currentValue == value) {
        if (role != DataRole || !currentValue.isNull()) {
            return false;
        }
    }

    QVariant attributeValue = value;
    if (isAttr(index) && index.column() == Column::Value) {
        switch (tokenFromIndex(index)) {
        case ivm::meta::Props::Token::name: {
            const QString newName = ivm::IVNameValidator::encodeName(entity()->type(), value.toString());
            if (!ivm::IVNameValidator::isAcceptableName(entity(), newName)) {
                return false;
            }
            attributeValue = newName;
            break;
        }
        default:
            break;
        }
    }

    return shared::PropertiesListModel::setData(index, attributeValue, role);
}

ivm::IVObject *IVPropertiesListModel::entity() const
{
    return qobject_cast<ivm::IVObject *>(PropertiesModelBase::entity());
}

FunctionPropertiesListModel::FunctionPropertiesListModel(
        cmd::CommandsStack::Macro *macro, shared::PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : IVPropertiesListModel(macro, dynPropConfig, parent)
{
}

QVariant FunctionPropertiesListModel::data(const QModelIndex &index, int role) const
{
    if (index.column() == Column::Value && role == EditRole
            && tokenFromIndex(index) == ivm::meta::Props::Token::instance_of) {
        QStringList availableFnTypes { QString() };
        if (auto fn = entity()->as<const ivm::IVFunction *>()) {
            availableFnTypes << fn->model()->getAvailableFunctionTypes(fn).keys();
        }
        return availableFnTypes;
    }

    return PropertiesListModel::data(index, role);
}

bool FunctionPropertiesListModel::isEditable(const QModelIndex &index) const
{
    if (!entity() || !index.isValid() || !PropertiesListModel::isEditable(index))
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
        if (entity()->isFunctionType() || index.column() == Column::Name)
            editable = false;
        else {
            if (auto fn = entity()->as<const ivm::IVFunction *>()) {
                editable = fn->instanceOf() || fn->interfaces().isEmpty();
            }
        }
        break;
    }
    default:
        if (auto fn = entity()->as<const ivm::IVFunction *>())
            editable = !fn->inheritsFunctionType();
        break;
    }

    return editable;
}

InterfacePropertiesListModel::InterfacePropertiesListModel(
        cmd::CommandsStack::Macro *macro, shared::PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : IVPropertiesListModel(macro, dynPropConfig, parent)
{
}

QVariant InterfacePropertiesListModel::data(const QModelIndex &index, int role) const
{
    if ((role == shared::PropertiesListModel::DataRole || role == shared::PropertiesListModel::EditRole)
            && index.column() == Column::Value) {
        if (entity()->type() == ivm::IVObject::Type::RequiredInterface
                && tokenFromIndex(index) == ivm::meta::Props::Token::name) {
            return entity()->as<ivm::IVInterfaceRequired *>()->ifaceLabel();
        }
    }
    return PropertiesListModel::data(index, role);
}

bool InterfacePropertiesListModel::isEditable(const QModelIndex &index) const
{
    if (!entity() || !index.isValid() || !PropertiesListModel::isEditable(index))
        return false;

    if (auto iface = entity()->as<const ivm::IVInterface *>()) {
        const bool isClone = iface->isClone();
        switch (tokenFromIndex(index)) {
        case ivm::meta::Props::Token::name:
        case ivm::meta::Props::Token::InheritPI:
            return !isClone;
        default:
            if (iface->isRequired()) {
                if (auto ri = iface->as<const ivm::IVInterfaceRequired *>()) {
                    return !isClone && !ri->hasPrototypePi();
                }
            }
            return !isClone;
        }
    }

    return true;
}
}
