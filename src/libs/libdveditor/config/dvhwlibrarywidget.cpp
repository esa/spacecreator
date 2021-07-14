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

#include "dvhwlibrarywidget.h"

#include "common.h"
#include "settingsmanager.h"
#include "ui_dvhwlibrarywidget.h"

#include <QFileDialog>
#include <QFileInfo>

namespace dve {

DVHWLibraryWidget::DVHWLibraryWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DVHWLibraryWidget)
{
    ui->setupUi(this);

    ui->pathEdit->setText(shared::hwLibraryPath());

    connect(ui->pathButton, &QPushButton::clicked, this, [this]() {
        QFileInfo fi(path());
        QString exec = QFileDialog::getExistingDirectory(this, tr("Select HW Library"), fi.absolutePath());
        if (!exec.isEmpty()) {
            ui->pathEdit->setText(exec);
        }
    });
}

DVHWLibraryWidget::~DVHWLibraryWidget()
{
    delete ui;
}

QString DVHWLibraryWidget::path() const
{
    return ui->pathEdit->text();
}

void DVHWLibraryWidget::save()
{
    if (path() != shared::hwLibraryPath()) {
        shared::SettingsManager::store<QString>(shared::SettingsManager::DVE::HwLibraryFile, path());
    }
}

} // namespace dve
