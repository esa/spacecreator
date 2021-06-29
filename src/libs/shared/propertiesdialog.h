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

#include <QDialog>
#include <memory>

namespace shared {
class VEObject;
class PropertyTemplateConfig;

class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertiesDialog(PropertyTemplateConfig *dynPropConfig, VEObject *obj,
            cmd::CommandsStackBase *commandsStack, QWidget *parent = nullptr);
    ~PropertiesDialog() override;

public Q_SLOTS:
    void done(int r) override;

protected:
    void insertTab(QWidget *widget, const QString &tabName);

    virtual QString objectTypeName() const = 0;
    virtual void init();
    virtual VEObject *dataObject() const;

    cmd::CommandsStackBase::Macro *commandMacro() const;
    cmd::CommandsStackBase *commandStack() const;
    PropertyTemplateConfig *propertiesConfig() const;

private:
    struct PropertiesDialogPrivate;
    std::unique_ptr<PropertiesDialogPrivate> d;
};

} // namespace shared
