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

#include "msceditorcore.h"

#include "chartlayoutmanager.h"
#include "commandlineparser.h"
#include "commands/cmddeleteentity.h"
#include "commands/cmddocumentcreate.h"
#include "commands/cmdentitynamechange.h"
#include "commands/cmdsetasn1file.h"
#include "commands/cmdsetmessagedeclarations.h"
#include "hierarchyviewmodel.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "mainmodel.h"
#include "messagedeclarationsdialog.h"
#include "mscappwidget.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "systemchecks.h"
#include "ui/graphicsviewbase.h"

#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QFileInfo>
#include <QMainWindow>
#include <QMenu>
#include <QMimeData>
#include <QStackedWidget>
#include <QUndoGroup>
#include <QUndoStack>
#include <QUrl>

namespace msc {

/*!
 * \class MSCEditorCore
 * \brief Has most of the code for the MSC Editor UI and the underlying library
 */

MSCEditorCore::MSCEditorCore(QObject *parent)
    : shared::EditorCore(parent)
    , m_model(new msc::MainModel())
{
    setSystemChecker(new SystemChecks(this));

    connect(m_model->commandsStack(), &msc::MscCommandsStack::nameChanged, this, &msc::MSCEditorCore::nameChanged);

    connect(&(mainModel()->chartViewModel()), &msc::ChartLayoutManager::initialNameAccepted, this,
            [this](MscEntity *entity) {
                if (!entity) {
                    return;
                }
                Q_EMIT nameChanged(entity, entity->name(), nullptr);
            });
}

MSCEditorCore::~MSCEditorCore() { }

MainModel *MSCEditorCore::mainModel() const
{
    return m_model.get();
}

shared::ui::GraphicsViewBase *MSCEditorCore::chartView()
{
    return m_mainWidget->chartView();
}

void MSCEditorCore::initConnections()
{
    if (m_connectionsDone) {
        return;
    }

    connect(&(m_model->chartViewModel()), &msc::ChartLayoutManager::initialNameAccepted, this,
            [this](MscEntity *entity) {
                if (!entity) {
                    return;
                }
                Q_EMIT nameChanged(entity, entity->name(), nullptr);
            });

    m_connectionsDone = true;
}

void MSCEditorCore::addToolBars(QMainWindow *window)
{
    window->addToolBar(mainToolBar());
}

MscAppWidget *MSCEditorCore::mainwidget()
{
    if (!m_mainWidget) {
        m_mainWidget = new MscAppWidget(this);
        m_model->chartViewModel().preferredChartBoxSize();
    }
    return m_mainWidget;
}

void MSCEditorCore::populateCommandLineArguments(shared::CommandLineParser *parser) const
{
    parser->handlePositional(shared::CommandLineParser::Positional::OpenFileMsc);
    parser->handlePositional(shared::CommandLineParser::Positional::DbgOpenMscExamplesChain);
    parser->handlePositional(shared::CommandLineParser::Positional::DropUnsavedChangesSilently);
}

QAction *MSCEditorCore::actionMessageDeclarations()
{
    if (!m_actionMessageDeclarations) {
        m_actionMessageDeclarations =
                new QAction(QIcon(QLatin1String(":/icons/message_declarations.svg")), tr("Show messages ..."));
        connect(m_actionMessageDeclarations, &QAction::triggered, this,
                [this]() { openMessageDeclarationEditor(nullptr); });
    }
    return m_actionMessageDeclarations;
}

QAction *MSCEditorCore::createActionCopy(QMainWindow *window)
{
    if (m_actionCopy == nullptr) {
        if (window != nullptr) {
            m_actionCopy = new QAction(tr("Copy:"), this);
            m_actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
            m_actionCopy->setMenu(new QMenu(window));
            m_actionCopy->menu()->addAction(
                    tr("Copy Diagram"), m_model.get(), &msc::MainModel::copyCurrentChart, QKeySequence::Copy);
            m_actionCopy->menu()->addAction(
                    tr("Copy as Picture"), m_model.get(), &msc::MainModel::copyCurrentChartAsPicture);
        } else {
            m_actionCopy = new QAction(tr("Copy Diagram"), this);
            m_actionCopy->setShortcut(QKeySequence::Copy);
            m_actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
            connect(m_actionCopy, &QAction::triggered, m_model.get(), &msc::MainModel::copyCurrentChart);
        }
    }
    return m_actionCopy;
}

QAction *MSCEditorCore::createActionPaste(QMainWindow *window)
{
    if (m_actionPaste == nullptr) {
        if (window) {
            m_actionPaste = new QAction(tr("Paste:"), window);
            m_actionPaste->setShortcut(QKeySequence::Paste);
            m_actionPaste->setIcon(QIcon::fromTheme("edit-paste"));
            connect(m_actionPaste, &QAction::triggered, m_model.get(), &msc::MainModel::pasteChart);
        } else {
            m_actionPaste = new QAction(tr("Paste:"), this);
            m_actionPaste->setShortcut(QKeySequence::Paste);
            m_actionPaste->setIcon(QIcon::fromTheme("edit-paste"));
            connect(m_actionPaste, &QAction::triggered, m_model.get(), &msc::MainModel::pasteChart);
        }
    }
    return m_actionPaste;
}

/*!
   Sets the checker for iv consistency
 */
void MSCEditorCore::setSystemChecker(SystemChecks *checker)
{
    if (checker == nullptr) {
        return;
    }

    if (m_systemChecks && m_systemChecks->parent() == this) {
        delete m_systemChecks;
    }
    m_systemChecks = checker;
    m_model->chartViewModel().setSystemChecker(m_systemChecks);
}

/*!
   Returns the checker for iv consistency
 */
SystemChecks *MSCEditorCore::systemChecker() const
{
    return m_systemChecks;
}

QUndoStack *MSCEditorCore::undoStack() const
{
    return m_model->undoStack();
}

msc::MscCommandsStack *MSCEditorCore::commandsStack() const
{
    return m_model->commandsStack();
}

/*!
   Changes the asn1 referenceto \p newName if the existing one if pointing to \p oldName
 */
bool MSCEditorCore::renameAsnFile(const QString &oldName, const QString &newName)
{
    QFileInfo oldFile(oldName);
    const QString oldFileName = oldFile.fileName();
    QFileInfo newFile(newName);
    const QString newFileName = newFile.fileName();

    MscModel *mscModel = m_model->mscModel();
    if (mscModel->dataDefinitionString() == oldFileName) {
        commandsStack()->push(new cmd::CmdSetAsn1File(mscModel, newFileName, "ASN.1"));

        Q_EMIT editedExternally(this);
        return true;
    }
    return false;
}

/*!
   Changes all instances that have the name \p oldName to have the new name \p name
 */
void MSCEditorCore::changeMscInstanceName(const QString &oldName, const QString &name)
{
    bool updated = false;
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscInstance *instance : chart->instances()) {
            if (instance->name() == oldName) {
                msc::MscCommandsStack *undo = commandsStack();
                auto cmd = new msc::cmd::CmdEntityNameChange(instance, name, nullptr);
                undo->push(cmd);
                updated = true;
            }
        }
    }
    if (updated) {
        Q_EMIT editedExternally(this);
    }
}

