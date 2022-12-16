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

#pragma once

#include <QWidget>

namespace spctr {
namespace Ui {
class TmcOptionsWidget;
}
class TmcOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    TmcOptionsWidget(QWidget *parent = nullptr);
    ~TmcOptionsWidget();

    void setSpinExecutable(const QString &spinExecutable);
    QString getSpinExecutable();

    void setDefaultOutputDirectory(const QString &defaultOutputDirectory);
    QString getDefaultOutputDirectory();

    void setSdl2PromelaTimeout(int timeout);
    int getSdl2PromelaTimeout();

    void setCCompilerTimeout(int timeout);
    int getCCompilerTimeout();

    void setExternalCommandTimeout(int timeout);
    int getExternalCommandTimeout();

    void setCompilerFlags(const QString &compilerFlags);
    QString getCompilerFlags();

private:
    Ui::TmcOptionsWidget *m_ui;
};

}
