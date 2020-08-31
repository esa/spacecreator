/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commandlineparser.h"

#include <QMainWindow>
#include <memory>

class QGraphicsView;
class QScreen;

namespace Ui {
class StreamingWindow;
}

namespace msc {
class MSCEditorCore;

class StreamingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StreamingWindow(msc::MSCEditorCore *plugin, QWidget *parent = nullptr);
    ~StreamingWindow();

    bool startRemoteControl(quint16 port);

private Q_SLOTS:
    void adaptWindowSizeToChart(const QRectF &rect);

private:
    QScreen *screen() const;
    void closeEvent(QCloseEvent *e) override;

    struct StreamingWindowPrivate;
    std::unique_ptr<StreamingWindowPrivate> const d;
};

}
