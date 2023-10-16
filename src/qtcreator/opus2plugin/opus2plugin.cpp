#include "opus2plugin.h"

#include "opus2pluginconstants.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <projectexplorer.h>
#include <projectexplorer/projectmanager.h>
#if QTC_VERSION >= 1100
#include <projectexplorer/projecttree.h>
#else
#include <projectexplorer/session.h>
#endif

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

namespace spctr {

Opus2Plugin::Opus2Plugin() { }

Opus2Plugin::~Opus2Plugin() { }

bool Opus2Plugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    addOpus2Menu();

    return true;
}

void Opus2Plugin::addOpus2Menu()
{
    Core::Context allContexts(
            Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    Core::ActionContainer *const acToolsOpus2 = Core::ActionManager::createMenu(Constants::M_TOOLS_OPUS2);
    m_opus2Menu = acToolsOpus2->menu();
    m_opus2Menu->setTitle(tr("&Opus2"));
    m_opus2Menu->setEnabled(false);

    Core::ActionContainer *const tools = Core::ActionManager::actionContainer(Core::Constants::M_TOOLS);
    tools->addMenu(acToolsOpus2);

    const auto runPopulationToolAction = new QAction(tr("Population tool"), this);
    connect(runPopulationToolAction, &QAction::triggered, [this]() { runPopulationTool(); });
    Core::Command *const runPopulationTool = Core::ActionManager::registerAction(
            runPopulationToolAction, Constants::RUN_POPULATION_TOOL_ID, allContexts);
    acToolsOpus2->addAction(runPopulationTool);

    const auto runTailoringToolAction = new QAction(tr("Tailoring tool"), this);
    connect(runTailoringToolAction, &QAction::triggered, [this]() { runTailoringTool(); });
    Core::Command *const runTailoringTool =
            Core::ActionManager::registerAction(runTailoringToolAction, Constants::RUN_TAILORING_TOOL_ID, allContexts);
    acToolsOpus2->addAction(runTailoringTool);

    const auto runDocumentGeneratorAction = new QAction(tr("Document generator"), this);
    connect(runDocumentGeneratorAction, &QAction::triggered, [this]() { runDocumentGenerator(); });
    Core::Command *const runDocumentGenerator = Core::ActionManager::registerAction(
            runDocumentGeneratorAction, Constants::RUN_DOCUMENT_GENERATOR_ID, allContexts);
    acToolsOpus2->addAction(runDocumentGenerator);

    const auto runFrontendGeneratorAction = new QAction(tr("Frontend generator"), this);
    connect(runFrontendGeneratorAction, &QAction::triggered, [this]() { runFrontendGenerator(); });
    Core::Command *const runFrontendGenerator = Core::ActionManager::registerAction(
            runFrontendGeneratorAction, Constants::RUN_FRONTEND_GENERATOR_ID, allContexts);
    acToolsOpus2->addAction(runFrontendGenerator);

#if QTC_VERSION >= 1100
    connect(ProjectExplorer::ProjectManager::instance(), &ProjectExplorer::ProjectManager::startupProjectChanged, this,
            &Opus2Plugin::onActiveProjectChanged);
#else
    connect(ProjectExplorer::SessionManager::instance(), &ProjectExplorer::SessionManager::startupProjectChanged, this,
            &Opus2Plugin::onActiveProjectChanged);
#endif
}

void Opus2Plugin::runPopulationTool()
{
    initializeOpus2Model();
}

void Opus2Plugin::runTailoringTool()
{
    initializeOpus2Model();
}

void Opus2Plugin::runDocumentGenerator()
{
    initializeOpus2Model();
}

void Opus2Plugin::runFrontendGenerator()
{
    initializeOpus2Model();
}

void Opus2Plugin::initializeOpus2Model()
{
    if (m_currentProject == nullptr || m_currentProjectDirectoryPath.isEmpty()) {
        return;
    }

    const QString opus2ModelFilePath = m_currentProjectDirectoryPath + "/" + m_opus2ModelFileName;
    const QString opus2OptionsFilePath = m_currentProjectDirectoryPath + "/" + m_opus2OptionsFileName;

    if (!QFileInfo::exists(opus2ModelFilePath) || !QFileInfo::exists(opus2OptionsFilePath)) {
        QFile::remove(opus2ModelFilePath);
        QFile::remove(opus2OptionsFilePath);

        qDebug() << m_opus2DefaultModelFilePath << QFileInfo::exists(m_opus2DefaultModelFilePath) << "\n";
        qDebug() << m_opus2DefaultOptionsFilePath << QFileInfo::exists(m_opus2DefaultOptionsFilePath) << "\n";

        if (!QFile::copy(m_opus2DefaultModelFilePath, opus2ModelFilePath)
                || !QFile::copy(m_opus2DefaultOptionsFilePath, opus2OptionsFilePath)) {
            QMessageBox::critical(qApp->activeWindow(), tr("Opus2 initialization error"),
                    tr("Could not copy default Opus2 model from library. Is Opus2 installed?"));
        }
    }
}

void Opus2Plugin::onActiveProjectChanged(ProjectExplorer::Project *project)
{
    m_currentProject = project;
    if (m_currentProject != nullptr) {
        m_currentProjectDirectoryPath = project->projectDirectory().toString();
    } else {
        m_currentProjectDirectoryPath.clear();
    }

    m_opus2Menu->setEnabled(project != nullptr);
}

void Opus2Plugin::extensionsInitialized() { }

ExtensionSystem::IPlugin::ShutdownFlag Opus2Plugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

}
