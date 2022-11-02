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
#include <tmc/TmcVerifier/verifier.h>

namespace tmc {
class TmcExecutor final : public QObject
{
    Q_OBJECT
public:
    TmcExecutor(QObject *parent = nullptr);
    ~TmcExecutor();

public Q_SLOTS:
    void execute();
    void verifierMessage(QString text);
    void finished(bool success);

private:
    tmc::verifier::TmcVerifier *m_verifier;
};
}
