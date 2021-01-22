/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "colormanagerdialog.h"

#include "colormanager.h"
#include "colorsettingswidget.h"
#include "ui_colormanagerdialog.h"

#include <QPushButton>

namespace ive {

ColorManagerDialog::ColorManagerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ColorManagerDialog)
    , m_originalFile(ColorManager::instance()->sourceFile())
{
    ui->setupUi(this);
    setWindowTitle(tr("Color scheme"));
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &ColorManagerDialog::onDialogButtonClicked);
}

ColorManagerDialog::~ColorManagerDialog()
{
    delete ui;
}

void ColorManagerDialog::accept()
{
    ui->widget->saveSettings();

    QDialog::accept();
}

void ColorManagerDialog::reject()
{
    ColorManager::instance()->setSourceFile(m_originalFile);
    QDialog::reject();
}

void ColorManagerDialog::onDialogButtonClicked(QAbstractButton *button)
{
    if (ui->buttonBox->button(QDialogButtonBox::RestoreDefaults) == button) {
        ui->widget->loadFile(ColorManager::defaultColorsResourceFile());
    }
}

}
