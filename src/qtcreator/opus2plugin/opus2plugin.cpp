#include "opus2plugin.h"
#include "opus2pluginconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

namespace spctr {

Opus2Plugin::Opus2Plugin()
{
}

Opus2Plugin::~Opus2Plugin()
{
}

bool Opus2Plugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    addOpus2Option();

    return true;
}

void Opus2Plugin::addOpus2Option()
{
    Core::Context allContexts(Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    Core::ActionContainer *const acToolsOpus2 = Core::ActionManager::createMenu(Constants::M_TOOLS_OPUS2);
    QMenu *const opus2Menu = acToolsOpus2->menu();
    opus2Menu->setTitle(tr("&Opus2"));
    opus2Menu->setEnabled(true);

    Core::ActionContainer *const tools = Core::ActionManager::actionContainer(Core::Constants::M_TOOLS);
    tools->addMenu(acToolsOpus2);

    const auto runPopulationToolAction = new QAction(tr("Population tool"), this);
    //connect(csvImportAction, &QAction::triggered, [this]() { selectBoardDialog(); });
    Core::Command *const runPopulationTool = Core::ActionManager::registerAction(runPopulationToolAction, Constants::RUN_POPULATION_TOOL_ID, allContexts);
    acToolsOpus2->addAction(runPopulationTool);
}

void Opus2Plugin::extensionsInitialized()
{
}

ExtensionSystem::IPlugin::ShutdownFlag Opus2Plugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

}
