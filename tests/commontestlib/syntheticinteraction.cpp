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

#include "syntheticinteraction.h"

#include <QApplication>
#include <QCursor>
#include <QMouseEvent>
#include <QPoint>
#include <QTest>
#include <QVector>

namespace QTest {
extern Q_TESTLIB_EXPORT int lastMouseTimestamp;
}

namespace vstest {

static QVector<QPoint> savedMousePositions = {};

void sendMousePress(QWidget *widget, const QPoint &point, Qt::MouseButton button, int delay)
{
    QMouseEvent event(QEvent::MouseButtonPress, point, widget->mapToGlobal(point), button, button, Qt::NoModifier);
    event.setTimestamp(++(QTest::lastMouseTimestamp));
    QSpontaneKeyEvent::setSpontaneous(&event);
    qApp->notify(widget, &event);

    if (delay > 0)
        QTest::qWait(delay);
}

void sendMouseMove(QWidget *widget, const QPoint &point, Qt::MouseButton button, Qt::MouseButtons buttons, int delay)
{
    QCursor::setPos(widget->mapToGlobal(point));
    QMouseEvent event(QEvent::MouseMove, point, button, buttons, Qt::NoModifier);
    event.setTimestamp(++(QTest::lastMouseTimestamp));
    QSpontaneKeyEvent::setSpontaneous(&event);
    qApp->notify(widget, &event);

    if (delay > 0) {
        QTest::qWait(delay);
    }
}

void sendMouseRelease(QWidget *widget, const QPoint &point, Qt::MouseButton button, int delay)
{
    QMouseEvent event(QEvent::MouseButtonRelease, point, widget->mapToGlobal(point), button, button, Qt::NoModifier);
    event.setTimestamp(++(QTest::lastMouseTimestamp));
    QTest::lastMouseTimestamp += QTest::mouseDoubleClickInterval; // avoid double clicks being generated
    QSpontaneKeyEvent::setSpontaneous(&event);
    qApp->notify(widget, &event);

    if (delay > 0)
        QTest::qWait(delay);
}

/*!
    Convenient function to call all mouse actions for a mouse drag action
    Avoids any Qt event handling (QApplication::processEvents()) between the press and the release.
 */
void sendMouseDrag(QWidget *widget, const QPoint &from, const QPoint &to, int delay)
{
    sendMouseMove(widget, from, Qt::NoButton, Qt::NoButton, delay);
    sendMousePress(widget, from, Qt::LeftButton, 0);
    sendMouseMove(widget, to, Qt::LeftButton, Qt::LeftButton, 0);
    sendMouseRelease(widget, to, Qt::LeftButton, delay);
}

void saveMousePosition()
{
    savedMousePositions.append(QCursor::pos());
}

void restoreMousePosition()
{
    if (savedMousePositions.isEmpty()) {
        return;
    }

    const QPoint &pos = savedMousePositions.takeLast();
    QCursor::setPos(pos);
}

}
