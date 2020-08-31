/*
   Copyright (C) 2018 - 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "msceditordata.h"

#include "mainwidget.h"
#include "msccontext.h"
#include "msceditorcore.h"
#include "msceditordocument.h"
#include "msceditorstack.h"
#include "mscpluginconstants.h"
#include "msctexteditor.h"

#include <QToolBar>
#include <QUndoGroup>
#include <QVBoxLayout>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/designmode.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editortoolbar.h>
#include <coreplugin/icore.h>
#include <coreplugin/idocument.h>
#include <coreplugin/infobar.h>
#include <coreplugin/minisplitter.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/outputpane.h>
#include <editormanager/editormanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projecttree.h>
#include <texteditor/texteditor.h>
#include <utils/icon.h>
#include <utils/qtcassert.h>
#include <utils/utilsicons.h>

namespace MscPlugin {

class MscTextEditorWidget : public TextEditor::TextEditorWidget
{
    Q_OBJECT
public:
    MscTextEditorWidget() = default;

    void finalizeInitialization() override { setReadOnly(true); }
};

class MscTextEditorFactory : public TextEditor::TextEditorFactory
{
public:
    MscTextEditorFactory()
    {
        setId(MscPlugin::Constants::K_MSC_EDITOR_ID);
        setEditorCreator([]() { return new MscTextEditor; });
        setEditorWidgetCreator([]() { return new MscTextEditorWidget; });
        setUseGenericHighlighter(true);
        setDuplicatedSupported(false);
    }

    MscTextEditor *create(MscPlugin::MainWidget *designWidget)
    {
        setDocumentCreator([designWidget]() { return new MscEditorDocument(designWidget); });
        return qobject_cast<MscTextEditor *>(createEditor());
    }
};

MscEditorData::MscEditorData(QObject *parent)
    : QObject(parent)
{
    m_contexts.add(MscPlugin::Constants::C_MSCEDITOR);

    QObject::connect(
            Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged, [this](Core::IEditor *editor) {
                if (editor && editor->document()->id() == Constants::K_MSC_EDITOR_ID) {
                    auto mscEditor = qobject_cast<MscTextEditor *>(editor);
                    QTC_ASSERT(mscEditor, return );
                    QWidget *dw = m_widgetStack->widgetForEditor(mscEditor);
                    QTC_ASSERT(dw, return );
                    m_widgetStack->setVisibleEditor(mscEditor);
                    m_mainToolBar->setCurrentEditor(mscEditor);
                    updateToolBar();
                }
            });

    m_editorFactory = new MscTextEditorFactory;
}

MscEditorData::~MscEditorData()
{
    if (m_context)
        Core::ICore::removeContextObject(m_context);

    if (m_modeWidget) {
        Core::DesignMode::unregisterDesignWidget(m_modeWidget);
        delete m_modeWidget;
        m_modeWidget = nullptr;
    }

    delete m_editorFactory;
}

void MscEditorData::fullInit()
{
    // Create widget-stack, toolbar, mainToolbar and whole design-mode widget
    m_widgetStack = new MscEditorStack;
    m_widgetToolBar = new QToolBar;
    m_mainToolBar = createMainToolBar();
    m_modeWidget = createModeWidget();

    // Create undo/redo group/actions
    m_undoGroup = new QUndoGroup(m_widgetToolBar);
    m_undoAction = m_undoGroup->createUndoAction(m_widgetToolBar);
    m_undoAction->setIcon(Utils::Icons::UNDO_TOOLBAR.icon());
    m_undoAction->setToolTip(tr("Undo (Ctrl + Z)"));

    m_redoAction = m_undoGroup->createRedoAction(m_widgetToolBar);
    m_redoAction->setIcon(Utils::Icons::REDO_TOOLBAR.icon());
    m_redoAction->setToolTip(tr("Redo (Ctrl + Y)"));

    Core::ActionManager::registerAction(m_undoAction, Core::Constants::UNDO, m_contexts);
    Core::ActionManager::registerAction(m_redoAction, Core::Constants::REDO, m_contexts);

    Core::Context mscContexts = m_contexts;
    mscContexts.add(Core::Constants::C_EDITORMANAGER);
    m_context = new MscContext(mscContexts, m_modeWidget, this);
    Core::ICore::addContextObject(m_context);

    Core::DesignMode::registerDesignWidget(
            m_modeWidget, QStringList(QLatin1String(MscPlugin::Constants::MSC_MIMETYPE)), m_contexts);
}

Core::IEditor *MscEditorData::createEditor()
{
    auto designWidget = new MainWidget;
    MscTextEditor *mscEditor = m_editorFactory->create(designWidget);

    m_undoGroup->addStack(designWidget->undoStack());
    m_widgetStack->add(mscEditor, designWidget);
    m_mainToolBar->addEditor(mscEditor);

    if (mscEditor) {
        Core::InfoBarEntry info(
                Core::Id(Constants::INFO_READ_ONLY), tr("This file can only be edited in <b>Design</b> mode."));
        info.setCustomButtonInfo(
                tr("Switch Mode"), []() { Core::ModeManager::activateMode(Core::Constants::MODE_DESIGN); });
        mscEditor->document()->infoBar()->addInfo(info);
    }

    connect(designWidget, &MscPlugin::MainWidget::asn1Selected, this, &MscEditorData::openEditor);

    return mscEditor;
}

/*!
   Opens the message declaration dialog for the current msc file
 */
