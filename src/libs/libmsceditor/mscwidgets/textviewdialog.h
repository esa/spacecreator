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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include <QDialog>

class QAction;

namespace msc {

class MscModel;

/*!
  \class msc::TextViewDialog
  \inmodule MscWidgets

  A text view of the current model. The text is the same as if it would be saved to a file.
*/
class TextViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextViewDialog(QWidget *parent = nullptr);
    ~TextViewDialog() override;

    void setModel(msc::MscModel *model);

    //! Get an action to show or hide this dialog
    QAction *toggleViewAction();

protected:
    void closeEvent(QCloseEvent *) override;
    void showEvent(QShowEvent *) override;

private Q_SLOTS:
    void updateText();

private:
    struct TextViewDialogPrivate;
    TextViewDialogPrivate *d;
};

}
