#pragma once

#include <extensionsystem/iplugin.h>

namespace spctr {

class Opus2Plugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Opus2Plugin.json")

public:
    Opus2Plugin();
    ~Opus2Plugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    void addOpus2Option();
};

}

