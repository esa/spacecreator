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

#include "editorcore.h"
#include "graphicsview.h"
#include "mscdocument.h"

#include <QList>
#include <QSharedPointer>
#include <QVector>
#include <memory>

namespace ivm {
class IVConnection;
class IVFunction;
}

namespace shared {
class UndoCommand;
}

namespace msc {
class MainModel;
class MscChart;
class MscCommandsStack;
class MscInstance;
class MscMessage;
class PointerTool;
class SystemChecks;
class MscAppWidget;

class MainWindow;

class MSCEditorCore : public shared::EditorCore
{
    Q_OBJECT

public:
    explicit MSCEditorCore(QObject *parent = nullptr);
    ~MSCEditorCore();

    msc::MainModel *mainModel() const;

    shared::ui::GraphicsViewBase *chartView() override;

    void initConnections();

    void addToolBars(QMainWindow *window) override;

    MscAppWidget *mainwidget();

    void populateCommandLineArguments(shared::CommandLineParser *parser) const override;

    QAction *actionCopy() { return m_actionCopy; }
    QAction *actionPaste() { return m_actionPaste; }

    QAction *actionMessageDeclarations();

    QAction *createActionCopy(QMainWindow *window);
    QAction *createActionPaste(QMainWindow *window);

    QAction *actionCheckInstances();
    QAction *actionCheckMessages();

    void setSystemChecker(msc::SystemChecks *checker);
    msc::SystemChecks *systemChecker() const;

    QUndoStack *undoStack() const override;
    msc::MscCommandsStack *commandsStack() const;

    bool renameAsnFile(const QString &oldName, const QString &newName) override;
    void changeMscInstanceName(const QString &oldName, const QString &name);
    void changeMscMessageName(
            const QString &oldName, const QString &newName, const QString &sourceName, const QString &targetName);
    void removeMscInstances(ivm::IVFunction *ivFunction);
    void removeMscMessages(ivm::IVConnection *ivConnection);
    QList<msc::MscInstance *> correspondingInstances(ivm::IVFunction *ivFunction) const;
    QList<msc::MscMessage *> correspondingMessages(ivm::IVConnection *ivConnection) const;

    QString filePath() const override;
    bool save() override;

public Q_SLOTS:
    void selectCurrentChart();
    void openMessageDeclarationEditor(QWidget *parentwidget);
    void addDocument(msc::MscDocument::HierarchyType type);

Q_SIGNALS:
    void nameChanged(QObject *entity, const QString &oldName, shared::UndoCommand *command);

private:
    QUrl helpPage() const override;

    std::unique_ptr<msc::MainModel> m_model;
    msc::SystemChecks *m_systemChecks = nullptr;

    QAction *m_actionCopy = nullptr;
    QAction *m_actionPaste = nullptr;
    QAction *m_actionMessageDeclarations = nullptr;
    QAction *m_actionCheckInstances = nullptr;
    QAction *m_actionCheckMessages = nullptr;

    bool m_connectionsDone = false;

    MscAppWidget *m_mainWidget = nullptr;
};

}

typedef QSharedPointer<msc::MSCEditorCore> MSCEditorCorePtr;
Q_DECLARE_METATYPE(MSCEditorCorePtr)
