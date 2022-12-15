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

#include "commands/cmdentityattributeschange.h"
#include "commands/cmdfunctionattrchange.h"
#include "commands/cmdifaceattrchange.h"
#include "ivcommonprops.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivpropertytemplateconfig.h"

#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QRegularExpression>

namespace ive {

ivm::meta::Props::Token IVPropertiesListModel::tokenFromIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return ivm::meta::Props::Token::Unknown;

    return ivm::meta::Props::token(tokenNameFromIndex(index));
}

QPair<QString, QVariant> IVPropertiesListModel::prepareDataForUpdate(
        const QModelIndex &index, const QVariant &value, int role) const
{
    if (!index.isValid())
        return {};

    const QVariant currentValue = data(index, role);
    if (currentValue == value) {
        if (role != DataRole || !currentValue.isNull()) {
            return {};
        }
    }

    if ((role == DataRole || role == Qt::EditRole) && index.column() == Column::Value) {
        const QString &name = tokenNameFromIndex(index);
        if (m_dataObject->entityAttributeValue(name) == value) {
            return {};
        }

        if (!isValueValid(name, value.toString())) {
            return {};
        }

        return { name, value };
    }
    return {};
}

bool IVPropertiesListModel::isEditable(const QModelIndex &index) const
{
    if (!entity() || !index.isValid() || !PropertiesListModel::isEditable(index))
        return false;

    bool editable = true;
    switch (tokenFromIndex(index)) {
    case ivm::meta::Props::Token::is_type:
    case ivm::meta::Props::Token::fixed_system_element:
    case ivm::meta::Props::Token::required_system_element:
        editable = false;
        break;
    default:
        break;
    }
    return editable;
}

IVPropertiesListModel::IVPropertiesListModel(
        cmd::CommandsStack::Macro *macro, shared::PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : shared::PropertiesListModel(macro, dynPropConfig, parent)
{
}

IVPropertiesListModel::~IVPropertiesListModel() { }

QVariant IVPropertiesListModel::data(const QModelIndex &index, int role) const
{
    if (role == TokenRole) {
        return QVariant::fromValue(tokenFromIndex(index));
    }

    return PropertiesListModel::data(index, role);
}

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

    return IVPropertiesListModel::data(index, role);
}

bool FunctionPropertiesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    const QPair<QString, QVariant> data = prepareDataForUpdate(index, value, role);
    if (!data.first.isEmpty()) {
        if (isAttr(index)) {
            return m_cmdMacro->push(new cmd::CmdFunctionAttrChange(m_propTemplatesConfig, entity(),
                    { EntityAttribute { data.first, data.second, EntityAttribute::Type::Attribute } }));
        }
        return m_cmdMacro->push(new shared::cmd::CmdEntityAttributesChange(m_propTemplatesConfig, entity(),
                { EntityAttribute { data.first, data.second, EntityAttribute::Type::Property } }));
    }
    return false;
}

ivm::IVFunction *FunctionPropertiesListModel::entity() const
{
    return qobject_cast<ivm::IVFunction *>(m_dataObject);
}

