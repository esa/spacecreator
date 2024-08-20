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

#include "context/action/actionsmanager.h"
#include "dv/dveditorfactory.h"
#include "dveditor.h"
#include "dvlibrary.h"
#include "iv/iveditorfactory.h"
#include "iv/ivqtceditor.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "ivlibrary.h"
#include "msc/msceditorfactory.h"
#include "msceditor.h"
#include "msceditorcore.h"
#include "msclibrary.h"
#include "scl/scleditorfactory.h"
#include "sharedlibrary.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectimpl.h"
#include "spacecreatorprojectmanager.h"
#include "templatinglibrary.h"
#include "tracecommonlibrary.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QStandardPaths>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/icore.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>
#include <languageclient/languageclientmanager.h>
#include <languageclient/languageclientsettings.h>

void initSpaceCreatorResources()
{
    Q_INIT_RESOURCE(asn1_resources);
    shared::initSharedLibrary();
    initTraceCommonLibrary();
    msc::initMscLibrary();
    msc::initMscEditor();
    ive::initIVEditor();
    ivm::initIVLibrary();
    dve::initDvEditor();
    dvm::initDVLibrary();
    templating::initTemplatingLibrary();
}

using namespace Core;

namespace spctr {

SpaceCreatorPlugin::SpaceCreatorPlugin()
{
    initSpaceCreatorResources();
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

    m_projectsManager = new SpaceCreatorProjectManager(this);

    std::function<QStringList()> deploymentFilesCallback = [this]() {
        if (auto ivEditor = qobject_cast<spctr::IVQtCEditor *>(Core::EditorManager::currentEditor())) {
            SpaceCreatorProjectImpl *project = m_projectsManager->project(ivEditor->ivPlugin());
            if (project) {
                return project->allDVFiles();
            }
        }
        return QStringList();
    };
    ive::ActionsManager::registerDeploymentFilesCallback(deploymentFilesCallback);

    m_sclFactory = new SclEditorFactory();
    m_mscFactory = new MscEditorFactory(m_projectsManager, this);
    m_ivFactory = new IVEditorFactory(m_projectsManager, this);
    m_dvFactory = new DVEditorFactory(m_projectsManager, this);

    addHelp();

    return true;
}

void SpaceCreatorPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
    setupTasteLanguageClients();
}

ExtensionSystem::IPlugin::ShutdownFlag SpaceCreatorPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void SpaceCreatorPlugin::addHelp()
{
    Core::ActionContainer *actions = Core::ActionManager::actionContainer(Core::Constants::M_HELP);
    Core::Context allContexts(
            Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    auto mscHelpAction = new QAction(tr("MSC editor help"), this);
    connect(mscHelpAction, &QAction::triggered, this, []() {
        msc::MSCEditorCore core;
        core.showHelp();
    });
    Core::Command *showMscHelp =
            Core::ActionManager::registerAction(mscHelpAction, Constants::MSC_HELP_ID, allContexts);
    actions->addAction(showMscHelp);

    auto iveHelpAction = new QAction(tr("Interface editor help"), this);
    connect(iveHelpAction, &QAction::triggered, this, []() {
        ive::IVEditorCore core;
        core.showHelp();
    });
    Core::Command *showIveHelp = Core::ActionManager::registerAction(iveHelpAction, Constants::IV_HELP_ID, allContexts);
    actions->addAction(showIveHelp);

    // Add directory to search for *.qch help files
    static const QString DOC_PATHS = "Help/InstalledDocumentation";
    QSettings qSettings;
    QStringList docDirs = qSettings.value(DOC_PATHS).toStringList();
    const QString spaceCreatorDocDir = QString("%1/tool-inst/share/doc/SpaceCreator")
                                               .arg(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    if (!docDirs.contains(spaceCreatorDocDir)) {
        docDirs.append(spaceCreatorDocDir);
        qSettings.setValue(DOC_PATHS, docDirs);
    }
}

void SpaceCreatorPlugin::setupTasteLanguageClients()
{
    if (ignoreTasteLanguageClients()) {
        return;
    }

    QString iniFilePath = findInitFileForTasteLanguageClients();
    QSettings qSettings(iniFilePath, QSettings::IniFormat);

    int size = qSettings.beginReadArray("LanguageClient");
    for (int i = 0; i < size; ++i) {
        qSettings.setArrayIndex(i);
        auto *stdIOSettings = new LanguageClient::StdIOSettings();
        stdIOSettings->m_name = qSettings.value("Name").toString();
        int startBehaviorInt = qSettings.value("StartBehavior").toInt();
        stdIOSettings->m_startBehavior = LanguageClient::BaseSettings::StartBehavior(startBehaviorInt);
        stdIOSettings->m_executable = Utils::FilePath::fromVariant(qSettings.value("Executable"));
        LanguageClient::LanguageFilter languageFilter;
        languageFilter.mimeTypes = qSettings.value("MimeTypes").toString().split(' ');
        languageFilter.filePattern = qSettings.value("FilePattern").toString().split(' ');
        stdIOSettings->m_languageFilter = languageFilter;
        stdIOSettings->m_enabled = qSettings.value("Enabled").toBool();
        stdIOSettings->m_initializationOptions = qSettings.value("InitializationOptions").toString();
        stdIOSettings->m_arguments = qSettings.value("Arguments").toString();

        LanguageClient::LanguageClientManager::registerClientSettings(stdIOSettings);
        LanguageClient::LanguageClientManager::enableClientSettings(stdIOSettings->m_id, stdIOSettings->m_enabled);
    }
    qSettings.endArray();
}

bool SpaceCreatorPlugin::ignoreTasteLanguageClients() const
{
    const QString IGNORETASTELANUAGECLIENTS = "IgnoreTasteLanuageClients";
    QSettings qSettings;
    qSettings.beginGroup("TasteLanguageClients");
    bool ignore = qSettings.value(IGNORETASTELANUAGECLIENTS, false).toBool();
    if (ignore == false) {
        qSettings.setValue(IGNORETASTELANUAGECLIENTS, true);
    }
    return ignore;
}

QString SpaceCreatorPlugin::findInitFileForTasteLanguageClients() const
{
    QDir appImageDir = findAppImageDir();
    QString iniFilePath = appImageDir.filePath("TasteLanguageClients.ini");
    QFileInfo iniFileInfo(iniFilePath);
    QString result = iniFileInfo.canonicalFilePath();
    if (result.isEmpty()) {
        qWarning() << "Could not find" << QDir::cleanPath(iniFilePath);
    }
    return result;
}

QDir SpaceCreatorPlugin::findAppImageDir() const
{
    QDir dir = QDir(QCoreApplication::applicationDirPath());
    dir.cdUp(); // dir is now the AppImageDir - the parent of the bin dir
    return dir;
}

}
