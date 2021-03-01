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

#include "coloroption.h"
#include "colors/colormanager.h"
#include "colors/colorsettingswidget.h"
#include "spacecreatorpluginconstants.h"

#include <QBoxLayout>
#include <QPushButton>

namespace spctr {

ColorOption::ColorOption(QObject *parent)
    : Core::IOptionsPage(parent)
    , m_originalFile(shared::ColorManager::instance()->sourceFile())
{
    setId(Constants::SETTINGS_IV_COLOR_ID);
    setDisplayName(tr("Colors"));
    setCategory(Constants::SETTINGS_CATEGORY);

    setDisplayCategory(tr(Constants::SETTINGS_CATEGORY_DISPLAY));
}

QWidget *ColorOption::widget()
{
    if (!m_widget) {
        m_widget = new QWidget();
        auto mainLayout = new QVBoxLayout(m_widget);
        m_widget->setLayout(mainLayout);

        m_colorWidget = new shared::ColorSettingsWidget(m_widget);
        mainLayout->addWidget(m_colorWidget);

        auto buttonLayout = new QHBoxLayout(m_widget);
        mainLayout->addLayout(buttonLayout);
        auto restoreButton = new QPushButton(tr("Restore Defaults"), m_widget);
        buttonLayout->addWidget(restoreButton);
        buttonLayout->addStretch(1);
        connect(restoreButton, &QPushButton::clicked, this, &ColorOption::restoreDefaults);
    }
    return m_widget;
}

void ColorOption::apply()
{
    m_reset = false;
    if (m_colorWidget) {
        m_colorWidget->saveSettings();
    }
}

void ColorOption::finish()
{
    if (m_reset) {
        shared::ColorManager::instance()->setSourceFile(m_originalFile);
    }
    delete m_widget;
}

void ColorOption::restoreDefaults()
{
    if (m_colorWidget) {
        m_colorWidget->loadFile(shared::ColorManager::defaultColorsResourceFile());
    }
}

}
