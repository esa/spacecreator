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

namespace tmc::converter {
/**
 * @brief Main class used to convert TASTE project to format ready for formal model verification.
 */
class TmcConverter
{
public:
    /**
     * @brief Constructor.
     *
     * @param inputIvFilepath Path to XML interface view.
     * @param outputDirectory Pat to output directory for conversion results.
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
     * @brief Add Stop Condition files to convert.
     *
     * This shall be called before @link{convert}
     *
     * @param files A list of paths with Stop Condition files.
     * @return true if files exist, otherwise false.
     */
    bool addStopConditionFiles(const QStringList &files);

    /**
     * @brief Attach input Observer specification
     *
     * It shall have the format FunctionName:InterfaceName:ObserverName.
     * The entire specification is passed as a single string for simplicity.
     *
     * @param attachmentSpecification Input Observer Attachment Specification
     * @return true if the operation succeeded, false otherwise.
     */
    bool attachInputObserver(const QString &attachmentSpecification);

private:
    bool convertModel(const std::set<conversion::ModelType> &sourceModelTypes, conversion::ModelType targetModelType,
            const std::set<conversion::ModelType> &auxilaryModelTypes, conversion::Options options) const;

    bool convertSystem(std::map<QString, ProcessMetadata> &allSdlFiles);

    bool convertStopConditions(const std::map<QString, ProcessMetadata> &allSdlFiles);

    bool convertInterfaceview(const QString &inputFilepath, const QString &outputFilepath,
            const QStringList &modelFunctions, const QStringList &environmentFunctions);
    bool convertDataview(const QList<QString> &inputFilepathList, const QString &outputFilepath);
    std::unique_ptr<ivm::IVModel> readInterfaceView(const QString &filepath);
    void findFunctionsToConvert(const ivm::IVModel &model, QStringList &sdlFunctions,
            std::map<QString, ProcessMetadata> &sdlProcesses, QStringList &envFunctions);
    bool isSdlFunction(const ivm::IVFunction *function);
    void findEnvironmentDatatypes(
            const ivm::IVModel &model, const QStringList &envFunctions, QStringList &envDataTypes);
    bool createEnvGenerationInlines(
            const QFileInfo &inputDataView, const QFileInfo &outputFilepath, const QStringList &envDatatypes);

    QFileInfo workDirectory() const;
    QFileInfo dataViewUniqLocation() const;
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

    QStringList m_stopConditionsFiles;
    QStringList m_inputObserverAttachments;

    conversion::Registry m_registry;
};
}
