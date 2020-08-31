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

#include "aadleditordata.h"

#include "aadlcontext.h"
#include "aadleditordocument.h"
#include "aadleditorstack.h"
#include "aadlmainwidget.h"
#include "aadlpluginconstants.h"
#include "aadltexteditor.h"
#include "iveditorplugin.h"

#include <QToolBar>
#include <QUndoGroup>
#include <QVBoxLayout>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/designmode.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editortoolbar.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <coreplugin/idocument.h>
#include <coreplugin/infobar.h>
#include <coreplugin/minisplitter.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/outputpane.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <texteditor/texteditor.h>
#include <utils/icon.h>
#include <utils/qtcassert.h>
#include <utils/utilsicons.h>

namespace AadlPlugin {

class AadlTextEditorWidget : public TextEditor::TextEditorWidget
{
public:
    AadlTextEditorWidget() = default;

    void finalizeInitialization() override { setReadOnly(true); }
};

class AadlTextEditorFactory : public TextEditor::TextEditorFactory
{
public:
    AadlTextEditorFactory()
    {
        setId(AadlPlugin::Constants::K_AADL_EDITOR_ID);
        setEditorCreator([]() { return new AadlTextEditor; });
        setEditorWidgetCreator([]() { return new AadlTextEditorWidget; });
        setUseGenericHighlighter(true);
        setDuplicatedSupported(false);
    }

    AadlTextEditor *create(AadlMainWidget *designWidget)
    {
        setDocumentCreator([designWidget]() { return new AadlEditorDocument(designWidget); });
        return qobject_cast<AadlTextEditor *>(createEditor());
    }
};

AadlEditorData::AadlEditorData(QObject *parent)
    : QObject(parent)
{
    m_contexts.add(AadlPlugin::Constants::C_AADL_EDITOR);

    QObject::connect(
            Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged, [this](Core::IEditor *editor) {
                if (editor && editor->document()->id() == Constants::K_AADL_EDITOR_ID) {
                    auto aadlEditor = qobject_cast<AadlTextEditor *>(editor);
                    QTC_ASSERT(aadlEditor, return );
                    QWidget *dw = m_widgetStack->widgetForEditor(aadlEditor);
                    QTC_ASSERT(dw, return );
                    m_widgetStack->setVisibleEditor(aadlEditor);
                    m_mainToolBar->setCurrentEditor(aadlEditor);
                    updateToolBar();
                }
            });

    m_editorFactory = new AadlTextEditorFactory;
}

AadlEditorData::~AadlEditorData()
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

void AadlEditorData::fullInit()
{
    // Create widget-stack, toolbar, mainToolbar and whole design-mode widget
    m_widgetStack = new AadlEditorStack;
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

    Core::Context aadlContexts = m_contexts;
    aadlContexts.add(Core::Constants::C_EDITORMANAGER);
    m_context = new AadlContext(aadlContexts, m_modeWidget, this);
    Core::ICore::addContextObject(m_context);

    Core::DesignMode::registerDesignWidget(
            m_modeWidget, QStringList(QLatin1String(AadlPlugin::Constants::AADL_MIMETYPE)), m_contexts);
}

Core::IEditor *AadlEditorData::createEditor()
{
    auto designWidget = new AadlMainWidget;
    designWidget->setMinimapVisible(m_minimapVisible);
    AadlTextEditor *aadlEditor = m_editorFactory->create(designWidget);

    m_undoGroup->addStack(designWidget->undoStack());
    m_widgetStack->add(aadlEditor, designWidget);
    m_mainToolBar->addEditor(aadlEditor);

    if (aadlEditor) {
        Core::InfoBarEntry info(
                Core::Id(Constants::INFO_READ_ONLY), tr("This file can only be edited in <b>Design</b> mode."));
        info.setCustomButtonInfo(
                tr("Switch Mode"), []() { Core::ModeManager::activateMode(Core::Constants::MODE_DESIGN); });
        aadlEditor->document()->infoBar()->addInfo(info);
    }

    return aadlEditor;
}

void AadlEditorData::showAsn1Dialog()
{
    auto editorManager = Core::EditorManager::instance();
    Core::IDocument *currentDoc = editorManager->currentDocument();
    auto document = qobject_cast<AadlPlugin::AadlEditorDocument *>(currentDoc);
    if (document && document->designWidget()) {
        document->designWidget()->showAsn1Dialog();
    }
}

void AadlEditorData::showMinimap(bool visible)
{
    m_minimapVisible = visible;

    for (auto openedDocument : Core::DocumentModel::openedDocuments()) {
        if (auto document = qobject_cast<AadlPlugin::AadlEditorDocument *>(openedDocument)) {
            if (document && document->designWidget()) {
                document->designWidget()->setMinimapVisible(visible);
            }
        }
    }
}

void AadlEditorData::onAttributesManagerRequested()
{
    auto editorManager = Core::EditorManager::instance();
    Core::IDocument *currentDoc = editorManager->currentDocument();
    auto document = qobject_cast<AadlPlugin::AadlEditorDocument *>(currentDoc);
    if (document && document->designWidget()) {
        document->designWidget()->onAttributesManagerRequested();
    }
}

void AadlEditorData::onColorSchemeMenuInvoked()
{
    auto editorManager = Core::EditorManager::instance();
    Core::IDocument *currentDoc = editorManager->currentDocument();
    auto document = qobject_cast<AadlPlugin::AadlEditorDocument *>(currentDoc);
    if (document && document->designWidget()) {
        document->designWidget()->onColorSchemeMenuInvoked();
    }
}

void AadlEditorData::onDynContextEditorMenuInvoked()
{
    auto editorManager = Core::EditorManager::instance();
    Core::IDocument *currentDoc = editorManager->currentDocument();
    auto document = qobject_cast<AadlPlugin::AadlEditorDocument *>(currentDoc);
    if (document && document->designWidget()) {
        document->designWidget()->onDynContextEditorMenuInvoked();
    }
}

aadlinterface::IVEditorPlugin *AadlEditorData::ivPlugin(const QString &fileName)
{
    return m_widgetStack->ivPlugin(fileName);
}

void AadlEditorData::updateToolBar()
{
    auto designWidget = static_cast<AadlMainWidget *>(m_widgetStack->currentWidget());
    if (designWidget && m_widgetToolBar) {
        m_undoGroup->setActiveStack(designWidget->undoStack());
        m_widgetToolBar->clear();
        m_widgetToolBar->addAction(m_undoAction);
        m_widgetToolBar->addAction(m_redoAction);
        m_widgetToolBar->addSeparator();
        for (QAction *action : designWidget->toolActions()) {
            m_widgetToolBar->addAction(action);
        }
    }
}

Core::EditorToolBar *AadlEditorData::createMainToolBar()
{
    auto toolBar = new Core::EditorToolBar;
    toolBar->setToolbarCreationFlags(Core::EditorToolBar::FlagsStandalone);
    toolBar->setNavigationVisible(false);
    toolBar->addCenterToolBar(m_widgetToolBar);

    return toolBar;
}

QWidget *AadlEditorData::createModeWidget()
{
    auto widget = new QWidget;

    widget->setObjectName("AadlEditorDesignModeWidget");
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
