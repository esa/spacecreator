/*
   Copyright (C) 2020 Denis Gofman - <sendevent@gmail.com>

   This application is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   This application is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
*/

#include "minimap.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointer>

namespace shared {
namespace ui {

struct MiniMapPrivate {
    bool isReady() const { return m_view && m_scene; }

    QPointer<QGraphicsView> m_view { nullptr };
    QPointer<QGraphicsScene> m_scene { nullptr };
};

MiniMap::MiniMap(QWidget *parent)
    : QDockWidget(parent)
    , d(new MiniMapPrivate)
{
    qDebug() << Q_FUNC_INFO << parent;
}

MiniMap::~MiniMap() {}

void MiniMap::setupSourceView(QGraphicsView *view)
{
    if (d->m_view) {
        // disconnect
    }

    d->m_view = view;

    if (d->m_view) {
        // connect
    }
}

void MiniMap::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    Q_EMIT visibilityChanged(isVisible());
}

void MiniMap::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);
    Q_EMIT visibilityChanged(isVisible());
}

} // namespace ui
} // namespace shared
