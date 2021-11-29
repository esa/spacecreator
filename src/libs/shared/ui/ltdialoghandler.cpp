/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ltdialoghandler.h"

ListTreeDialog *LTDialogButtonHandler::m_dialog = nullptr;

void LTDialogButtonHandler::setLTDialog(ListTreeDialog *dialog)
{
    m_dialog = dialog;

    QObject::connect(m_dialog->button(), &QPushButton::pressed, onButtonPressed);
}

void LTDialogButtonHandler::onButtonPressed()
{
    QStandardItemModel *const model = m_dialog->model();
    const unsigned int rows = model->rowCount();
    const unsigned int cols = model->columnCount();

    for (unsigned int i = 0; i < cols; i++) {
        for (unsigned int j = 0; j < rows; j++) {
            QStandardItem *const item = model->takeItem(j, i);
            if (item != nullptr && item->checkState() == Qt::Checked) {
                m_dialog->selectedItemsPtr()->append(item->text());
            }
        }
    }

    m_dialog->close();
}
