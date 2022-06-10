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

#include <QStringList>
#include <tmc/TmcConverter/converter.h>

namespace tmc::verifier {
/**
 * @brief Main class uses to process formal model verification on TASTE project.
 *
 */
class TmcVerifier final
{
public:
    /**
     * @brief Constructor.
     *
     * @param   inputIvFilepath         Path to XML interface view.
     * @param   outputDirectory         Path to output directory.
     */
    TmcVerifier(const QString &inputIvFilepath, const QString &outputDirectory);

    /**
     * @brief   Specify which IV functions should be treated as an environment
     *          during model checking
     *
     * @param   environmentFunctions    Functions to treat as an evironment
     */
    void addEnvironmentFunctions(const std::vector<QString> &environmentFunctions);
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

    /**
     * @brief Prepare the system and process formal model verification.
     *
     * @return true if whole process succed, otherwise false
     */
    bool execute();

private:
    bool buildVerifier();
    bool executeVerifier();

private:
    const QString m_inputIvFilepath;
    const QString m_outputDirectory;
    std::unique_ptr<::tmc::converter::TmcConverter> m_converter;
};
}
