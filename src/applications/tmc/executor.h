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
#include <memory>
#include <tmc/TmcVerifier/verifier.h>

namespace tmc {
/**
 * @brief TmcExecutor creates TmcVerifier and starts conversion
 * from TASTE project to Promela files.
 * After conversion finishes it quits QCoreApplication.
 * During the conversion the messages are reported on stdout.
 */
class TmcExecutor final : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     *
     * @param parent parent of the object.
     */
    TmcExecutor(QObject *parent = nullptr);

public Q_SLOTS:
    /**
     * @brief Start the execution of verification.
     */
    void execute();

private Q_SLOTS:
    void verifierMessage(QString text);
    void finished(bool success);

private:
    std::unique_ptr<tmc::verifier::TmcVerifier> m_verifier;
};
}
