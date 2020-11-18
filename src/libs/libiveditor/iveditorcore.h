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

#include "aadlobjectiface.h"
#include "editorcore.h"
#include "ui/graphicsviewbase.h"

#include <QVector>

namespace aadl {
class AADLObjectConnection;
}

namespace aadlinterface {
namespace cmd {
class CommandsStack;
}

class InterfaceDocument;

class IVEditorCore : public shared::EditorCore
{
    Q_OBJECT
public:
    explicit IVEditorCore(QObject *parent = 0);
    ~IVEditorCore();

    aadlinterface::InterfaceDocument *document() const;

    void setPluginActive(bool active) override;

    shared::ui::GraphicsViewBase *chartView() override;
    void addToolBars(QMainWindow *window) override;
    void addMenuFileActions(QMenu *menu, QMainWindow *window) override;
    void addMenuEditActions(QMenu *menu, QMainWindow *window) override;
    void addMenuViewActions(QMenu *menu, QMainWindow *window) override;
    void addMenuHelpActions(QMenu *menu, QMainWindow *window) override;

    void registerBasicActions();

    QToolBar *docToolBar() { return m_docToolBar; }

    void populateCommandLineArguments(shared::CommandLineParser *parser) const override;

    QAction *actionSaveSceneRender() { return m_actionSaveSceneRender; }
    QAction *actionShowAsnDialog() { return m_actionShowAsnDialog; }

    bool addFunction(const QString &name);
    bool addConnection(QString name, const QString &fromInstanceName, const QString &toInstanceName);

    bool renameAadlFunction(const QString &oldName, const QString &newName);
    bool renameAadlConnection(const QString &oldName, const QString &newName, const QString &fromInstanceName,
            const QString &toInstanceName);

    QUndoStack *undoStack() const override;
    cmd::CommandsStack *commandsStack() const;

    bool renameAsnFile(const QString &oldName, const QString &newName) override;

    QString filePath() const override;
    bool save() override;

    QVector<aadl::AADLObjectFunction *> allAadlFunctions() const;
    QVector<aadl::AADLObjectConnection *> allAadlConnections() const;

    QStringList aadlFunctionsNames() const;
    QStringList aadlConnectionNames() const;

public Q_SLOTS:
    void onSaveRenderRequested();

private:
    void saveSceneRender(const QString &filePath) const;
    aadl::AADLObjectIface *getInterface(
            const QString &ifName, aadl::AADLObjectIface::IfaceType ifType, aadl::AADLObjectFunction *parentFunction);
    Q_SLOT void updateAadlItems();

    aadlinterface::InterfaceDocument *m_document { nullptr };

    QToolBar *m_docToolBar;

    QAction *m_actionSaveSceneRender { nullptr };
    QAction *m_actionShowAsnDialog { nullptr };

    QVector<aadl::AADLObjectFunction *> m_aadlFunctions;
    QVector<aadl::AADLObjectConnection *> m_aadlConnections;

    Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
};

}
