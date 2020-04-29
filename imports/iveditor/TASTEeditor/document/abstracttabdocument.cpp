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

namespace document {

/*!
\class taste3::document::AbstractTabDocument
\brief taste3::document::AbstractTabDocument is the base interface to handle particular data format
editing and displaying (AADL-graphics, AADL-text, same for the MSC, etc).
It designed to be used in the multi-document editor, providing methods to handle
data loading, saving and closing, document's dirtyness and operations stack management.
It's the place to create and store related instances of QGraphicsScene and QGraphicasView.
An descendant instances should be created via TabDocumentFactory.
Currently it's used only to work with AADL (graphical) data, the other formats integration
may need some API changes/polishing.

\sa taste3::document::TabDocumentFactory, taste3::document::DocumentsManager, taste3::document::InterfaceTabDocument
*/

struct AbstractTabDocumentPrivate {
    explicit AbstractTabDocumentPrivate(QObject *parent)
        : m_commandsStack(new QUndoStack(parent))
    {
    }

    QPointer<QGraphicsScene> m_scene { nullptr };
    QPointer<QWidget> m_view { nullptr };
    QPointer<QUndoStack> const m_commandsStack;
    QString m_filePath;
    int m_lastSavedIndex { 0 };
    bool m_dirty { false };
};

AbstractTabDocument::AbstractTabDocument(QObject *parent)
    : QObject(parent)
    , d(new AbstractTabDocumentPrivate(this))
{
    connect(commandsStack(), &QUndoStack::indexChanged, this, &AbstractTabDocument::updateDirtyness);
}

AbstractTabDocument::~AbstractTabDocument() {}

void AbstractTabDocument::init()
{
    if (QGraphicsScene *scene = createScene())
        setupScene(scene);
    if (QWidget *view = createView())
        setupView(view);
}

void AbstractTabDocument::setupScene(QGraphicsScene *scene)
{
    d->m_scene = scene;
}

QGraphicsScene *AbstractTabDocument::scene() const
{
    return d->m_scene;
}

void AbstractTabDocument::setupView(QWidget *view)
{
    d->m_view = view;
}

QWidget *AbstractTabDocument::view() const
{
    return d->m_view;
}

QUndoStack *AbstractTabDocument::commandsStack() const
{
    return d->m_commandsStack.data();
}

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

bool AbstractTabDocument::create(const QString &path)
{
    const bool created = createImpl(path);
    if (created)
        resetDirtyness();
    return created;
}

bool AbstractTabDocument::createImpl(const QString &path)
{
    Q_UNUSED(path)
    return false;
}

bool AbstractTabDocument::load(const QString &path)
{
    const bool loaded = loadImpl(path);

    if (loaded) {
        d->m_filePath = path;
        d->m_commandsStack->clear();
        resetDirtyness();
        Q_EMIT dirtyChanged(false);
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

void AbstractTabDocument::close()
{
    closeImpl();
    d->m_filePath.clear();
    resetDirtyness();
    Q_EMIT dirtyChanged(false);
}

QString AbstractTabDocument::path() const
{
    return d->m_filePath;
}

QString AbstractTabDocument::supportedFileExtensions() const
{
    return { "*.*" };
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
        Q_EMIT dirtyChanged(isDirty());
    }
}

QString AbstractTabDocument::title() const
{
    return tr("Untitled");
}

QMenu *AbstractTabDocument::customMenu() const
{
    return nullptr;
}

void AbstractTabDocument::onSavedExternally(const QString &filePath, bool saved)
{
    if (saved) {
        const bool forceTitleUpdate = d->m_filePath != filePath && !isDirty();

        d->m_filePath = filePath;

        if (forceTitleUpdate)
            Q_EMIT dirtyChanged(false);
        else
            resetDirtyness();
    }
}

}
