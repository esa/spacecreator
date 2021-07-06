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

#pragma once

#include "commandsstackbase.h"
#include "propertiesmodelbase.h"
#include "propertytemplate.h"

namespace shared {

class PropertyTemplate;
class PropertyTemplateConfig;

class PropertiesListModel : public PropertiesModelBase
{
    Q_OBJECT
public:
    enum Column
    {
        Name = 0,
        Value
    };

    explicit PropertiesListModel(cmd::CommandsStackBase::Macro *macro, shared::PropertyTemplateConfig *dynPropConfig,
            QObject *parent = nullptr);
    ~PropertiesListModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = DataRole) override;

    void setDataObject(VEObject *obj) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool createProperty(const QString &propName) override;
    bool removeProperty(const QModelIndex &index) override;

    bool isAttr(const QModelIndex &id) const override;
    bool isProp(const QModelIndex &id) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    static QString tokenNameFromIndex(const QModelIndex &index);

protected:
    cmd::CommandsStackBase::Macro *m_cmdMacro { nullptr };
    shared::PropertyTemplateConfig *m_propTemplatesConfig { nullptr };
    QStringList m_names;

    virtual bool isEditable(const QModelIndex &idx) const;
    void createNewRow(int row, const QString &name);
    void updateRows(const QList<shared::PropertyTemplate *> &templates);

    void invalidateAttributes(const QString &attrName);
    bool isValueValid(const QString &name, const QString &value) const;

private:
    struct RowData {
        int row { -1 };
        shared::PropertyTemplate::Info info;
        QString label;
        QString name;
        QVariant value;
        QVariant editValue;
        QString rxValidator;
    };

private:
    void updateRow(const RowData &data);
    QStringList sortedKeys(const QList<shared::PropertyTemplate *> &templates) const;
};

} // namespace shared
