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

#pragma once

#include <QWidget>

namespace Ui {
class DVHWLibraryWidget;
}

namespace dve {

class DVHWLibraryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DVHWLibraryWidget(QWidget *parent = nullptr);
    ~DVHWLibraryWidget();

    QString path() const;
    void save();

private:
    Ui::DVHWLibraryWidget *ui;
};

} // namespace dve