/*!
   Changes all messages that have the name \p oldName to have the new name \p newName, if the source and target have the
   names \p sourceName and \p targetName
 */
void MSCEditorCore::changeMscMessageName(
        const QString &oldName, const QString &newName, const QString &sourceName, const QString &targetName)
{
    bool updated = false;
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscMessage *message : chart->messages()) {
            if (message->name() == oldName) {
                const QString messageSource = message->sourceInstance() ? message->sourceInstance()->name() : "";
                const QString messageTarget = message->targetInstance() ? message->targetInstance()->name() : "";
                if (messageSource == sourceName && messageTarget == targetName) {
                    msc::MscCommandsStack *undo = commandsStack();
                    auto cmd = new msc::cmd::CmdEntityNameChange(message, newName, nullptr);
                    undo->push(cmd);
                    updated = true;
                }
            }
        }
    }
    if (updated) {
        Q_EMIT editedExternally(this);
    }
}

/*!
   Removes all instance that are corresponding to the function \p ivFunction
 */
void MSCEditorCore::removeMscInstances(ivm::IVFunction *ivFunction)
{
    bool updated = false;
    msc::MscCommandsStack *undo = commandsStack();
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscInstance *instance : chart->instances()) {
            if (m_systemChecks->correspond(ivFunction, instance)) {
                auto cmd = new msc::cmd::CmdDeleteEntity({ instance }, chart);
                undo->push(cmd);
                updated = true;
            }
        }
    }

    if (updated) {
        Q_EMIT editedExternally(this);
    }
}

