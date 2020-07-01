/*
Copyright (C) 2020 by Viking Software www.vikingsoftware.com

Permission to use, copy, modify, and/or distribute this software for any purpose
with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.
*/

#pragma once

#include <QtCore/qnamespace.h>

class QWidget;
class QPoint;

namespace vstest {

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

}
