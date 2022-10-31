/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#pragma once

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <tmc/TmcConverter/converter.h>

namespace tmc::verifier {
/**
 * @brief Main class uses to process formal model verification on TASTE project.
 *
 */
class TmcVerifier final : public QObject
{
    Q_OBJECT
public:
    enum class ExplorationMode
    {
        DepthFirst,
        BreadthFirst,
    };

    enum class ExecuteMode
    {
        ConvertOnly,
        ConvertAndVerify,
    };

public:
    /**
     * @brief Constructor.
     *
     * @param   inputIvFilepath         Path to XML interface view.
     * @param   outputDirectory         Path to output directory.
     */
    TmcVerifier(const QString &inputIvFilepath, const QString &outputDirectory);

    /**
     * @brief   Set paths to MSC files to be converted to observers
     *
     * @param   mscObserverFiles    Paths to the files
     */
    void setMscObserverFiles(const QStringList &mscObserverFiles);
    /**
     * @brief   Specify which IV functions should be treated as an environment
     *          during model checking
     *
     * @param   environmentFunctions    Functions to treat as an evironment
     */
    void setEnvironmentFunctions(const std::vector<QString> &environmentFunctions);
    /**
     * @brief   Specify which IV functions shouldn't be treated as an environment
     *          during model checking
     *
     * @param   keepFunctions   Functions to be treated as an evironment
     */
    void setKeepFunctions(const std::vector<QString> &keepFunctions);
    /**
     * @brief   Set global input vector length limit
     *
     * @param   limit   Limit to set
     */
    void setGlobalInputVectorLengthLimit(std::optional<QString> limit);
    /**
     * @brief   Set per interface input vector length limits
     *
     * @param   limits  Limits to set
     */
    void setInterfaceInputVectorLengthLimits(std::unordered_map<QString, QString> limits);
    /**
     * @brief   Set non-environment processes base priority
     *
     * @param   value   Priority to set
     */
    void setProcessesBasePriority(std::optional<QString> value);
    /**
     * @brief   Set path to the ASN.1 containing subtypes
     *
     * @param   filepaths    Paths to the files
     */
    void setSubtypesFilepaths(const std::vector<QString> &filepath);
    /**
     * @brief   Enable or disable Real ASN.1 type
     *
     * @param   isRealTypeEnabled True if Real type should be enabled
     */
    void setRealTypeEnabled(bool isRealTypeEnabled);
    /**
     * @brief   Set delta (interval) for Real values generation
     *
     * @param   delta Value of the interval
     */
    void setDelta(std::optional<float> delta);
    /**
     * @brief Add Stop Condition files to verifier.
     *
     * @param files A list of paths with Stop Condition files.
     * @return true if files exist, otherwise false.
     */
    bool addStopConditionFiles(const QStringList &files);

    /**
     * @brief Attach an Observer
     *
     * @param observerPath Path to the observer process file
     * @param priority Observer priority
     * @return true if the operation succeeded, false otherwise.
     */
    bool attachObserver(const QString &observerPath, const uint32_t priority);

    void setExplorationMode(ExplorationMode mode);
    void setSearchShortestPath(bool enabled = true);
    void setUseFairScheduling(bool enabled = true);
    void setUseBitHashing(bool enabled = true);
    void setNumberOfCores(int value);
    void setTimeLimit(int timeLimit);
    void setSearchStateLimit(int searchStateLimit);
    void setErrorLimit(int errorLimit);
    void setMemoryLimit(int memoryLimit);
    void setRawCommandline(QString rawCommandline);

    /**
     * @brief Prepare the system and process formal model verification.
     *
     * @param onlyConvert if true, only convert model to promela - do not process verification
     * @return true if whole process succed, otherwise false
     */
    bool execute(ExecuteMode executeMode);

Q_SIGNALS:
    void verifierMessage(QString text);
    void finished(bool success);

private:
    void buildVerifier();
    void executeVerifier();

    void executeSpin();
    void executeCC();
    void collectErrors();
    void generateTraces(int count);
    void generateNextTrace();
    void generateReport();
    void saveReport(const QString &data);

private Q_SLOTS:
    void processStderrReady();
    void processStdoutReady();
    void verifierStderrReady();
    void verifierStdoutReady();
    void traceStderrReady();
    void traceStdoutReady();
    void conversionFinished(bool success);
    void spinStarted();
    void spinFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void ccStarted();
    void ccFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void panStarted();
    void panFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void traceGeneratorStarted();
    void traceGeneratorFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void timeout();

private:
    const QString m_inputIvFilepath;
    const QString m_outputDirectory;
    std::unique_ptr<::tmc::converter::TmcConverter> m_converter;

    ExplorationMode m_explorationMode;
    bool m_searchShortestPath;
    bool m_useFairScheduling;
    bool m_useBitHashing;
    std::optional<int> m_numberOfCores;
    std::optional<int> m_timeLimitSeconds;
    std::optional<int> m_searchStateLimit;
    std::optional<int> m_errorLimit;
    std::optional<int> m_memoryLimit;
    QString m_rawCommandline;

    ExecuteMode m_executeMode;
    QProcess *m_process;
    QProcess *m_verifierProcess;
    QProcess *m_traceGeneratorProcess;
    QTimer *m_timer;

    QStringList m_trailFilesToConvert;
    int m_trailCounter;
    QStringList m_spinTraceFiles;
    QStringList m_traceFiles;
    QString m_currentTraceFile;

    QMetaObject::Connection m_processStartedConnection;
    QMetaObject::Connection m_processFinishedConnection;
    QMetaObject::Connection m_conversionFinishedConnection;

    constexpr static int m_startTimeout = 30000;
    constexpr static int m_commandTimeout = 60000;
    constexpr static int m_verifierDefaultTimeout = 360000;
};
}
