#pragma once

#include "devicegeneratorplugin_global.h"

#include <extensionsystem/iplugin.h>

namespace spctr {

class DeviceGeneratorPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "DeviceGeneratorPlugin.json")

public:
    DeviceGeneratorPlugin();
    ~DeviceGeneratorPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();
};

}

