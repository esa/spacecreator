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
#include <QStringList>
#include <conversion/common/modeltype.h>
#include <conversion/registry/registry.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <memory>

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

private:
    bool convertModel(std::set<conversion::ModelType> sourceModelTypes, conversion::ModelType targetModelType,
            const std::set<conversion::ModelType> auxilaryModelTypes, conversion::Options options) const;

    bool convertSystem();

    bool convertStopConditions();

    bool convertInterfaceview(const QString &inputFilepath, const QString &outputFilepath);
    bool convertDataview(const QList<QString> &inputFilepathList, const QString &outputFilepath);
    std::unique_ptr<ivm::IVModel> readInterfaceView(const QString &filepath);
    void findIvFunctions(const ivm::IVModel &model, QStringList &sdlFunctions, QStringList &envFunctions);
    bool isSdlFunction(const ivm::IVFunction *function);
    void findEnvDatatypes(const ivm::IVModel &model, const QStringList &envFunctions, QStringList &envDataTypes);
    bool createEnvGenerationInlines(
            const QFileInfo &inputDataView, const QFileInfo &outputFilepath, const QStringList &envDatatypes);

    QFileInfo workDirectory() const;
    QFileInfo dataViewUniqLocation() const;
    QFileInfo sdlImplementationBaseDirectory(const QString &functionName) const;
    QFileInfo sdlImplementationLocation(const QString &functionName) const;
    QFileInfo sdlSystemStructureLocation(const QString &functionName) const;
    QFileInfo sdlFunctionDatamodelLocation(const QString &functionName) const;
    QFileInfo outputFilepath(const QString &name);

private:
    const QString m_inputIvFilepath;
    const QString m_outputDirectoryFilepath;
    const QDir m_ivBaseDirectory;
    const QDir m_outputDirectory;
    ivm::IVPropertyTemplateConfig *m_dynPropConfig;

    QStringList m_stopConditionsFiles;

    conversion::Registry m_registry;
};
}
