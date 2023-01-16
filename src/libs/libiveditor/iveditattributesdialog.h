/*
   Copyright (C) 2022-2023 European Space Agency - <maxime.perrotin@esa.int>

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

class QAbstractItemModel;

namespace ive
{

/*!
 * \brief Dialog for editing attributes of functions and interfaces.
 */
class IVEditAttributesDialog : public QDialog {
public:
    explicit IVEditAttributesDialog(QAbstractItemModel* functionsModel, QAbstractItemModel* interfacesModel, QWidget *parent);
    ~IVEditAttributesDialog();

private:
    struct Private;
    Private* d;
};

}
