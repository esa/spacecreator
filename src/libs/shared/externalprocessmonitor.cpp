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

#include "externalprocessmonitor.h"

#include "externalprocess.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QLayout>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QPushButton>
#include <QTextBrowser>

namespace shared {

ExternalProcrocessMonitor::ExternalProcrocessMonitor(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, Qt::Window)
    , m_display(new QTextBrowser(this))
    , m_process(shared::ExternalProcess::create(this))
{
    connect(m_process.get(), &QProcess::errorOccurred, this, &ExternalProcrocessMonitor::onErrorOccurred);
    connect(m_process.get(), &QProcess::finished, this, &ExternalProcrocessMonitor::onFinished);
    connect(m_process.get(), &QProcess::readyReadStandardError, this,
            &ExternalProcrocessMonitor::onReadyReadStandardError);
    connect(m_process.get(), &QProcess::readyReadStandardOutput, this,
            &ExternalProcrocessMonitor::onReadyReadStandardOutput);
    connect(qApp, &QApplication::aboutToQuit, this, &ExternalProcrocessMonitor::syncStop);

    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(m_display);

    resize(640, 480);
    m_process->setProcessChannelMode(QProcess::MergedChannels);
}

ExternalProcrocessMonitor::~ExternalProcrocessMonitor() { }

void ExternalProcrocessMonitor::setCloseOnSuccessfulFinish(bool closeAtEnd)
{
    m_closeAtEnd = closeAtEnd;
}

void ExternalProcrocessMonitor::showCloseButton(bool showClose)
{
    if (showClose && m_closeButton == nullptr) {
        m_closeButton = new QPushButton(tr("Close"), this);
        connect(m_closeButton, &QPushButton::clicked, this, &ExternalProcrocessMonitor::close);
        m_layout->addWidget(m_closeButton);
        return;
    }

    if (!showClose && m_closeButton != nullptr) {
        delete m_closeButton;
        m_closeButton = nullptr;
        return;
    }
}

bool ExternalProcrocessMonitor::start(const QString &app, const QStringList &args, const QString &workingDir)
{
    if (app.isEmpty()) {
        return false;
    }

    if (!workingDir.isEmpty()) {
        m_process->setWorkingDirectory(workingDir);
    }

    m_display->append(QString("<b>Starting</b> %1 %2\nin %3")
                              .arg(app, args.join(" "), QDir(m_process->workingDirectory()).absolutePath()));
    setWindowTitle(app);
    show();

    m_process->start(app, args);

    return true;
}

/*!
 * Starts the application given in \app and shows the output/errors in a window. Blocks the current process until the
 * window is closed.
 * \param app The programm to start
 * \param args List of parameters to pass to the programm
 * \param workingDir The working directory to start the programm in
 * \param parent
 * \return Returns the exit code of the process
 */
bool ExternalProcrocessMonitor::executeBlocking(
        const QString &app, const QStringList &args, const QString &workingDir, QWidget *parent)
{
    if (app.isEmpty()) {
        return false;
    }

    auto monitor = new ExternalProcrocessMonitor(parent);
    monitor->setCloseOnSuccessfulFinish(false);
    monitor->showCloseButton(true);

    if (!workingDir.isEmpty()) {
        monitor->m_process->setWorkingDirectory(workingDir);
    }

    monitor->m_display->append(
            QString("<b>Starting</b> %1 %2\nin %3")
                    .arg(app, args.join(" "), QDir(monitor->m_process->workingDirectory()).absolutePath()));
    monitor->setWindowTitle(app);
    monitor->m_process->start(app, args);
    monitor->exec();
    monitor->deleteLater();

    if (monitor->m_process->error() == QProcess::FailedToStart) {
        return -2;
    }
    return monitor->m_process->exitStatus() == QProcess::NormalExit ? monitor->m_process->exitCode() : -1;
}

void ExternalProcrocessMonitor::onErrorOccurred(QProcess::ProcessError error)
{
    m_display->append(QString("<b>ERR:</b> %1 - %2").arg(QString::number(error), m_process->errorString()));
}

void ExternalProcrocessMonitor::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (m_closeAtEnd && exitStatus == QProcess::NormalExit && exitCode == 0) {
        close();
        return;
    }

    m_display->append(
            QString("<b>Exit:</b> %1 - %2")
                    .arg(QString::number(exitCode), exitStatus == QProcess::NormalExit ? tr("Normal") : tr("Crash")));
}

void ExternalProcrocessMonitor::onReadyReadStandardError()
{
    m_display->append("ERR: " + m_process->readAllStandardError());
}

void ExternalProcrocessMonitor::onReadyReadStandardOutput()
{
    m_display->append(m_process->readAllStandardOutput());
}

void ExternalProcrocessMonitor::closeEvent(QCloseEvent *event)
{
    if (m_process->state() == QProcess::Running) {
        const QString msg = tr("The process %1 [%2] is still running.");
        const qint64 pid = m_process->processId();
        switch (QMessageBox::question(this, tr("Closing"), msg.arg(m_process->program(), QString::number(pid)),
                tr("Kill"), tr("Cancel closing"))) {
        case 0:
            syncStop();
            break;
        default:
            event->ignore();
            return;
        }
    }

    QDialog::closeEvent(event);
}

void ExternalProcrocessMonitor::syncStop()
{
    if (m_process->state() == QProcess::Running) {
        m_process->kill();
        m_process->waitForFinished();
    }
}

}
