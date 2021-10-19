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

#include "actionsbar.h"

#include <QAction>
#include <QActionEvent>
#include <QFrame>
#include <QToolButton>
#include <QVBoxLayout>

namespace shared {

ActionsBar::ActionsBar(QWidget *parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
{
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addStretch(1);
    setLayout(m_layout);
}

/*!
   Adds a new QToolButton when a new action was added using the function addAction(QAction*)
 */
void ActionsBar::actionEvent(QActionEvent *event)
{
    auto action = static_cast<QAction *>(event->action());

    switch (event->type()) {
    case QEvent::ActionAdded: {
        if (action->isSeparator()) {
            QFrame *line = new QFrame(this);
            line->setFrameShape(QFrame::HLine);
            m_layout->insertWidget(m_layout->count() - 1, line);
        } else {
            auto button = new QToolButton(this);
            button->setAutoRaise(true);
            button->setFocusPolicy(Qt::NoFocus);
            button->setIconSize(m_iconSize);
            button->setDefaultAction(action);
            m_layout->insertWidget(m_layout->count() - 1, button);
        }
        break;
    }

    case QEvent::ActionChanged:
        break;

    case QEvent::ActionRemoved: {
        break;
    }

    default:
        Q_ASSERT_X(false, "ActionsBar::actionEvent", "internal error");
    }
}

void ActionsBar::addSeparator()
{
    QAction *action = new QAction(this);
    action->setSeparator(true);
    addAction(action);
}

}
