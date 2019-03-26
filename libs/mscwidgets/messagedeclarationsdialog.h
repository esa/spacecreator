/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

namespace msc {
class MscModel;
class MscMessageDeclaration;
class MscMessageDeclarationList;
}

namespace Ui {
class MessageDeclarationsDialog;
}

class MessageDeclarationsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDeclarationsDialog(msc::MscMessageDeclarationList *model, QWidget *parent = nullptr);
    ~MessageDeclarationsDialog();

    msc::MscMessageDeclarationList *declarations() const;

private Q_SLOTS:
    void showSelectedMessage();
    void addDeclaration();
    void removeDeclaration();

    void updateNames();

private:
    Ui::MessageDeclarationsDialog *ui;
    msc::MscMessageDeclarationList *m_model = nullptr;
    msc::MscMessageDeclaration *m_selectedDeclaration = nullptr;
};
