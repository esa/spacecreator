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

namespace test {
namespace ui {

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
}
