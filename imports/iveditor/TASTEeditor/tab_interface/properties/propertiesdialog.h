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

#include "app/commandsstack.h"

#include <QDialog>

namespace Ui {
class PropertiesDialog;
}

namespace taste3 {
namespace aadl {

class AADLObject;
class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertiesDialog(AADLObject *obj, QWidget *parent = nullptr);
    ~PropertiesDialog() override;

public Q_SLOTS:
    void open() override;
    void done(int r) override;

private:
    Ui::PropertiesDialog *ui;
    AADLObject *m_dataObject { nullptr };

    QString objectTypeName() const;

    void initTabs();

    taste3::cmd::CommandsStack::Macro *m_cmdMacro { nullptr };
};

} // namespace aadl
} // namespace taste3
