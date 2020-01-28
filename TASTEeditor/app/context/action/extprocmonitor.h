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
