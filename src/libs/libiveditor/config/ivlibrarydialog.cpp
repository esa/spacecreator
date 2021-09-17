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

#include "ivlibrarydialog.h"

#include "ivlibrarywidget.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace ive {

IVLibraryDialog::IVLibraryDialog(QWidget *parent)
    : QDialog(parent)
{
    auto vbLayout = new QVBoxLayout;
    auto btnBox = new QDialogButtonBox(QDialogButtonBox::Apply | QDialogButtonBox::Cancel, this);
    auto widget = new IVLibraryWidget(this);
    connect(btnBox, &QDialogButtonBox::accepted, widget, &IVLibraryWidget::save);

    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(btnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    vbLayout->addWidget(widget);
    vbLayout->addWidget(btnBox);
    setLayout(vbLayout);
}

} // namespace ive
