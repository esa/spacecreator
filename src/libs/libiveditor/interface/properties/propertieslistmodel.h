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

#pragma once

#include "aadlcommonprops.h"
#include "commandsstack.h"
#include "propertiesmodelbase.h"
#include "propertytemplate.h"

#include <QVector>

namespace aadl {
class AADLObject;
class PropertyTemplateConfig;
}

namespace aadlinterface {

class PropertiesListModel : public PropertiesModelBase
{
    Q_OBJECT

public:
    static const int PropertyInfoRole { Qt::UserRole + 2 };
    static const int PropertyNameRole { PropertyInfoRole + 1 };
    static const int PropertyDataRole { PropertyNameRole + 1 };
    static const int ColumnTitle { 0 };
    static const int ColumnValue { 1 };

    explicit PropertiesListModel(
            cmd::CommandsStack::Macro *macro, aadl::PropertyTemplateConfig *dynPropConfig, QObject *parent = nullptr);
    ~PropertiesListModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setDataObject(aadl::AADLObject *obj);
    const aadl::AADLObject *dataObject() const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool createProperty(const QString &propName) override;
    bool removeProperty(const QModelIndex &index) override;

    bool isAttr(const QModelIndex &id) const override;
    bool isProp(const QModelIndex &id) const override;

protected:
    cmd::CommandsStack::Macro *m_cmdMacro { nullptr };
    aadl::PropertyTemplateConfig *m_propTamplesConfig { nullptr };
    aadl::AADLObject *m_dataObject { nullptr };
    QVector<QString> m_names;

    virtual bool isEditable(const QModelIndex &idx) const;
    void createNewRow(int row, const QString &label, const QString &name, aadl::PropertyTemplate::Info info,
            const QVariant &value, const QVariant &editValue, const QVariant &defaulValue);

    void invalidateProperties(const QString &propName);
    void invalidateAttributes(const QString &attrName);
};

class FunctionPropertiesListModel : public PropertiesListModel
{
public:
    explicit FunctionPropertiesListModel(
            cmd::CommandsStack::Macro *macro, aadl::PropertyTemplateConfig *dynPropConfig, QObject *parent = nullptr);

protected:
    bool isEditable(const QModelIndex &index) const override;
};

class InterfacePropertiesListModel : public PropertiesListModel
{
public:
    explicit InterfacePropertiesListModel(
            cmd::CommandsStack::Macro *macro, aadl::PropertyTemplateConfig *dynPropConfig, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

protected:
    bool isEditable(const QModelIndex &index) const override;
};

}
