#pragma once

#include <QMenu>
#include <QStandardPaths>
#include <extensionsystem/iplugin.h>
#include <projectexplorer/project.h>

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
    void addOpus2Menu();
    void runPopulationTool();
    void runTailoringTool();
    void runDocumentGenerator();
    void runFrontendGenerator();
    void initializeOpus2Model();
    void onActiveProjectChanged(ProjectExplorer::Project *project);

private:
    const QString m_opus2ModelFileName = "opus2_model.xml";
    const QString m_opus2OptionsFileName = "opus2_options.xml";
    const QString m_opus2DefaultModelFilePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
            + "/tool-inst/lib/Opus2/opus2/data/default_model/" + m_opus2ModelFileName;
    const QString m_opus2DefaultOptionsFilePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
            + "/tool-inst/lib/Opus2/opus2/data/default_model/" + m_opus2OptionsFileName;

    QMenu *m_opus2Menu = nullptr;
    ProjectExplorer::Project *m_currentProject = nullptr;
    QString m_currentProjectDirectoryPath;
};

}
