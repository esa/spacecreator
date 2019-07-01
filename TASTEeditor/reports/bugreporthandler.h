#pragma once

#include <QMimeType>
#include <QObject>
#include <QPointer>
#include <QQueue>
#include <QSharedPointer>

class QNetworkAccessManager;
class QNetworkReply;

namespace reports {
struct BugReport {
    QString title;
    QString description;
    QByteArray log;
    QList<QPixmap> images;
};

class BugReportHandler : public QObject
{
    Q_OBJECT
public:
    explicit BugReportHandler(const QString &host, const QString &projectID, const QByteArray &accessToken,
                              QObject *parent = nullptr);
    ~BugReportHandler() override;
    void sendReport(QSharedPointer<BugReport> report);

Q_SIGNALS:
    void error(const QString &errorText);
    void reportSent(const QString &text);

private Q_SLOTS:
    void attachmentUploaded();
    void issueCreated();

private:
    void uploadAttachment(const QByteArray &content, const QMimeType &mimeType);
    void createIssue();

    void clear();

private:
    QNetworkAccessManager *m_networkManager = nullptr;
    const QString m_host;
    const QString m_project;
    const QByteArray m_token;

    QPointer<QNetworkReply> m_reply;

    QSharedPointer<BugReport> m_report;
    QStringList m_attachmentsLinks;
    QQueue<QPair<QByteArray, QMimeType>> m_attachments;
};

} // namespace reports
