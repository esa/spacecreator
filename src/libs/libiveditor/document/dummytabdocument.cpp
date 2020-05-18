/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dummytabdocument.h"

#include <QAction>
#include <QDebug>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace document {

DummyTabDocument::DummyTabDocument(QObject *parent)
    : AbstractTabDocument(parent)
{
}

DummyTabDocument::~DummyTabDocument()
{
    if (m_view && !m_view->parent())
        delete m_view;
}

QWidget *DummyTabDocument::createView()
{
    if (!m_view)
        m_view = new QGraphicsView;
    return m_view;
}

QGraphicsScene *DummyTabDocument::createScene()
{
    if (!m_customScene)
        m_customScene = new QGraphicsScene(this);
    return m_customScene;
}

QString DummyTabDocument::title() const
{
    return tr("Dummy Tab");
}

bool DummyTabDocument::loadImpl(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        qWarning() << Q_FUNC_INFO << "Invalid path";
        return false;
    }

    return false;
}

bool DummyTabDocument::saveImpl(const QString & /*path*/)
{
    return false;
}

void DummyTabDocument::closeImpl() { }

QVector<QAction *> DummyTabDocument::initActions()
{
    if (!m_actDummy) {
        m_actDummy = new QAction(tr("%1 Stub").arg(title()));
        connect(m_actDummy, &QAction::triggered, this, &DummyTabDocument::onActionDummy);
    }

    return { m_actDummy };
}

void DummyTabDocument::onActionDummy() { }

}