/*!
   Removes all messages that are corresponding to the connection \p ivConnection
 */
void MSCEditorCore::removeMscMessages(ivm::IVConnection *ivConnection)
{
    bool updated = false;
    msc::MscCommandsStack *undo = commandsStack();
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscMessage *message : chart->messages()) {
            if (m_systemChecks->correspond(ivConnection, message)) {
                auto cmd = new msc::cmd::CmdDeleteEntity({ message }, chart);
                undo->push(cmd);
                updated = true;
            }
        }
    }

    if (updated) {
        Q_EMIT editedExternally(this);
    }
}

/*!
   Returns a list of all corresponding instances for iv function \p IVFunction.
 */
QList<MscInstance *> MSCEditorCore::correspondingInstances(ivm::IVFunction *ivFunction) const
{
    QList<MscInstance *> corresponds;
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscInstance *instance : chart->instances()) {
            if (m_systemChecks->correspond(ivFunction, instance)) {
                corresponds.append(instance);
            }
        }
    }

    return corresponds;
}

/*!
   Returns a list of all corresponding messages for iv connection \p ivConnection.
 */
QList<MscMessage *> MSCEditorCore::correspondingMessages(ivm::IVConnection *ivConnection) const
{
    QList<MscMessage *> corresponds;
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscMessage *message : chart->messages()) {
            if (m_systemChecks->correspond(ivConnection, message)) {
                corresponds.append(message);
            }
        }
    }
    return corresponds;
}

QString MSCEditorCore::filePath() const
{
    return m_model->currentFilePath();
}

bool MSCEditorCore::save()
{
    if (!m_model) {
        return false;
    }

    return m_model->saveMsc(m_model->currentFilePath());
}

/*!
 * \brief msc::MSCEditorCore::selectCurrentChart Set the current chart as the currently selected.
 */
void MSCEditorCore::selectCurrentChart()
{
    msc::MscChart *chart = m_model->chartViewModel().currentChart();
    if (chart != nullptr) {
        connect(chart, &msc::MscEntity::commentChanged, m_mainWidget, &msc::MscAppWidget::checkGlobalComment,
                Qt::UniqueConnection);
    }
    m_mainWidget->checkGlobalComment();
}

void MSCEditorCore::openMessageDeclarationEditor(QWidget *parentwidget)
{
    msc::MscModel *model = mainModel()->mscModel();
    if (!model) {
        return;
    }

    QVector<msc::MscDocument *> docs = model->documents();
    if (docs.isEmpty()) {
        return;
    }

    MessageDeclarationsDialog dialog(
            docs.at(0)->messageDeclarations(), model, commandsStack(), m_systemChecks, parentwidget);
    dialog.setFileName(model->dataDefinitionString());
    dialog.setIVConnectionNames(m_systemChecks->connectionNames());
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        commandsStack()->beginMacro("Edit message declarations");
        commandsStack()->push(new cmd::CmdSetMessageDeclarations(docs.at(0), dialog.declarations()));
        commandsStack()->push(new cmd::CmdSetAsn1File(model, dialog.fileName(), "ASN.1"));
        commandsStack()->endMacro();
    }
}

/*!
   Adds a MSC document if the hierarchy view is active and a "non-leaf" document is selected
 */
void MSCEditorCore::addDocument(MscDocument::HierarchyType type)
{
    if (!m_mainWidget || m_mainWidget->viewMode() != MscAppWidget::ViewMode::HIERARCHY) {
        return;
    }
    MscDocument *parentDoc = m_model->hierarchyViewModel().selectedDocument();
    if (!parentDoc) {
        return;
    }
    if (!parentDoc->isAddChildEnable()) {
        return;
    }

    MscDocument *document = new MscDocument(QObject::tr("Document_%1").arg(parentDoc->documents().size()));
    document->setHierarchyType(type);

    commandsStack()->push(new cmd::CmdDocumentCreate(document, parentDoc));

    m_model->setSelectedDocument(document);
}

QUrl MSCEditorCore::helpPage() const
{
    return QUrl("https://taste.tuxfamily.org/wiki/index.php?title=MSC_tutorial");
}
}
