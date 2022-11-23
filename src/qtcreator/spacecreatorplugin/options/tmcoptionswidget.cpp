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
}
