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

#pragma once

#include "spinconfigsaver.h"

#include <QString>
#include <QThread>
#include <memory>

namespace tmc::verifier {
class TmcVerifier;
}

namespace ive {
class SpinRunWorker : public QThread
{
    Q_OBJECT
public:
    SpinRunWorker(QObject *parent, const QStringList &properties, const QStringList &subtypes,
            const QStringList &functions, const SpinConfigData &config, const QString &projectRoot,
            const QString &outputDirectory);
    ~SpinRunWorker();

    void setSubtypesPath(const QString &subtypesPath);
    void setPropertiesPath(const QString &propertiesPath);

    void stop();

Q_SIGNALS:
    void textAvailable(QString text);
    void jobFinished(bool success);

protected:
    void run() override;

private:
    QStringList m_propertiesSelected;
    QStringList m_subtypesSelected;
    QStringList m_functionsSelected;
    SpinConfigData m_spinConfig;
    QString m_projectRoot;
    QString m_outputDirectory;
    QString m_propertiesPath;
    QString m_subtypesPath;

    std::unique_ptr<tmc::verifier::TmcVerifier> m_verifier;
};
}