bool FunctionPropertiesListModel::isEditable(const QModelIndex &index) const
{
    if (!entity() || !index.isValid() || !IVPropertiesListModel::isEditable(index))
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

InterfacePropertiesListModel::InterfacePropertiesListModel(cmd::CommandsStack::Macro *macro,
        shared::PropertyTemplateConfig *dynPropConfig, ivm::IVModel *layersModel, QObject *parent)
    : IVPropertiesListModel(macro, dynPropConfig, parent)
    , m_layersModel(layersModel)
{
}

void InterfacePropertiesListModel::setDataObject(shared::VEObject *obj)
{
    shared::PropertiesListModel::setDataObject(obj);

    QStringList layerTitles;
    auto layers = m_layersModel->allObjectsByType<ivm::IVConnectionLayerType>();
    for (auto *const layer : layers) {
        layerTitles.append(QString(layer->title()));
    }

    for (int i = 0; i < rowCount(); i++) {
        QStandardItem *titleItem = item(i, Column::Name);

        if (titleItem->data(Roles::DataRole) == ivm::meta::Props::token(ivm::meta::Props::Token::layer)) {
            QStandardItem *itemObj = item(i, Column::Value);
            itemObj->setData(QVariant(layerTitles), Roles::EditRole);
        }
    }

    ivm::IVInterfaceProvided* interface = obj->as<ivm::IVInterfaceProvided*>();

    if(interface == nullptr)
    {
        return;
    }

    if(interface->entityAttributeValue(ivm::meta::Props::token(ivm::meta::Props::Token::is_qgen_interface)) == false)
    {
        return;
    }

    ivm::IVFunction* function = interface->function()->as<ivm::IVFunction*>();
    if(function == nullptr)
    {
        return;
    }

    QStringList interfacesTitles;
    auto interfaces = function->interfaces();
    for (auto *const interface : interfaces) {
        if(interface->entityAttributeValue(ivm::meta::Props::token(ivm::meta::Props::Token::is_qgen_interface)) == true &&
            interface->entityAttributeValue(ivm::meta::Props::token(ivm::meta::Props::Token::qgen_interface_type)).toString() ==
             ivm::meta::Props::token(ivm::meta::Props::Token::Full))
        {
            interfacesTitles.append(QString(interface->title()));
        }
    }

    for (int i = 0; i < rowCount(); i++) {
        QStandardItem *titleItem = item(i, Column::Name);

        if (titleItem->data(Roles::DataRole) == ivm::meta::Props::token(ivm::meta::Props::Token::qgen_full_interface_ref)) {
            QStandardItem *itemObj = item(i, Column::Value);
            itemObj->setData(QVariant(interfacesTitles), Roles::EditRole);
        }
    }
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
    return IVPropertiesListModel::data(index, role);
}

static inline bool hasOutParameter(const QVector<shared::InterfaceParameter> &paramsList)
{
    auto it = std::find_if(paramsList.cbegin(), paramsList.cend(),
            [](const shared::InterfaceParameter &param) { return param.isOutDirection(); });
    return it != paramsList.cend();
}

bool InterfacePropertiesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (isAttr(index) && index.column() == Column::Value) {
        if (ivm::meta::Props::Token::kind == tokenFromIndex(index)) {
            const auto kindType = shared::typeFromName<ivm::IVInterface::OperationKind>(value.toString());
            const QVector<shared::InterfaceParameter> paramsList = entity()->params();
            if (kindType == ivm::IVInterface::OperationKind::Cyclic) {
                if (!paramsList.isEmpty()) {
                    QMessageBox::warning(qApp->activeWindow(), tr("Interface change"),
                            tr("It's not possible to set Kind to \"Cyclic\" for Interface that has parameter(s)"));
                    return false;
                }
            } else if (kindType == ivm::IVInterface::OperationKind::Sporadic) {
                if (paramsList.size() > 1) {
                    QMessageBox::warning(qApp->activeWindow(), tr("Interface change"),
                            tr("It's not possible to set Kind to \"Sporadic\" for Interface that has more than one "
                               "parameter"));
                    return false;
                } else if (hasOutParameter(paramsList)) {
                    QMessageBox::warning(qApp->activeWindow(), tr("Interface change"),
                            tr("It's not possible to set Kind to \"Sporadic\" for Interface that has at least one "
                               "\"OUT\" parameter"));
                    return false;
                }
            }
        }
    }

    const QPair<QString, QVariant> data = prepareDataForUpdate(index, value, role);
    if (!data.first.isEmpty()) {
        return m_cmdMacro->push(new cmd::CmdIfaceAttrChange(m_propTemplatesConfig, entity(), data.first, data.second));
    }
    return false;
}

ivm::IVInterface *InterfacePropertiesListModel::entity() const
{
    return qobject_cast<ivm::IVInterface *>(m_dataObject);
}

bool InterfacePropertiesListModel::isEditable(const QModelIndex &index) const
{
    if (!entity() || !index.isValid() || !PropertiesListModel::isEditable(index))
        return false;

    if (auto iface = entity()->as<const ivm::IVInterface *>()) {
        const bool isClone = iface->isClone();
        switch (tokenFromIndex(index)) {
        case ivm::meta::Props::Token::name:
            if (iface->isRequiredSystemElement()) {
                return false;
            } // no break
        case ivm::meta::Props::Token::InheritPI:
            return !isClone;
        case ivm::meta::Props::Token::fixed_system_element:
        case ivm::meta::Props::Token::required_system_element:
            return false;
        case ivm::meta::Props::Token::kind:
            if (iface->isRequiredSystemElement()) {
                return false;
            } // no break
        default:
            if (iface->isRequired()) {
                if (auto ri = iface->as<const ivm::IVInterfaceRequired *>()) {
                    return !isClone && !ri->hasPrototypePi();
                }
            }
            if (index.column() == Column::Name) {
                return !isClone;
            }
            break;
        }
    }

    return true;
}
}
