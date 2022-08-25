#include "devicegeneratorplugin.h"
#include "devicegeneratorpluginconstants.h"

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

DeviceGeneratorPlugin::DeviceGeneratorPlugin()
{
}

DeviceGeneratorPlugin::~DeviceGeneratorPlugin()
{
}

bool DeviceGeneratorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    return true;
}

void DeviceGeneratorPlugin::extensionsInitialized()
{
}

ExtensionSystem::IPlugin::ShutdownFlag DeviceGeneratorPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

}
