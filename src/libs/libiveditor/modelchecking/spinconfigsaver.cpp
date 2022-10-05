#include "spinconfigsaver.h"

#include <QDebug>

SpinConfigSaver::SpinConfigSaver() { }

SpinConfigData SpinConfigSaver::getConfigData() const
{
    return configData;
}

QString SpinConfigSaver::explorationModeToString(const ExplorationMode &explorationMode)
{
    switch (explorationMode) {
    case ExplorationMode::BreadthFirst:
        return QString("BreadthFirst");
    case ExplorationMode::DepthFirst:
        return QString("DepthFirst");
    }
    return "";
}

ExplorationMode SpinConfigSaver::explorationModeFromStringRef(const QStringRef &explorationModeStr)
{
    if (explorationModeStr == "BreadthFirst") {
        return ExplorationMode::BreadthFirst;
    } else {
        return ExplorationMode::DepthFirst;
    }
}

QString SpinConfigSaver::interfaceGenerationLimitsToString(const QList<QPair<QString, int>> &interfaceGenerationLimits)
{
    QString generationLimitsStr;
    for (const auto &limitPair : interfaceGenerationLimits) {
        generationLimitsStr += limitPair.first + " " + limitPair.second + ";";
    }
    return generationLimitsStr;
}

QList<QPair<QString, int>> SpinConfigSaver::parseIfaceGenerationLimits(const QStringRef &interfaceGenerationLimitsStr)
{
    constexpr int INTERFACE_IDX = 0;
    constexpr int LIMIT_IDX = 1;
    constexpr int LIMIT_PAIR_SIZE = 2;

    QList<QPair<QString, int>> ifaceGenerationLimits;
    for (const auto limitPairStr : interfaceGenerationLimitsStr.split(";")) {
        QVector<QStringRef> limitPairVect = limitPairStr.split(" ");
        if (limitPairVect.length() == LIMIT_PAIR_SIZE) {
            ifaceGenerationLimits.push_back(
                    QPair<QString, int> { limitPairVect[INTERFACE_IDX].toString(), limitPairVect[LIMIT_IDX].toInt() });
        }
    }
    return ifaceGenerationLimits;
}

QStringRef readAttribute(const QString &attribute, const QXmlStreamReader &xml)
{
    if (xml.attributes().hasAttribute(attribute)) {
        return xml.attributes().value(attribute);
    } else {
        qWarning() << "Error, no attribute " << attribute << " in the xmel file";
    }
    return {};
}

void SpinConfigSaver::saveSpinConfig(const SpinConfigData &configData, QXmlStreamWriter &xml)
{
    xml.writeStartElement("SPINModelChecker");

    xml.writeAttribute("errorlimit", QString::number(configData.errorLimit));
    xml.writeAttribute("explorationmode", explorationModeToString(configData.explorationMode));
    xml.writeAttribute(
            "globalinputvectorgenerationlimit", QString::number(configData.globalInputVectorGenerationLimit));
    xml.writeAttribute(
            "interfacegenerationlimits", interfaceGenerationLimitsToString(configData.interfaceGenerationLimits));
    xml.writeAttribute("memorylimitmb", QString::number(configData.memoryLimitMB));
    xml.writeAttribute("numberofcores", QString::number(configData.numberOfCores));
    xml.writeAttribute("searchshortestpath", QString::number(configData.searchShortestPath));
    xml.writeAttribute("searchstatelimit", QString::number(configData.searchStateLimit));
    xml.writeAttribute("timelimitseconds", QString::number(configData.timeLimitSeconds));
    xml.writeAttribute("usebithashing", QString::number(configData.useBitHashing));
    xml.writeAttribute("usefairscheduling", QString::number(configData.useFairScheduling));

    xml.writeEndElement();
}

bool SpinConfigSaver::readSpinConfig(QXmlStreamReader &xml)
{
    configData.errorLimit = readAttribute("errorlimit", xml).toInt();
    configData.explorationMode = explorationModeFromStringRef(readAttribute("explorationmode", xml));
    configData.globalInputVectorGenerationLimit = readAttribute("globalinputevectorgenerationlimit", xml).toInt();
    configData.interfaceGenerationLimits = parseIfaceGenerationLimits(readAttribute("interfacegenerationlimits", xml));
    configData.memoryLimitMB = readAttribute("memorylimitmb", xml).toInt();
    configData.numberOfCores = readAttribute("numberofcores", xml).toInt();
    configData.searchShortestPath = readAttribute("searchshortestpath", xml).toInt();
    configData.searchStateLimit = readAttribute("searchstatelimit", xml).toInt();
    configData.useBitHashing = readAttribute("usebithashing", xml).toInt();
    configData.useFairScheduling = readAttribute("usefairscheduling", xml).toInt();

    xml.skipCurrentElement();
    return true;
}
