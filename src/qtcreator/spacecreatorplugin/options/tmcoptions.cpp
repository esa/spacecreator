/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "tmcoptions.h"

#include "spacecreatorpluginconstants.h"
#include "tmcoptionswidget.h"

#include <QDir>
#include <QSettings>
#include <tmc/TmcConfig/constants.h>

namespace spctr {

TmcOptions::TmcOptions(QObject *parent)
    : Core::IOptionsPage(parent)
{
    setId(Constants::SETTINGS_TMC_ID);
    setDisplayName(tr("Taste Model Checker settings"));
    setCategory(Constants::SETTINGS_CATEGORY);

    setDisplayCategory(tr(Constants::SETTINGS_CATEGORY_DISPLAY));
}

QWidget *TmcOptions::widget()
{
    if (!m_widget) {
        m_widget = new TmcOptionsWidget();

        QSettings settings;
        QVariant spinExecutable = settings.value(tmc::TmcConstants::SETTINGS_TMC_SPIN_EXE_KEY);
        if (!spinExecutable.isValid()) {
            spinExecutable = QString("spin");
        }
        m_widget->setSpinExecutable(spinExecutable.toString());

        QVariant defaultOutputDirectory = settings.value(tmc::TmcConstants::SETTINGS_TMC_SPIN_DEFAULT_OUTPUT_DIRECTORY);
        if(!defaultOutputDirectory.isValid()) {
            defaultOutputDirectory = QDir::homePath();
        }
        m_widget->setDefaultOutputDirectory(defaultOutputDirectory.toString());
    }
    return m_widget;
}

void TmcOptions::apply()
{
    if (!m_widget) {
        return;
    }

    QSettings settings;
    settings.setValue(tmc::TmcConstants::SETTINGS_TMC_SPIN_EXE_KEY, m_widget->getSpinExecutable());
    settings.setValue(tmc::TmcConstants::SETTINGS_TMC_SPIN_DEFAULT_OUTPUT_DIRECTORY, m_widget->getDefaultOutputDirectory());
    // automatically make destination directory
    QDir defaultOutputDirectory(m_widget->getDefaultOutputDirectory());
    defaultOutputDirectory.mkpath(QStringLiteral("."));
}

void TmcOptions::finish()
{
    m_widget.clear();
}
}
