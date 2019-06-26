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

#pragma once

#include <QAction>
#include <QObject>
#include <QVector>
#include <memory>

class QGraphicsScene;
class QToolBar;
class QUndoStack;

namespace taste3 {
namespace document {

struct AbstractTabDocumentPrivate;

class AbstractTabDocument : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTabDocument(QObject *parent = nullptr);
    virtual ~AbstractTabDocument();

    void init();

    void fillToolBar(QToolBar *toolBar);

    QGraphicsScene *scene() const;
    QWidget *view() const;
    QUndoStack *commandsStack() const;

    bool load(const QString &path);
    bool save(const QString &path);

    QString path() const;

    bool isDirty() const;

    virtual QString title() const;

signals:
    void dirtyChanged(bool dirty) const;

protected slots:
    void updateDirtyness();

protected:
    virtual bool loadImpl(const QString &path) = 0;
    virtual bool saveImpl(const QString &path) = 0;
    virtual QVector<QAction *> initActions() = 0;

    void setupScene(QGraphicsScene *scene);
    void setupView(QWidget *view);

    virtual QWidget *createView() = 0;
    virtual QGraphicsScene *createScene() = 0;

private:
    std::unique_ptr<AbstractTabDocumentPrivate> const d;
    void resetDirtyness();
};

} // ns document
} // ns taste3
