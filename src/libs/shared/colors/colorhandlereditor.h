/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "colormanager.h"

#include <QWidget>

namespace Ui {
class ColorHandlerEditor;
}

namespace shared {

class ColorHandlerEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ColorHandlerEditor(QWidget *parent = nullptr);
    ~ColorHandlerEditor();

    void setColorHandler(ColorHandler *h);
    ColorHandler *colorHandler() const;

Q_SIGNALS:
    void colorHandlerValueChanged();

private Q_SLOTS:
    void on_sbWidth_valueChanged(qreal v);
    void on_btnColorStroke_colorChanged(const QColor &c);
    void on_cbFillType_currentIndexChanged(int id);
    void on_btnColor_colorChanged(const QColor &c);
    void on_btnColorStop_colorChanged(const QColor &c);

private:
    Ui::ColorHandlerEditor *ui;
    ColorHandler *m_colorHandler { nullptr };
};

}
