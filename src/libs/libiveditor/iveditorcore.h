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
#include "ivinterface.h"
#include "ui/graphicsviewbase.h"

#include <QPointer>
#include <QSharedPointer>
#include <QVector>

namespace ivm {
class AbstractSystemChecks;
class IVConnection;
class IVFunction;
}

namespace ive {
namespace cmd {
class CommandsStack;
}

class InterfaceDocument;
class IVAppWidget;

class IVEditorCore : public shared::EditorCore
{
    Q_OBJECT
public:
    explicit IVEditorCore(QObject *parent = nullptr);
    ~IVEditorCore() override;

    ive::InterfaceDocument *document() const;

    void setDvChecks(ivm::AbstractSystemChecks *checks);
    ivm::AbstractSystemChecks *dvChecks() const;

    shared::ui::GraphicsViewBase *chartView() override;
    QWidget *mainwidget();
    void addToolBars(QMainWindow *window) override;

    QUndoStack *undoStack() const override;
    cmd::CommandsStack *commandsStack() const;

    void populateCommandLineArguments(shared::CommandLineParser *parser) const override;
    bool renameAsnFile(const QString &oldName, const QString &newName) override;

    QString filePath() const override;
    bool save() override;

    void registerBasicActions();

    QAction *actionExportFunctions();
    QAction *actionExportType();
    QAction *actionToggleE2EView();

    ivm::IVFunction *addFunction(const QString &name, ivm::IVFunction *parent = nullptr);
    bool addConnection(QString name, const QString &fromInstanceName, const QString &toInstanceName);
    ivm::IVInterface *addInterface(QString name, const QString &functionName);

    bool renameIVFunction(const QString &oldName, const QString &newName);
    bool renameIVConnection(const QString &oldName, const QString &newName, const QString &fromInstanceName,
            const QString &toInstanceName);
    bool renameCyclicInterface(const QString &oldName, const QString &newName, const QString &functionName);

    ivm::IVConnectionLayerType *manageConnectionLayer(
            const QString &name, ivm::IVConnectionLayerType *parent = nullptr);

    QVector<ivm::IVFunction *> allIVFunctions() const;
    QVector<ivm::IVConnection *> allIVConnections() const;

    QStringList ivFunctionsNames() const;
    QStringList ivConnectionNames() const;

    void centerOnView();

public Q_SLOTS:
    void onSaveRenderRequested();

private Q_SLOTS:
    void updateIVItems();

private:
    void saveSceneRender(const QString &filePath) const;
    ivm::IVInterface *getInterface(
            const QString &ifName, ivm::IVInterface::InterfaceType ifType, ivm::IVFunction *parentFunction);
    QUrl helpPage() const override;

    ive::InterfaceDocument *m_document { nullptr };

    QAction *m_actionExportFunctions { nullptr };
    QAction *m_actionExportType { nullptr };
    QAction *m_actionToggleE2EView { nullptr };

    QVector<ivm::IVFunction *> m_ivFunctions;
    QVector<ivm::IVConnection *> m_ivConnections;
    QVector<ivm::IVConnectionLayerType *> m_ivLayers;

    Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;

    QPointer<ivm::AbstractSystemChecks> m_checks;

    IVAppWidget *m_mainWidget = nullptr;
};

}

typedef QSharedPointer<ive::IVEditorCore> IVEditorCorePtr;
Q_DECLARE_METATYPE(IVEditorCorePtr)