void MscEditorData::editMessageDeclarations(QWidget *parentWidget)
{
    auto editorManager = Core::EditorManager::instance();
    Core::IDocument *currentDoc = editorManager->currentDocument();
    auto document = qobject_cast<MscPlugin::MscEditorDocument *>(currentDoc);
    if (document && document->designWidget()) {
        document->designWidget()->mscPlugin()->openMessageDeclarationEditor(parentWidget);
    }
}

QStringList MscEditorData::aadlFiles() const
{
    ProjectExplorer::Project *project = ProjectExplorer::ProjectTree::currentProject();
    if (!project) {
        return {};
    }

    QStringList result;
    for (Utils::FileName fileName : project->files(ProjectExplorer::Project::AllFiles)) {
        if (fileName.toString().endsWith(".xml", Qt::CaseInsensitive)) {
            result.append(fileName.toString());
        }
    }

    return result;
}

QVector<msc::MSCEditorCore *> MscEditorData::mscPlugins() const
{
    QVector<msc::MSCEditorCore *> plugins;
    for (MscTextEditor *editor : m_widgetStack->editors()) {
        auto mscEdit = qobject_cast<MscTextEditorWidget *>(editor->editorWidget());
        if (mscEdit) {
            auto document = qobject_cast<MscEditorDocument *>(mscEdit->textDocument());
            if (document) {
                MainWidget *designWidget = document->designWidget();
                if (designWidget && designWidget->mscPlugin()) {
                    plugins.append(designWidget->mscPlugin());
                }
            }
        }
    }
    return plugins;
}

void MscEditorData::openEditor(const QString &fileName)
{
    Core::EditorManager::instance()->openEditor(fileName);
}

void MscEditorData::setMinimapVisible(bool visible)
{
    m_minimapVisible = visible;

    for (auto openedDocument : Core::DocumentModel::openedDocuments()) {
        if (auto document = qobject_cast<MscPlugin::MscEditorDocument *>(openedDocument)) {
            if (document && document->designWidget()) {
                document->designWidget()->mscPlugin()->minimapView()->setVisible(visible);
            }
        }
    }
}

void MscEditorData::updateToolBar()
{
    auto designWidget = static_cast<MainWidget *>(m_widgetStack->currentWidget());
    if (designWidget && m_widgetToolBar) {
        m_undoGroup->setActiveStack(designWidget->undoStack());
        m_widgetToolBar->clear();
        m_widgetToolBar->addAction(m_undoAction);
        m_widgetToolBar->addAction(m_redoAction);
        m_widgetToolBar->addSeparator();
        m_widgetToolBar->addAction(designWidget->actionToolDelete());
        m_widgetToolBar->addSeparator();
        m_widgetToolBar->addAction(designWidget->actionCopy());
        m_widgetToolBar->addAction(designWidget->actionPaste());
        // m_widgetToolBar->addAction(designWidget->actionScreenshot());
        m_widgetToolBar->addSeparator();
        for (QAction *action : designWidget->toolActions()) {
            m_widgetToolBar->addAction(action);
        }
        m_widgetToolBar->addSeparator();
        for (QAction *action : designWidget->mscPlugin()->hierarchyActions()) {
            m_widgetToolBar->addAction(action);
        }

        designWidget->mscPlugin()->showToolbars(false);
    }
}

Core::EditorToolBar *MscEditorData::createMainToolBar()
{
    auto toolBar = new Core::EditorToolBar;
    toolBar->setToolbarCreationFlags(Core::EditorToolBar::FlagsStandalone);
    toolBar->setNavigationVisible(false);
    toolBar->addCenterToolBar(m_widgetToolBar);

    return toolBar;
}

QWidget *MscEditorData::createModeWidget()
{
    auto widget = new QWidget;

    widget->setObjectName("MscEditorDesignModeWidget");
    auto layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_mainToolBar);
    // Avoid mode switch to 'Edit' mode when the application started by
    // 'Run' in 'Design' mode emits output.
    auto splitter = new Core::MiniSplitter(Qt::Vertical);
    splitter->addWidget(m_widgetStack);
    auto outputPane = new Core::OutputPanePlaceHolder(Core::Constants::MODE_DESIGN, splitter);
    outputPane->setObjectName("DesignerOutputPanePlaceHolder");
    splitter->addWidget(outputPane);
    layout->addWidget(splitter);
    widget->setLayout(layout);

    return widget;
}

}

#include "msceditordata.moc"
