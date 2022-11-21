/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "spinrunworker.h"

#include <QDebug>
#include <QEventLoop>
#include <QSettings>
#include <tmc/TmcConfig/constants.h>
#include <tmc/TmcVerifier/verifier.h>

using tmc::verifier::TmcVerifier;

namespace ive {
SpinRunWorker::SpinRunWorker(QObject *parent, const QStringList &properties, const QStringList &subtypes,
        const QStringList &functions, const SpinConfigData &config, const QString &projectRoot,
        const QString &outputDirectory)
    : QThread(parent)
    , m_propertiesSelected(properties)
    , m_subtypesSelected(subtypes)
    , m_functionsSelected(functions)
    , m_spinConfig(config)
    , m_projectRoot(projectRoot)
    , m_outputDirectory(outputDirectory)
{
}

SpinRunWorker::~SpinRunWorker() {}

void SpinRunWorker::setSubtypesPath(const QString &subtypesPath)
{
    m_subtypesPath = subtypesPath;
}

void SpinRunWorker::setPropertiesPath(const QString &propertiesPath)
{
    m_propertiesPath = propertiesPath;
}

void SpinRunWorker::stop()
{
    quit();
}

void SpinRunWorker::run()
{
    QString interfaceView = m_projectRoot + QDir::separator() + "interfaceview.xml";
    QDir::setCurrent(m_projectRoot);
    QDir projectRootDir(m_projectRoot);

    if (projectRootDir.exists(m_outputDirectory)) {
        QDir outputDirectory(projectRootDir.filePath(m_outputDirectory));
        outputDirectory.removeRecursively();
    }

    projectRootDir.mkdir(m_outputDirectory);

    m_verifier = std::make_unique<TmcVerifier>(interfaceView, m_outputDirectory);

    m_verifier->setExplorationMode(m_spinConfig.explorationMode == ExplorationMode::DepthFirst
                    ? TmcVerifier::ExplorationMode::DepthFirst
                    : TmcVerifier::ExplorationMode::BreadthFirst);

    if (m_spinConfig.supportReal) {
        m_verifier->setRealTypeEnabled(true);
        if (m_spinConfig.deltaValue.has_value()) {
            m_verifier->setDelta(QString::number(m_spinConfig.deltaValue.value()));
        } else {
            m_verifier->setDelta("");
        }
    } else {
        m_verifier->setRealTypeEnabled(false);
    }

    m_verifier->setSearchShortestPath(m_spinConfig.searchShortestPath);
    m_verifier->setUseFairScheduling(m_spinConfig.useFairScheduling);
    m_verifier->setUseBitHashing(m_spinConfig.useBitHashing);
    if (m_spinConfig.numberOfCores.has_value()) {
        m_verifier->setNumberOfCores(m_spinConfig.numberOfCores.value());
    }
    if (m_spinConfig.timeLimitSeconds.has_value()) {
        m_verifier->setTimeLimit(m_spinConfig.timeLimitSeconds.value());
    }
    if (m_spinConfig.searchStateLimit.has_value()) {
        m_verifier->setSearchStateLimit(m_spinConfig.searchStateLimit.value());
    }
    if (m_spinConfig.errorLimit.has_value()) {
        m_verifier->setErrorLimit(m_spinConfig.errorLimit.value());
    }
    if (m_spinConfig.memoryLimitMB.has_value()) {
        m_verifier->setMemoryLimit(m_spinConfig.memoryLimitMB.value());
    }
    if (!m_spinConfig.rawCommandLine.isEmpty()) {
        m_verifier->setRawCommandline(m_spinConfig.rawCommandLine);
    }

    QSettings settings;
    QVariant spinExecutable = settings.value(tmc::TmcConstants::SETTINGS_TMC_SPIN_EXE_KEY);
    if (spinExecutable.isValid()) {
        m_verifier->setSpinExecutable(spinExecutable.toString());
    }

    if (m_spinConfig.globalInputVectorGenerationLimit.has_value()) {
        m_verifier->setGlobalInputVectorLengthLimit(
                QString::number(m_spinConfig.globalInputVectorGenerationLimit.value()));
    }

    if (!m_spinConfig.ifaceGenerationLimits.isEmpty()) {
        std::unordered_map<QString, QString> values;
        for (const QPair<QString, int> &limit : m_spinConfig.ifaceGenerationLimits) {
            values.emplace(limit.first, QString::number(limit.second));
        }
        m_verifier->setInterfaceInputVectorLengthLimits(values);
    }

    std::vector<QString> subtypes;
    for (const QString &subtype : m_subtypesSelected) {
        subtypes.push_back(m_subtypesPath + QDir::separator() + subtype);
    }
    m_verifier->setSubtypesFilepaths(subtypes);

    QStringList stopConditionFiles;
    QStringList mscObserverFiles;
    for (const QString &property : m_propertiesSelected) {
        if (property.endsWith(".pr")) {
            m_verifier->attachObserver(m_propertiesPath + QDir::separator() + property, 1);
        } else if (property.endsWith(".msc")) {
            mscObserverFiles.append(m_propertiesPath + QDir::separator() + property);
        } else if (property.endsWith(".scl")) {
            stopConditionFiles.append(m_propertiesPath + QDir::separator() + property);
        }
    }
    if (!stopConditionFiles.isEmpty()) {
        m_verifier->addStopConditionFiles(stopConditionFiles);
    }
    if (!mscObserverFiles.isEmpty()) {
        m_verifier->setMscObserverFiles(mscObserverFiles);
    }
    m_verifier->setKeepFunctions(std::vector<QString>(m_functionsSelected.begin(), m_functionsSelected.end()));

    QEventLoop loop;
    connect(m_verifier.get(), SIGNAL(verifierMessage(QString)), this, SIGNAL(textAvailable(QString)));
    connect(m_verifier.get(), SIGNAL(finished(bool)), this, SIGNAL(jobFinished(bool)));
    connect(m_verifier.get(), SIGNAL(finished(bool)), &loop, SLOT(quit()), Qt::QueuedConnection);

    m_verifier->execute(TmcVerifier::ExecuteMode::ConvertAndVerify);

    loop.exec();

    m_verifier.reset();
}
}
