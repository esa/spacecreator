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

#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QProcess>
#include <QStringList>
#include <QTimer>
#include <conversion/common/modeltype.h>
#include <conversion/registry/registry.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <map>
#include <memory>
#include <tmc/SdlToPromelaConverter/converter.h>
#include <tmc/SdlToPromelaConverter/processmetadata.h>
#include <unordered_map>

namespace tmc::converter {
/**
 * @brief Main class used to convert TASTE project to format ready for formal model verification.
 */
class TmcConverter final : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Attached observer information
     */
    class ObserverInfo
    {
    public:
        /**
         * @brief Constructor
         *
         * @param path Path to the observer process file
         * @param priority Observer priority
         */
        ObserverInfo(const QString path, const uint32_t priority);
        /**
         * @brief Getter for the observer path
         *
         * @returns Path to the obsever process file
         */
        auto path() const -> const QString &;
        /**
         * @brief Getter for the observer priority
         *
         * @returns Observer priority
         */
        auto priority() const -> uint32_t;

    private:
        QString m_path;
        uint32_t m_priority;
    };

    /**
     * @brief Constructor.
     *
     * @param   inputIvFilepath         Path to XML interface view.
     * @param   outputDirectory         Path to output directory for conversion results.
     */
    TmcConverter(const QString &inputIvFilepath, const QString &outputDirectory, QObject *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~TmcConverter();

    /**
     * @brief Prepare system system conversion.
     *
     * This method shall be called after converter setup, but always before
     * calling convert or convertTrace.
     * This method reads input interface view and prepares everything what is
     * necessary to process actual conversion.
     *
     * @return true if preparation succeed, otherwise false.
     */
    bool prepare();

    /**
     * @brief Process system conversion.
     *
     * This method converts interface view, data view and SDL into promela files.
     */
    void convert();
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
    void setDelta(std::optional<QString> value);
    /**
     * @brief   Set support for multicast in source Interface View
     *
     * @param   isMulticastEnabled true is multicast should be enabled
     */
    void setMulticastEnabled(bool isMulticastEnabled);
    /**
     * @brief   Set timeout for external sdl2promela process.
     *
     * @param   timeout Timeout value.
     */
    void setSdl2PromelaTimeout(int timeout);
    /**
     * @brief   Set path to the ASN.1 containing subtypes
     *
     * @param   filepaths   Paths to the files
     */
    void setSubtypesFilepaths(const std::vector<QString> &filepaths);
    /**
     * @brief Add Stop Condition files to convert.
     *
     * This shall be called before @link{convert}
     *
     * @param files A list of paths with Stop Condition files.
     * @return true if files exist, otherwise false.
     */
    bool addStopConditionFiles(const QStringList &files);

    /**
     * @brief Getter for stop condition files.
     *
     * @return list of stop condition files.
     */
    const QStringList &getStopConditionFiles() const;

    /**
     * @brief Attach an Observer
     *
     * @param observerPath Path to the observer process file
     * @param priority Observer priority
     * @return true if the operation succeeded, false otherwise.
     */
    auto attachObserver(const QString &observerPath, const uint32_t priority) -> bool;

    /**
     * @brief Process trace conversion
     *
     * This method converts Spin Trail into Simulator Trail
     *
     * @param inputFile input spin trail filepath
     * @param outputFile output simulator trail filepath
     * @return true if conversion succeed, otherwise false.
     */
    bool convertTrace(const QString &inputFile, const QString &outputFile);

    /**
     * @brief Return number of proctypes in converted system.
     *
     * This doesn't include init proctype
     *
     * @return Number of proctypes
     */
    size_t getNumberOfProctypes() const;

    /**
     * @brief Convert ASN.1 files to promela and save the result in file
     *
     * @param inputFilepathList list of input ASN.1 files.
     * @param outputFilepath output promela file.
     * @return true if conversion succeed, otherwise false.
     */
    bool convertDataview(const QList<QString> &inputFilepathList, const QString &outputFilepath);

Q_SIGNALS:
    /**
     * @brief Conversion message.
     *
     * @param text message text.
     */
    void message(QString text);
    /**
     * @brief Conversion was finished.
     *
     * @param success true if conversion was successful, otherwise false.
     */
    void conversionFinished(bool success);

private:
    bool convertModel(const std::set<conversion::ModelType> &sourceModelTypes, conversion::ModelType targetModelType,
            const std::set<conversion::ModelType> &auxilaryModelTypes, conversion::Options options);

    void integrateObserver(const ObserverInfo &info, QStringList &observerNames, QStringList &asn1Files,
            std::map<QString, ProcessMetadata> &allSdlFiles);

    bool convertStopConditions(const std::map<QString, ProcessMetadata> &allSdlFiles);

    bool convertInterfaceview(const QString &inputFilepath, const QString &outputFilepath,
            const QList<QString> &asn1FilepathList, const QStringList &modelFunctions,
            const QStringList &environmentFunctions);
    bool convertMscObservers(const QString &ivFilePath);
    void generateObserverDatamodel(QProcess &process, const QString &sdlFileName);

    std::unique_ptr<ivm::IVModel> readInterfaceView(const QString &filepath);
    void saveOptimizedInterfaceView(const ivm::IVModel *ivModel, const QString outputFilePath);
    void findFunctionsToConvert(const ivm::IVModel &model, QStringList &sdlFunctions,
            std::map<QString, ProcessMetadata> &sdlProcesses, QStringList &envFunctions);
    bool isSdlFunction(const ivm::IVFunction *function);
    void findEnvironmentDatatypes(
            const ivm::IVModel &model, const QStringList &envFunctions, QStringList &envDataTypes);
    bool createEnvGenerationInlines(const QFileInfo &inputDataView, const QString &ivFilepath,
            const QFileInfo &outputFilepath, const QStringList &envDatatypes);

    QFileInfo workDirectory() const;
    QFileInfo simuDataViewLocation() const;
    QFileInfo dataviewUniqLocation() const;
    QFileInfo sdlImplementationBaseDirectory(const QString &functionName) const;
    QFileInfo sdlImplementationLocation(const QString &functionName) const;
    QFileInfo sdlSystemStructureLocation(const QString &functionName) const;
    QFileInfo sdlFunctionDatamodelLocation(const QString &functionName) const;
    QFileInfo sdlFunctionDatamodelLocation(const QString &functionName, const QString &functionTypeName) const;
    QFileInfo sdlFunctionContextLocation(const QString &functionName) const;
    QFileInfo outputFilepath(const QString &name);
    void convertNextFunction();
    void convertNextMscObserver();
    void convertNextObserver();
    void attachNextObserver();
    void generateCDataview();
    bool generateMessageSizes(QFileInfo input, QFileInfo output);

private Q_SLOTS:
    void finishConversion();
    void processStderrReady();
    void processStdoutReady();
    void processStarted();
    void processError(QProcess::ProcessError error);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void timeout();
    void functionConversionFinished(bool success);
    void observerConversionFinished(bool success);
    void stopConditionConversionFinished(bool success);
    void asn1sccProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void asn1sccProcessStarted();

private:
    const QString m_inputIvFilepath;
    const QString m_outputDirectoryFilepath;
    const QDir m_ivBaseDirectory;
    const QDir m_outputDirectory;
    ivm::IVPropertyTemplateConfig *m_dynPropConfig;

    QStringList m_mscObserverFiles;
    std::vector<QString> m_environmentFunctions;
    std::vector<QString> m_keepFunctions;
    std::optional<QString> m_globalInputVectorLengthLimit;
    std::optional<QString> m_processesBasePriority;
    std::optional<QString> m_delta;
    bool m_isRealTypeEnabled;
    bool m_isMulticastEnabled;
    std::unordered_map<QString, QString> m_interfaceInputVectorLengthLimits;
    std::vector<QString> m_subtypesFilepaths;
    QStringList m_stopConditionsFiles;
    std::vector<ObserverInfo> m_observerInfos;
    QStringList m_observerAttachmentInfos;
    QStringList m_observerNames;

    conversion::Registry m_registry;

    QString m_outputOptimizedIvFileName;
    QStringList m_modelFunctions;
    QStringList m_finalEnvironmentFunctions;
    QStringList m_environmentDatatypes;
    std::map<QString, ProcessMetadata> m_allSdlFiles;
    QStringList m_asn1Files;
    size_t m_numberOfProctypes;

    std::list<ProcessMetadata> m_functionsToConvert;
    std::list<QString> m_mscObserversToConvert;
    std::list<ObserverInfo> m_observersToConvert;

    QProcess *m_process;
    QTimer *m_timer;
    SdlToPromelaConverter *m_sdlToPromelaConverter;

    QMetaObject::Connection m_conversionFinishedConnection;

    inline static const QString m_opengeodeCommand = "opengeode";
    constexpr static int m_commandStartTimeout = 12000;
    constexpr static int m_commandTimeout = 12000;
};
}
