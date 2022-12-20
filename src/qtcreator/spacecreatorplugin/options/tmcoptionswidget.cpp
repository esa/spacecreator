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

#include "tmcoptionswidget.h"

#include "ui_tmcoptionswidget.h"

#include <QFileDialog>

namespace spctr {
TmcOptionsWidget::TmcOptionsWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::TmcOptionsWidget())
{
    m_ui->setupUi(this);
}

TmcOptionsWidget::~TmcOptionsWidget()
{
    delete m_ui;
}

void TmcOptionsWidget::setSpinExecutable(const QString &spinExecutable)
{
    m_ui->spinExecutableLineEdit->setText(spinExecutable);
}

QString TmcOptionsWidget::getSpinExecutable()
{
    return m_ui->spinExecutableLineEdit->text();
}

void TmcOptionsWidget::setDefaultOutputDirectory(const QString &defaultOutputDirectory)
{
    m_ui->outputDirectoryLineEdit->setText(defaultOutputDirectory);
}

QString TmcOptionsWidget::getDefaultOutputDirectory()
{
    return m_ui->outputDirectoryLineEdit->text();
}

void TmcOptionsWidget::setSdl2PromelaTimeout(int timeout)
{
    m_ui->sdl2promelaTimeoutLineEdit->setValue(timeout);
}

int TmcOptionsWidget::getSdl2PromelaTimeout()
{
    return m_ui->sdl2promelaTimeoutLineEdit->value();
}

void TmcOptionsWidget::setCCompilerTimeout(int timeout)
{
    m_ui->ccompilerTimeoutLineEdit->setValue(timeout);
}

int TmcOptionsWidget::getCCompilerTimeout()
{
    return m_ui->ccompilerTimeoutLineEdit->value();
}

void TmcOptionsWidget::setExternalCommandTimeout(int timeout)
{
    m_ui->externalCommandTimeoutLineEdit->setValue(timeout);
}

int TmcOptionsWidget::getExternalCommandTimeout()
{
    return m_ui->externalCommandTimeoutLineEdit->value();
}

void TmcOptionsWidget::setCompilerFlags(const QString &compilerFlags)
{
    m_ui->compilerFlagsLineEdit->setText(compilerFlags);
}

QString TmcOptionsWidget::getCompilerFlags()
{
    return m_ui->compilerFlagsLineEdit->text();
}
}
