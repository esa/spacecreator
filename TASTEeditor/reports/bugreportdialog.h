#pragma once

#include <QDialog>

namespace Ui {
class BugreportDialog;
}

namespace reports {
class BugReportHandler;
} // namespace reports

class BugreportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BugreportDialog(const QString &logPath, const QList<QPixmap> &images, QWidget *parent = nullptr);
    ~BugreportDialog();

private Q_SLOTS:
    void sendReport();
    void error(const QString &errorString);
    void reportSent(const QString &msg);
    void updateButtonBox();

private:
    Ui::BugreportDialog *m_ui = nullptr;
    reports::BugReportHandler *m_reportHandler = nullptr;
    const QList<QPixmap> m_images;
};
