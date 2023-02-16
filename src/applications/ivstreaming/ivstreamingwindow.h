/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QMainWindow>
#include <memory>

namespace ive {
class InterfaceDocument;
}

namespace Ui {
class IVStreamingWindow;
}

namespace iv {

/**
 * This is the main window class for the IV stream remote control.
 * It inclues the model as well.
 */
class IVStreamingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IVStreamingWindow(QWidget *parent = nullptr);
    ~IVStreamingWindow();

    bool startRemoteControl(quint16 port);

private:
    void closeEvent(QCloseEvent *e) override;

    Ui::IVStreamingWindow *ui = nullptr;
    ive::InterfaceDocument *m_document = nullptr;
};

}
