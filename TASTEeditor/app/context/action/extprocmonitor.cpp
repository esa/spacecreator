#include "extprocmonitor.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QLayout>
#include <QMessageBox>

namespace taste3 {
namespace ctx {

ExtProcMonitor::ExtProcMonitor(QWidget *parent)
    : QWidget(parent, Qt::Dialog)
    , m_display(new QTextBrowser(this))
    , m_process(new QProcess(this))
{
    connect(m_process, &QProcess::errorOccurred, this, &ExtProcMonitor::onErrorOccurred);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &ExtProcMonitor::onFinished);
    connect(m_process, &QProcess::readyReadStandardError, this, &ExtProcMonitor::onReadyReadStandardError);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &ExtProcMonitor::onReadyReadStandardOutput);
    connect(qApp, &QApplication::aboutToQuit, this, &ExtProcMonitor::syncStop);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_display);

    resize(640, 480);
    m_process->setProcessChannelMode(QProcess::MergedChannels);
}

bool ExtProcMonitor::start(const QString &app, const QStringList &args, const QString &workingDir)
{
    if (app.isEmpty())
        return false;

    m_process->setWorkingDirectory(workingDir);

    m_display->append(
            QString("<b>Starting</b> %1 %2\nin %3").arg(app, args.join(" "), QFileInfo(workingDir).absolutePath()));
    setWindowTitle(app);
    show();

    m_process->start(app, args);

    return true;
}

void ExtProcMonitor::onErrorOccurred(QProcess::ProcessError error)
{
    m_display->append(QString("<b>ERR:</b> %1 - %2").arg(QString::number(error), m_process->errorString()));
}

void ExtProcMonitor::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_display->append(
            QString("<b>Exit:</b> %1 - %2")
                    .arg(QString::number(exitCode), exitStatus == QProcess::NormalExit ? tr("Normal") : tr("Crash")));
}

void ExtProcMonitor::onReadyReadStandardError()
{
    m_display->append("ERR: " + m_process->readAllStandardError());
}

void ExtProcMonitor::onReadyReadStandardOutput()
{
    m_display->append(m_process->readAllStandardOutput());
}

void ExtProcMonitor::closeEvent(QCloseEvent *event)
{
    if (m_process->state() == QProcess::Running) {
        const QString msg("The process %1 [%2] is still running.");
        const qint64 pid =
#ifdef Q_OS_WIN
                m_process->processId();
#else
                m_process->pid();
#endif
        switch (QMessageBox::question(this, tr("Closing"), msg.arg(m_process->program(), QString::number(pid)),
                                      tr("Kill"), tr("Cancel"))) {
        case 0: {
            syncStop();
            break;
        }
        default: {
            event->ignore();
            return;
        }
        }
    }

    event->accept();
}

void ExtProcMonitor::syncStop()
{
    if (m_process->state() == QProcess::Running) {
        m_process->kill();
        m_process->waitForFinished();
    }
}

} // ns ctx
} // ns taste3
