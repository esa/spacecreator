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
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <memory>
#include <tmc/TmcConverter/converter.h>

namespace tmc::verifier {
class TmcVerifier
{
public:
    TmcVerifier(const QString &inputIvFilepath, const QString &outputDirectory);

    bool addStopConditionFiles(const QStringList &files);
    bool execute();

private:
    bool convertSystem();
    bool convertStopConditions();
    bool buildVerifier();
    bool executeVerifier();

    bool convertInterfaceview(const QString &inputFilepath, const QString &outputFilepath);
    bool convertDataview(const QList<QString> &inputFilepathList, const QString &outputFilepath);
    std::unique_ptr<ivm::IVModel> readInterfaceView(const QString &filepath);
    QStringList findIvFunctions(const ivm::IVModel &model);
    bool runSdl2Promela(const QFileInfo &systemStructure, const QFileInfo &functionFile, const QFileInfo &outputFile);
    bool runSdl2PromelaForScl(const QFileInfo &inputFile, const QFileInfo &outputFile);

private:
    std::unique_ptr<::tmc::converter::TmcConverter> m_converter;
    ivm::IVPropertyTemplateConfig *m_dynPropConfig;

    const QString m_inputIvFilepath;
    const QString m_outputDirectory;
    QStringList m_stopConditionsFiles;

    QString m_sdl2PromelaCommand;
    QStringList m_sdl2PromelaArgs;
    int m_externalCommandTimeout;
};
}
