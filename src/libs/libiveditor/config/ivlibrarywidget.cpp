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

#include "ivlibrarywidget.h"

#include "common.h"
#include "settingsmanager.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace ive {

IVLibraryWidget::IVLibraryWidget(QWidget *parent)
    : QWidget(parent)
    , m_lineEditExportedComponents(new QLineEdit(this))
    , m_lineEditSharedTypes(new QLineEdit(this))
{
    auto gridLayout = new QGridLayout;
    gridLayout->setColumnStretch(1, 1);

    const QString description { tr(
            "\"%1\" is set. Current value will be stored in settings but won't be used while environment "
            "variable is set") };
    auto label = new QLabel;
    QPalette pal = label->palette();
    pal.setColor(QPalette::WindowText, Qt::red);
    label->setPalette(pal);
    if (qEnvironmentVariableIsSet(shared::env::kComponentsLibrary)) {
        label->setText(description.arg(shared::env::kComponentsLibrary));
    }
    gridLayout->addWidget(label, 0, 0, 1, 3);
    m_lineEditExportedComponents->setText(shared::componentsLibraryPath());
    auto btn = new QPushButton(tr("..."));
    connect(btn, &QPushButton::clicked, this, [this]() {
        const QFileInfo fi(m_lineEditExportedComponents->text());
        const QString exec = QFileDialog::getExistingDirectory(this, tr("Select Component Library"), fi.absolutePath());
        if (!exec.isEmpty()) {
            m_lineEditExportedComponents->setText(exec);
        }
    });
    gridLayout->addWidget(new QLabel(tr("Component Library:")), 1, 0);
    gridLayout->addWidget(m_lineEditExportedComponents, 1, 1);
    gridLayout->addWidget(btn, 1, 2);

    label = new QLabel;
    label->setPalette(pal);
    if (qEnvironmentVariableIsSet(shared::env::kSharedTypesLibrary)) {
        label->setText(description.arg(shared::env::kSharedTypesLibrary));
    }
    gridLayout->addWidget(label, 2, 0, 1, 3);
    m_lineEditSharedTypes->setText(shared::sharedTypesPath());
    btn = new QPushButton(tr("..."));
    connect(btn, &QPushButton::clicked, this, [this]() {
        const QFileInfo fi(m_lineEditSharedTypes->text());
        const QString exec =
                QFileDialog::getExistingDirectory(this, tr("Select Shared Types Library"), fi.absolutePath());
        if (!exec.isEmpty()) {
            m_lineEditSharedTypes->setText(exec);
        }
    });
    gridLayout->addWidget(new QLabel(tr("Shared Types Library:")), 3, 0);
    gridLayout->addWidget(m_lineEditSharedTypes, 3, 1);
    gridLayout->addWidget(btn, 3, 2);

    gridLayout->addItem(new QSpacerItem(5, 5, QSizePolicy::Preferred, QSizePolicy::Expanding), 4, 0, 1, 3);
    setLayout(gridLayout);
}

QString IVLibraryWidget::componentsLibraryPath() const
{
    return m_lineEditExportedComponents->text();
}

QString IVLibraryWidget::sharedTypesPath() const
{
    return m_lineEditSharedTypes->text();
}

void ive::IVLibraryWidget::save()
{
    if (componentsLibraryPath() != shared::componentsLibraryPath()) {
        shared::SettingsManager::store<QString>(shared::SettingsManager::IVE::ComponentsPath, componentsLibraryPath());
    }

    if (sharedTypesPath() != shared::sharedTypesPath()) {
        shared::SettingsManager::store<QString>(shared::SettingsManager::IVE::SharedTypesPath, componentsLibraryPath());
    }
}

} // namespace ive
