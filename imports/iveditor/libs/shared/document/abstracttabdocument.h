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

namespace document {

class AbstractTabDocument : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTabDocument(QObject *parent = nullptr);
    virtual ~AbstractTabDocument();

    virtual void init();

    void fillToolBar(QToolBar *toolBar);

    QGraphicsScene *scene() const;
    QWidget *view() const;
    QUndoStack *commandsStack() const;

    bool create(const QString &path = QString());
    bool load(const QString &path);
    bool save(const QString &path);
    void close();

    QString path() const;
    virtual QString supportedFileExtensions() const;

    bool isDirty() const;

    virtual QString title() const;

    // A menu entry specific for current doc.
    // Return nullptr if the doc does not need any menu
    virtual QMenu *customMenu() const;

Q_SIGNALS:
    void dirtyChanged(bool dirty) const;

public Q_SLOTS:
    void onSavedExternally(const QString &filePath, bool saved);

protected Q_SLOTS:
    void updateDirtyness();

protected:
    void resetDirtyness();
    void setupScene(QGraphicsScene *scene);
    void setupView(QWidget *view);

    virtual bool createImpl(const QString &path = QString());
    virtual bool loadImpl(const QString &path) = 0;
    virtual bool saveImpl(const QString &path) = 0;
    virtual void closeImpl() = 0;
    virtual QVector<QAction *> initActions() = 0;

    virtual QWidget *createView() = 0;
    virtual QGraphicsScene *createScene() = 0;

private:
    struct AbstractTabDocumentPrivate;
    std::unique_ptr<AbstractTabDocumentPrivate> const d;
};

}
