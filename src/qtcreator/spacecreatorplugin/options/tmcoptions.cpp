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
        if (!defaultOutputDirectory.isValid()) {
            defaultOutputDirectory = QString("tmc-output");
        }
        m_widget->setDefaultOutputDirectory(defaultOutputDirectory.toString());

        QVariant sdl2PromelaTimeout = settings.value(tmc::TmcConstants::SETTINGS_TMC_SDL2PROMELA_TIMEOUT);
        if (!sdl2PromelaTimeout.isValid()) {
            sdl2PromelaTimeout = 60;
        }
        m_widget->setSdl2PromelaTimeout(sdl2PromelaTimeout.toInt());

        QVariant ccompilerTimeout = settings.value(tmc::TmcConstants::SETTINGS_TMC_CCOMPILER_TIMEOUT);
        if (!ccompilerTimeout.isValid()) {
            ccompilerTimeout = 60;
        }
        m_widget->setCCompilerTimeout(ccompilerTimeout.toInt());

        QVariant externalCommandTimeout = settings.value(tmc::TmcConstants::SETTINGS_TMC_EXTERNAL_COMMAND_TIMEOUT);
        if (!externalCommandTimeout.isValid()) {
            externalCommandTimeout = 60;
        }
        m_widget->setExternalCommandTimeout(externalCommandTimeout.toInt());

        QVariant compilerFlags = settings.value(tmc::TmcConstants::SETTINGS_TMC_COMPILER_FLAGS);
        if (!compilerFlags.isValid()) {
            compilerFlags = QString("-O2");
        }
        m_widget->setCompilerFlags(compilerFlags.toString());
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
    settings.setValue(
            tmc::TmcConstants::SETTINGS_TMC_SPIN_DEFAULT_OUTPUT_DIRECTORY, m_widget->getDefaultOutputDirectory());
    settings.setValue(tmc::TmcConstants::SETTINGS_TMC_SDL2PROMELA_TIMEOUT, m_widget->getSdl2PromelaTimeout());
    settings.setValue(tmc::TmcConstants::SETTINGS_TMC_CCOMPILER_TIMEOUT, m_widget->getCCompilerTimeout());
    settings.setValue(tmc::TmcConstants::SETTINGS_TMC_EXTERNAL_COMMAND_TIMEOUT, m_widget->getExternalCommandTimeout());
    settings.setValue(tmc::TmcConstants::SETTINGS_TMC_COMPILER_FLAGS, m_widget->getCompilerFlags());
}

void TmcOptions::finish()
{
    m_widget.clear();
}
}
