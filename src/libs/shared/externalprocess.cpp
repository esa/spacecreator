/*
  Copyright (C) 2024 European Space Agency - <maxime.perrotin@esa.int>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public License
along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "externalprocess.h"

#include <QDir>
#include <QProcess>
#include <QProcessEnvironment>

namespace shared {

std::unique_ptr<QProcess> ExternalProcess::create(QObject *parent)
{
    auto process = std::make_unique<QProcess>(parent);

    static QProcessEnvironment env;
    if (env.isEmpty()) {
        env = QProcessEnvironment::systemEnvironment();

        auto replaceToOriginal = [](const QString &key, QProcessEnvironment &env) {
            const QString userKey = QString("_ORIGINAL_%1").arg(key);
            if (env.keys().contains(userKey)) {
                const QString &content = env.value(userKey);
                if (content.isEmpty()) {
                    env.remove(key);
                } else {
                    env.insert(key, content);
                }
            }
        };

        // The vars preceded with _ORIGINAL_ are set by the "…install/appimage/AppRun" script
        replaceToOriginal("LD_LIBRARY_PATH", env);
        replaceToOriginal("XDG_DATA_HOME", env);
        replaceToOriginal("GSETTINGS_SCHEMA_DIR", env);
    }

    process->setProcessEnvironment(env);

    return process;
}

int ExternalProcess::executeBlocking(const QString &program, const QStringList &arguments)
{
    std::unique_ptr<QProcess> process = create();
    // Implementation from QProcess::execute
    process->setProcessChannelMode(QProcess::ForwardedChannels);
    process->start(program, arguments);
    if (!process->waitForFinished(-1) || process->error() == QProcess::FailedToStart) {
        return -2;
    }
    return process->exitStatus() == QProcess::NormalExit ? process->exitCode() : -1;
}

} // namespace shared
