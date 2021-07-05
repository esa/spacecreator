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

#include "propertytemplatewidget.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace shared {

PropertyTemplateManager::PropertyTemplateManager(const QList<PropertyTemplateConfig *> &configs, QWidget *parent)
    : QDialog(parent)
{
    auto vLayout = new QVBoxLayout;

    auto widget = new PropertyTemplateWidget(this);
    widget->setPropertyTemplateConfigs(configs);
    vLayout->addWidget(widget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
    vLayout->addWidget(buttonBox);

    setLayout(vLayout);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &PropertyTemplateManager::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PropertyTemplateManager::reject);
    connect(this, &PropertyTemplateManager::accepted, widget, &PropertyTemplateWidget::save);
    connect(widget, &PropertyTemplateWidget::hasErrorChanged, buttonBox->button(QDialogButtonBox::Save),
            &QPushButton::setDisabled);
}

} // namespace shared
