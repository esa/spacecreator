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

#include "tst_common.h"

#include <QApplication>
#include <QMouseEvent>
#include <QTest>

namespace msc {
namespace test {
namespace ui {

void sendMousePress(QWidget *widget, const QPoint &point, Qt::MouseButton button, int delay)
{
    QMouseEvent event(QEvent::MouseButtonPress, point, widget->mapToGlobal(point), button, button, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
    if (delay > 0)
        QTest::qWait(delay);
}

void sendMouseMove(QWidget *widget, const QPoint &point, Qt::MouseButton button, Qt::MouseButtons buttons, int delay)
{
    QTest::mouseMove(widget, point);
    QMouseEvent event(QEvent::MouseMove, point, button, buttons, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
    QApplication::processEvents();
    if (delay > 0)
        QTest::qWait(delay);
}

void sendMouseRelease(QWidget *widget, const QPoint &point, Qt::MouseButton button, int delay)
{
    QMouseEvent event(QEvent::MouseButtonRelease, point, widget->mapToGlobal(point), button, button, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
    if (delay > 0)
        QTest::qWait(delay);
}

} // ns ui
} // ns test
} // ns msc
