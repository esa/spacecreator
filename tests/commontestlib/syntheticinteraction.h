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

#include <QtCore/qnamespace.h>

class QWidget;
class QPoint;

namespace test {
namespace ui {

// EventsDelayMs - In my local environment 20ms is enough, but I'm not sure
// about CI, so here is some extra time span.
// The difference in total test duration (20ms per event vs 100ms) is about 3s.
static int EventsDelayMs = 50;

// QGraphicsScene does not accept QTest's mouse press event,
// so we have to synthesize them:
void sendMousePress(
        QWidget *widget, const QPoint &point, Qt::MouseButton button = Qt::LeftButton, int delay = EventsDelayMs);

void sendMouseMove(QWidget *widget, const QPoint &point, Qt::MouseButton button = Qt::NoButton,
        Qt::MouseButtons buttons = Qt::NoButton, int delay = EventsDelayMs);

void sendMouseRelease(
        QWidget *widget, const QPoint &point, Qt::MouseButton button = Qt::LeftButton, int delay = EventsDelayMs);

void sendMouseDrag(QWidget *widget, const QPoint &from, const QPoint &to, int delay = EventsDelayMs);

void saveMousePosition();
void restoreMousePosition();

} // ns ui
} // ns test
