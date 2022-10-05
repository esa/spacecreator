#ifndef SPINCONFIGSAVER_H
#define SPINCONFIGSAVER_H

#include <QList>
#include <QIODevice>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

enum class ExplorationMode
{
    DepthFirst,
    BreadthFirst
};

struct SpinConfigData
{
    ExplorationMode explorationMode = ExplorationMode::DepthFirst;
    bool searchShortestPath = false;
    bool useFairScheduling = false;
    bool useBitHashing = false;
    int numberOfCores = 1;
    int timeLimitSeconds = 300;
    int searchStateLimit = 1000000;
    int errorLimit = 1;
    int memoryLimitMB = 2048;
    int globalInputVectorGenerationLimit = 4;
    QList<QPair<QString, int>> interfaceGenerationLimits;
    QString rawCommandLine; // if non-empty, overrides other configuration settings
};

class SpinConfigSaver
{
public:
    SpinConfigSaver();
    void saveSpinConfig(const SpinConfigData &configData, QXmlStreamWriter &xml);
    bool readSpinConfig(QXmlStreamReader &xml);
    SpinConfigData getConfigData() const;
private:
    QString explorationModeToString(const ExplorationMode &explorationMode);
    ExplorationMode explorationModeFromStringRef(const QStringRef &explorationModeStr);
    QString interfaceGenerationLimitsToString(const QList<QPair<QString, int>> &interfaceGenerationLimits);
    QList<QPair<QString, int>> parseIfaceGenerationLimits(const QStringRef &interfaceGenerationLimitsStr);
    SpinConfigData configData;
};

#endif
