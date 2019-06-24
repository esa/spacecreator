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

#include "abstracttabdocument.h"

#include <QGraphicsScene>
#include <QPointer>
#include <QToolBar>
#include <QUndoStack>

namespace taste3 {
namespace document {

struct AbstractTabDocumentPrivate {
    explicit AbstractTabDocumentPrivate()
        : m_commandsStack(new QUndoStack)
    {
    }

    QPointer<QGraphicsScene> m_scene { nullptr };
    std::unique_ptr<QUndoStack> const m_commandsStack;
    QString m_filePath;
    int m_lastSavedIndex { 0 };
    bool m_dirty { false };
};

AbstractTabDocument::AbstractTabDocument(QObject *parent)
    : QObject(parent)
    , d(new AbstractTabDocumentPrivate())
{
    connect(commandsStack(), &QUndoStack::indexChanged, this, &AbstractTabDocument::updateDirtyness);
}

void AbstractTabDocument::setDocScene(QGraphicsScene *scene)
{
    d->m_scene = scene;
}

AbstractTabDocument::~AbstractTabDocument() {}

void AbstractTabDocument::fillToolBar(QToolBar *toolBar)
{
    if (!toolBar)
        return;

    toolBar->setUpdatesEnabled(false);
    toolBar->clear();

    const QVector<QAction *> &docActions = initActions();
    for (QAction *action : docActions)
        toolBar->addAction(action);

    toolBar->setUpdatesEnabled(true);
}

QGraphicsScene *AbstractTabDocument::scene() const
{
    return d->m_scene;
}

QUndoStack *AbstractTabDocument::commandsStack() const
{
    return d->m_commandsStack.get();
}

bool AbstractTabDocument::load(const QString &path)
{
    const bool loaded = loadImpl(path);

    if (loaded) {
        d->m_filePath = path;
        d->m_commandsStack->clear();
        resetDirtyness();
    }

    return loaded;
}

bool AbstractTabDocument::save(const QString &path)
{
    const bool saved = saveImpl(path);

    if (saved) {
        d->m_filePath = path;
        resetDirtyness();
    }

    return saved;
}

QString AbstractTabDocument::path() const
{
    return d->m_filePath;
}

void AbstractTabDocument::resetDirtyness()
{
    d->m_lastSavedIndex = d->m_commandsStack->index();
    updateDirtyness();
}

bool AbstractTabDocument::isDirty() const
{
    return d->m_dirty;
}

void AbstractTabDocument::updateDirtyness()
{
    const bool wasDirty = isDirty();
    d->m_dirty = d->m_lastSavedIndex != d->m_commandsStack->index();

    if (wasDirty != isDirty()) {
        emit dirtyChanged(isDirty());
    }
}

QString AbstractTabDocument::title() const
{
    return tr("Untitled");
}

} // ns document
} // ns taste3
