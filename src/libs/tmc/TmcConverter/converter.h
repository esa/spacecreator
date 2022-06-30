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

#include "processmetadata.h"

#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QStringList>
#include <conversion/common/modeltype.h>
#include <conversion/registry/registry.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <map>
#include <memory>
#include <tmc/SdlToPromelaConverter/processmetadata.h>
#include <unordered_map>

namespace tmc::converter {
/**
 * @brief Main class used to convert TASTE project to format ready for formal model verification.
 */
class TmcConverter
{
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
    TmcConverter(const QString &inputIvFilepath, const QString &outputDirectory);

    /**
     * @brief Process system conversion.
     *
     * This method converts interface view, data view and SDL into promela files.
     *
     * @return true if conversion succeed, otherwise false.
     */
    bool convert();
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
     * @brief   Set path to the ASN.1 containing subtypes
     *
     * @param   filepath    Path to the file
     */
    void setSubtypesFilepath(std::optional<QString> filepath);
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
     * @brief Attach an Observer
     *
     * @param observerPath Path to the observer process file
     * @param priority Observer priority
     * @return true if the operation succeeded, false otherwise.
     */
    auto attachObserver(const QString &observerPath, const uint32_t priority) -> bool;

private:
    bool convertModel(const std::set<conversion::ModelType> &sourceModelTypes, conversion::ModelType targetModelType,
            const std::set<conversion::ModelType> &auxilaryModelTypes, conversion::Options options) const;

    auto integrateObserver(const ObserverInfo &info, QStringList &observerNames, QStringList &asn1Files,
            std::map<QString, ProcessMetadata> &allSdlFiles, QStringList &attachmentInfos);
    bool convertSystem(std::map<QString, ProcessMetadata> &allSdlFiles);

    bool convertStopConditions(const std::map<QString, ProcessMetadata> &allSdlFiles);

    bool convertInterfaceview(const QString &inputFilepath, const QString &outputFilepath,
            const QStringList &modelFunctions, const QStringList &environmentFunctions);
    bool convertDataview(const QList<QString> &inputFilepathList, const QString &outputFilepath);
    std::unique_ptr<ivm::IVModel> readInterfaceView(const QString &filepath);
    void saveOptimizedInterfaceView(const ivm::IVModel *ivModel, const QString outputFilePath);
    void findFunctionsToConvert(const ivm::IVModel &model, QStringList &sdlFunctions,
            std::map<QString, ProcessMetadata> &sdlProcesses, QStringList &envFunctions);
    bool isSdlFunction(const ivm::IVFunction *function);
    void findEnvironmentDatatypes(
            const ivm::IVModel &model, const QStringList &envFunctions, QStringList &envDataTypes);
    bool createEnvGenerationInlines(
            const QFileInfo &inputDataView, const QFileInfo &outputFilepath, const QStringList &envDatatypes);

    QFileInfo workDirectory() const;
    QFileInfo simuDataViewLocation() const;
    QFileInfo sdlImplementationBaseDirectory(const QString &functionName) const;
    QFileInfo sdlImplementationLocation(const QString &functionName) const;
    QFileInfo sdlSystemStructureLocation(const QString &functionName) const;
    QFileInfo sdlFunctionDatamodelLocation(const QString &functionName) const;
    QFileInfo sdlFunctionDatamodelLocation(const QString &functionName, const QString &functionTypeName) const;
    QFileInfo sdlFunctionContextLocation(const QString &functionName) const;
    QFileInfo outputFilepath(const QString &name);

private:
    const QString m_inputIvFilepath;
    const QString m_outputDirectoryFilepath;
    const QDir m_ivBaseDirectory;
    const QDir m_outputDirectory;
    ivm::IVPropertyTemplateConfig *m_dynPropConfig;

    std::vector<QString> m_environmentFunctions;
    std::vector<QString> m_keepFunctions;
    std::optional<QString> m_globalInputVectorLengthLimit;
    std::unordered_map<QString, QString> m_interfaceInputVectorLengthLimits;
    std::optional<QString> m_subtypesFilepath;
    QStringList m_stopConditionsFiles;
    std::vector<ObserverInfo> m_observerInfos;
    QStringList m_observerAttachmentInfos;
    QStringList m_observerNames;

    conversion::Registry m_registry;
};
}
