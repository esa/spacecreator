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

#include <QPointer>
#include <QWidget>

class QComboBox;

namespace shared {
namespace ui {
class GraphicsViewBase;
} // namespace ui
} // namespace shared

namespace ive {

class ZoomValidator;

class ZoomController : public QWidget
{
    Q_OBJECT

public:
    explicit ZoomController(QWidget *parent = nullptr);

    void setView(shared::ui::GraphicsViewBase *view);

protected Q_SLOTS:
    void onCurrentIndexChanged(int idx);
    void setZoomLevel(qreal percent);
    void displayZoomLevel(qreal percent);

private:
    QComboBox *m_combo { nullptr };
    ZoomValidator *m_validator { nullptr };
    QPointer<shared::ui::GraphicsViewBase> m_view;

    void refill();
};

}
