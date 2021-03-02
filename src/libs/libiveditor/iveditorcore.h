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

#include "aadliface.h"
#include "editorcore.h"
#include "ui/graphicsviewbase.h"

#include <QVector>

namespace ivm {
class AADLConnection;
class AADLFunction;
}

namespace ive {
namespace cmd {
class CommandsStack;
}

class InterfaceDocument;

class IVEditorCore : public shared::EditorCore
{
    Q_OBJECT
public:
    explicit IVEditorCore(QObject *parent = nullptr);
    ~IVEditorCore() override;

    ive::InterfaceDocument *document() const;

    shared::ui::GraphicsViewBase *chartView() override;
    void addToolBars(QMainWindow *window) override;

    void registerBasicActions();

    QToolBar *docToolBar() { return m_docToolBar; }

    void populateCommandLineArguments(shared::CommandLineParser *parser) const override;

    QAction *actionExportFunctions();
    QAction *actionExportType();
    QAction *actionToggleE2EView();

    ivm::AADLFunction *addFunction(const QString &name, ivm::AADLFunction *parent = nullptr);
    bool addConnection(QString name, const QString &fromInstanceName, const QString &toInstanceName);
    ivm::AADLIface *addInterface(QString name, const QString &functionName);

    bool renameAadlFunction(const QString &oldName, const QString &newName);
    bool renameAadlConnection(const QString &oldName, const QString &newName, const QString &fromInstanceName,
            const QString &toInstanceName);
    bool renameCyclicInterface(const QString &oldName, const QString &newName, const QString &functionName);

    QUndoStack *undoStack() const override;
    cmd::CommandsStack *commandsStack() const;

    bool renameAsnFile(const QString &oldName, const QString &newName) override;

    QString filePath() const override;
    bool save() override;

    QVector<ivm::AADLFunction *> allAadlFunctions() const;
    QVector<ivm::AADLConnection *> allAadlConnections() const;

    QStringList aadlFunctionsNames() const;
    QStringList aadlConnectionNames() const;

public Q_SLOTS:
    void onSaveRenderRequested();

private:
    void saveSceneRender(const QString &filePath) const;
    ivm::AADLIface *getInterface(
            const QString &ifName, ivm::AADLIface::IfaceType ifType, ivm::AADLFunction *parentFunction);
    Q_SLOT void updateAadlItems();
    QUrl helpPage() const override;

    ive::InterfaceDocument *m_document { nullptr };

    QToolBar *m_docToolBar { nullptr };

    QAction *m_actionExportFunctions { nullptr };
    QAction *m_actionExportType { nullptr };
    QAction *m_actionToggleE2EView { nullptr };

    QVector<ivm::AADLFunction *> m_aadlFunctions;
    QVector<ivm::AADLConnection *> m_aadlConnections;

    Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
};

}
