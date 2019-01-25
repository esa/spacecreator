#pragma once

#include "mscplugin_global.h"

#include <extensionsystem/iplugin.h>

namespace MscPlugin {
namespace Internal {

class MscPluginPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "MscPlugin.json")

public:
    MscPluginPlugin();
    ~MscPluginPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    void triggerAction();
};

} // namespace Internal
} // namespace MscPlugin
