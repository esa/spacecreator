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

#pragma once

#include <QPointer>
#include <coreplugin/dialogs/ioptionspage.h>

namespace aadlinterface {
class ColorSettingsWidget;
}

namespace spctr {

class IVColorOption : public Core::IOptionsPage
{
    Q_OBJECT

public:
    IVColorOption(QObject *parent = nullptr);

    QWidget *widget() override;
    void apply() override;
    void finish() override;

public Q_SLOTS:
    void restoreDefaults();

private:
    QPointer<QWidget> m_widget;
    QPointer<aadlinterface::ColorSettingsWidget> m_colorWidget;
    QString m_originalFile;
    bool m_reset = true;
};

}
