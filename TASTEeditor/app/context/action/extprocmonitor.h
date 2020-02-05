/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include <QProcess>
#include <QTextBrowser>
#include <QWidget>

namespace taste3 {
namespace ctx {

class ExtProcMonitor : public QWidget
{
    Q_OBJECT
public:
    explicit ExtProcMonitor(QWidget *parent = nullptr);
    bool start(const QString &app, const QStringList &args, const QString &workingDir);

Q_SIGNALS:

private Q_SLOTS:
    void onErrorOccurred(QProcess::ProcessError error);
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onReadyReadStandardError();
    void onReadyReadStandardOutput();

    void syncStop();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QTextBrowser *m_display;
    QProcess *m_process;
};

} // ns ctx
} // ns taste3
