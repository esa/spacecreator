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

#include "propertytemplatemanager.h"

#include "baseitems/common/aadlutils.h"
#include "propertytemplateconfig.h"
#include "ui_propertytemplatemanager.h"

#include <QDialogButtonBox>
#include <QPushButton>

namespace ive {

PropertyTemplateManager::PropertyTemplateManager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PropertyTemplateManager)
{
    ui->setupUi(this);

    connect(ui->widget, &PropertyTemplateWidget::hasErrorChanged, this,
            [this]() { ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ui->widget->hasError()); });
}

PropertyTemplateManager::~PropertyTemplateManager()
{
    delete ui;
}

void PropertyTemplateManager::accept()
{
    ui->widget->save();
    auto config = ivm::PropertyTemplateConfig::instance();
    config->init(ive::dynamicPropertiesFilePath());

    QDialog::accept();
}

}
