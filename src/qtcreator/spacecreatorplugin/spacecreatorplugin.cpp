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

#include "spacecreatorplugin.h"

#include "aadlchecks.h"
#include "aadleditordata.h"
#include "aadleditorfactory.h"
#include "aadlmodelstorage.h"
#include "asn1library.h"
#include "interface/interfacedocument.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "mscchart.h"
#include "msceditor.h"
#include "msceditorcore.h"
#include "msceditordata.h"
#include "msceditorfactory.h"
#include "mscinstance.h"
#include "msclibrary.h"
#include "mscmodelstorage.h"
#include "sharedlibrary.h"
#include "spacecreatorpluginconstants.h"

#include <QAction>
#include <QMessageBox>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/designmode.h>
#include <coreplugin/icore.h>
#include <coreplugin/idocument.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>
#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>

void initMscResources()
{
    Q_INIT_RESOURCE(asn1_resources);
    shared::initSharedLibrary();
    Asn1Acn::initAsn1Library();
    msc::initMscLibrary();
    msc::initMscEditor();
    aadlinterface::initIvEditor();
}

using namespace Core;

namespace spctr {

SpaceCreatorPlugin::SpaceCreatorPlugin()
{
    initMscResources();
}

SpaceCreatorPlugin::~SpaceCreatorPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool SpaceCreatorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    auto editorManager = Core::EditorManager::instance();

    m_aadlStorage = new AadlModelStorage(this);
    m_mscStorage = new MscModelStorage(this);
    m_mscFactory = new MscEditorFactory(this);
    m_aadlFactory = new AadlEditorFactory(m_aadlStorage, this);

    // MSC
    m_messageDeclarationAction = new QAction(tr("Message declarations ..."), this);
    Core::Command *messageDeclCmd =
            Core::ActionManager::registerAction(m_messageDeclarationAction, Constants::MESSAGE_DECLARATIONS_ID);
    connect(m_messageDeclarationAction, &QAction::triggered, this, &SpaceCreatorPlugin::showMessageDeclarations);

    m_messageDeclarationAction->setEnabled(false);
    connect(editorManager, &Core::EditorManager::currentEditorChanged, this, [&](Core::IEditor *editor) {
        if (editor && editor->document()) {
            const bool isMsc = editor->document()->filePath().toString().endsWith(".msc", Qt::CaseInsensitive);
            m_messageDeclarationAction->setEnabled(isMsc);
        }
    });

    auto action = new QAction(tr("Check instances"), this);
    Core::Command *checkInstancesCmd = Core::ActionManager::registerAction(
            action, Constants::CHECK_INSTANCES_ID, Core::Context(Core::Constants::C_GLOBAL));
    connect(action, &QAction::triggered, this, &SpaceCreatorPlugin::checkInstances);

    action = new QAction(tr("Check messages"), this);
    Core::Command *checkMessagesCmd = Core::ActionManager::registerAction(
            action, Constants::CHECK_MESSAGES_ID, Core::Context(Core::Constants::C_GLOBAL));
    connect(action, &QAction::triggered, this, &SpaceCreatorPlugin::checkMessages);

    auto showMinimapAction = new QAction(tr("Show minimap"), this);
    showMinimapAction->setCheckable(true);
    Core::Command *showMinimapCmd = Core::ActionManager::registerAction(
            showMinimapAction, Constants::SHOW_MINIMAP_ID, Core::Context(Core::Constants::C_DESIGN_MODE));
    connect(showMinimapAction, &QAction::triggered, this, &SpaceCreatorPlugin::setMinimapVisible);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("SpaceCreator"));
    menu->addAction(showMinimapCmd);
    menu->addSeparator();
    menu->addAction(messageDeclCmd);
    menu->addAction(checkInstancesCmd);
    menu->addAction(checkMessagesCmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    connect(m_mscFactory, &MscEditorFactory::mscDataLoaded, this,
            [this](const QString &fileName, QSharedPointer<msc::MSCEditorCore> mscData) {
                mscData->aadlChecker()->setIvCore(ivCore()); // All msc documents have access to the iv model
                m_mscStorage->setMscData(fileName, mscData);
            });

    // AADL
    m_asn1DialogAction = new QAction(tr("Show ASN1 dialog ..."), this);
    Core::Command *showAsn1Cmd = Core::ActionManager::registerAction(m_asn1DialogAction, Constants::AADL_SHOW_ASN1_ID);
    connect(m_asn1DialogAction, &QAction::triggered, this, &SpaceCreatorPlugin::showAsn1Dialog);

    m_asn1DialogAction->setEnabled(false);
    connect(editorManager, &Core::EditorManager::currentEditorChanged, this, [&](Core::IEditor *editor) {
        if (editor && editor->document()) {
            const bool isAadl =
                    editor->document()->filePath().toString().endsWith("interfaceview.xml", Qt::CaseInsensitive);
            m_asn1DialogAction->setEnabled(isAadl);
        }
    });

    QAction *actCommonProps = new QAction(tr("Common Properties"), this);
    Core::Command *showCommonPropsCmd = Core::ActionManager::registerAction(
            actCommonProps, Constants::AADL_SHOW_COMMON_PROPS_ID, Core::Context(Core::Constants::C_DESIGN_MODE));
    connect(actCommonProps, &QAction::triggered, this, &SpaceCreatorPlugin::onAttributesManagerRequested);

    QAction *actColorScheme = new QAction(tr("Color Scheme"), this);
    Core::Command *showColorSchemeCmd = Core::ActionManager::registerAction(
            actColorScheme, Constants::AADL_SHOW_COLOR_SCHEME_ID, Core::Context(Core::Constants::C_DESIGN_MODE));
    connect(actColorScheme, &QAction::triggered, this, &SpaceCreatorPlugin::onColorSchemeMenuInvoked);

    QAction *actDynContext = new QAction(tr("Context Actions"), this);
    Core::Command *showDynContextCmd = Core::ActionManager::registerAction(
            actDynContext, Constants::AADL_SHOW_DYN_CONTEXT_ID, Core::Context(Core::Constants::C_DESIGN_MODE));
    connect(actDynContext, &QAction::triggered, this, &SpaceCreatorPlugin::onDynContextEditorMenuInvoked);

    menu->addSeparator();
    menu->addAction(showAsn1Cmd);
    menu->addAction(showCommonPropsCmd);
    menu->addAction(showColorSchemeCmd);
    menu->addAction(showDynContextCmd);
    menu->menu()->setEnabled(true);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    connect(m_aadlFactory, SIGNAL(aadlDataLoaded(const QString &, QSharedPointer<aadlinterface::IVEditorCore>)),
            m_aadlStorage, SLOT(setIvData(const QString &, QSharedPointer<aadlinterface::IVEditorCore>)));

    return true;
}

void SpaceCreatorPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
    DesignMode::setDesignModeIsRequired();
}

ExtensionSystem::IPlugin::ShutdownFlag SpaceCreatorPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void SpaceCreatorPlugin::showMessageDeclarations()
{
    m_mscFactory->editorData()->editMessageDeclarations(Core::ICore::mainWindow());
}

void SpaceCreatorPlugin::checkInstances()
{
    QSharedPointer<aadlinterface::IVEditorCore> ivp = ivCore();
    if (!ivp) {
        return;
    }

    QVector<QSharedPointer<msc::MSCEditorCore>> mscCores = allMscCores();

    for (QSharedPointer<msc::MSCEditorCore> mplugin : mscCores) {
        mplugin->aadlChecker()->setIvCore(ivp);
    }

    // Check for names
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> resultNames;
    for (QSharedPointer<msc::MSCEditorCore> mplugin : mscCores) {
        resultNames += mplugin->aadlChecker()->checkInstanceNames();
    }

    // Check for nested functions usage
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> resultRelations;
    for (QSharedPointer<msc::MSCEditorCore> mplugin : mscCores) {
        resultRelations += mplugin->aadlChecker()->checkInstanceRelations();
    }

    QString text;
    if (!resultNames.isEmpty()) {
        text += tr("Following instances have no corresponding aadl function:\n");
        for (auto item : resultNames) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }
    if (!resultRelations.isEmpty()) {
        if (!text.isEmpty()) {
            text += "\n\n";
        }
        text += tr("Following instances are used with parent/hild of nested functions:\n");
        for (auto item : resultRelations) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }

    if (resultNames.isEmpty() && resultRelations.isEmpty()) {
        QMessageBox::information(nullptr, tr("All instaces are ok"), tr("All instaces are ok"));
    } else {
        QMessageBox::information(nullptr, tr("Non conforming instances"), text);
    }
}

void SpaceCreatorPlugin::checkMessages()
{
    QSharedPointer<aadlinterface::IVEditorCore> ivp = ivCore();
    if (!ivp) {
        return;
    }

    QVector<QSharedPointer<msc::MSCEditorCore>> mscCores = allMscCores();

    // check messages
    QVector<QPair<msc::MscChart *, msc::MscMessage *>> resultNames;
    for (QSharedPointer<msc::MSCEditorCore> mplugin : mscCores) {
        mplugin->aadlChecker()->setIvCore(ivp);
        resultNames += mplugin->aadlChecker()->checkMessages();
    }

    QString text;
    if (!resultNames.isEmpty()) {
        text += tr("Following messages have no corresponding aadl connection:\n");
        for (auto item : resultNames) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }
    if (resultNames.isEmpty()) {
        QMessageBox::information(nullptr, tr("All messages are ok"), tr("All messages are ok"));
    } else {
        QMessageBox::information(nullptr, tr("Non conforming messages"), text);
    }
}

void SpaceCreatorPlugin::setMinimapVisible(bool visible)
{
    m_mscFactory->editorData()->setMinimapVisible(visible);
    m_aadlFactory->editorData()->showMinimap(visible);
}

void SpaceCreatorPlugin::showAsn1Dialog()
{
    m_aadlFactory->editorData()->showAsn1Dialog();
}

void SpaceCreatorPlugin::onAttributesManagerRequested()
{
    m_aadlFactory->editorData()->onAttributesManagerRequested();
}

void SpaceCreatorPlugin::onColorSchemeMenuInvoked()
{
    m_aadlFactory->editorData()->onColorSchemeMenuInvoked();
}

void SpaceCreatorPlugin::onDynContextEditorMenuInvoked()
{
    m_aadlFactory->editorData()->onDynContextEditorMenuInvoked();
}

QSharedPointer<aadlinterface::IVEditorCore> SpaceCreatorPlugin::ivCore() const
{
    QStringList aadlFiles = m_mscFactory->editorData()->aadlFiles();
    if (aadlFiles.empty()) {
        qWarning() << "No AADL file in the projec";
        return {};
    }

    return m_aadlStorage->ivData(aadlFiles.first());
}

QVector<QSharedPointer<msc::MSCEditorCore>> SpaceCreatorPlugin::allMscCores() const
{
    QStringList mscFiles = m_mscFactory->editorData()->mscFiles();
    QVector<QSharedPointer<msc::MSCEditorCore>> allMscCores;
    for (const QString &mscFile : mscFiles) {
        QSharedPointer<msc::MSCEditorCore> core = m_mscStorage->mscData(mscFile);
        if (core) {
            allMscCores.append(core);
        }
    }
    return allMscCores;
}

}
