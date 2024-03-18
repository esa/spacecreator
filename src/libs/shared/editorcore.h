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

#pragma once

#include "minimap.h"

#include <QObject>
#include <QPointer>
#include <QToolBar>

class QAction;
class QMainWindow;
class QMenu;
class QUndoStack;
class QUrl;

namespace shared {
namespace cmd {
class CommandsStackBase;
}

class DataModel;

class CommandLineParser;

namespace ui {
class GraphicsViewBase;
}

class EditorCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl requirementsURL READ requirementsURL WRITE setRequirementsURL NOTIFY requirementsURLChanged FINAL)

public:
    explicit EditorCore(QObject *parent = nullptr);
    virtual ~EditorCore();

    virtual DataModel *dataModel() const = 0;

    QWidget *minimapView() const;
    virtual ui::GraphicsViewBase *chartView() = 0;
    virtual QToolBar *mainToolBar();
    virtual void addToolBars(QMainWindow *window) = 0;

    QAction *actionNewFile();
    QAction *actionOpenFile();
    QAction *actionSaveFile();
    QAction *actionSaveFileAs();
    QAction *actionQuit();
    QAction *actionUndo();
    QAction *actionRedo();
    QAction *actionToggleMinimap();
    virtual QAction *actionDelete() const = 0;

    // Populate a CommandLineParser with the arguments this application can handle
    virtual void populateCommandLineArguments(CommandLineParser *parser) const = 0;

    void setupMiniMap();

    virtual QUndoStack *undoStack() const = 0;

    virtual bool renameAsnFile(const QString &oldName, const QString &newName) = 0;

    //! Returns the full file path of the file in this core
    virtual QString filePath() const = 0;
    //! Saves the file
    virtual bool save() = 0;

    static QAction *createHelpLinesAction(QObject *parent = nullptr);
    static QAction *createSnapToGridAction(QObject *parent = nullptr);

    //! Sets the URL where requirements are stored
    virtual void setRequirementsURL(const QUrl &url);
    //! Returns the URL where requirements are stored
    virtual const QUrl &requirementsURL() const;

    //! Sets the URL where reviews are stored
    virtual void setReviewsURL(const QUrl &url);
    //! Returns the URL where reviews are stored
    virtual const QUrl &reviewsURL() const;

public Q_SLOTS:
    void showHelp();
    void showAboutDialog();

Q_SIGNALS:
    void editedExternally(shared::EditorCore *);
    void requirementsURLChanged(const QUrl &url); //! Emitted when the user edited the URL of the requirements
    void reviewsURLChanged(const QUrl &url); //! Emitted when the user edited the URL of the reviews

private:
    virtual QUrl helpPage() const;

    QPointer<QToolBar> m_mainToolBar { nullptr };

    QPointer<ui::MiniMap> m_miniMap { nullptr };

    QAction *m_actionNewFile { nullptr };
    QAction *m_actionOpenFile { nullptr };
    QAction *m_actionSaveFile { nullptr };
    QAction *m_actionSaveFileAs { nullptr };
    QAction *m_actionCloseFile { nullptr };
    QAction *m_actionQuit { nullptr };
    QAction *m_actionUndo { nullptr };
    QAction *m_actionRedo { nullptr };
    QAction *m_actionToggleMinimap { nullptr };
};

}

typedef QSharedPointer<shared::EditorCore> EditorCorePtr;
Q_DECLARE_METATYPE(EditorCorePtr)
