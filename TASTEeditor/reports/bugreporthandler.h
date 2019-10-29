/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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
