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

#include <QDialog>

namespace Ui {
class AddDynamicPropertyDialog;
}

namespace taste3 {
namespace aadl {

class DynamicProperty;
class AddDynamicPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDynamicPropertyDialog(const QStringList &prohibitedNames, QWidget *parent = nullptr);
    ~AddDynamicPropertyDialog() override;

    DynamicProperty *attribute() const;
public slots:
    void accept() override;

private:
    Ui::AddDynamicPropertyDialog *ui;
    QStringList m_prohibitedNames;
    QColor m_nameColorDefault;
    DynamicProperty *m_attr;
    bool validateName(bool showWarn);
    bool validateType();
    bool validateValuesList();
    bool validateScope();

    QStringList listValues() const;
};

} // ns aadl
} // ns taste3
