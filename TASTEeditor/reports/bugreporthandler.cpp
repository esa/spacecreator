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

#include "bugreporthandler.h"

#include <QBuffer>
#include <QFutureWatcher>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeDatabase>
#include <QMimeType>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QtConcurrentRun>

namespace reports {

BugReportHandler::BugReportHandler(const QString &host, const QString &projectID, const QByteArray &accessToken,
                                   QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_host(host)
    , m_project(projectID)
    , m_token(accessToken)
{
}

BugReportHandler::~BugReportHandler()
{
    if (m_reply)
        m_reply->abort();

    clear();
}

void BugReportHandler::uploadAttachment(const QByteArray &content, const QMimeType &mimeType)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart httpPart;
    httpPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType.name()));
    httpPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant(QStringLiteral("form-data; name=\"file\"; filename=\"attachment.%1\"")
                                        .arg(mimeType.preferredSuffix())));
    httpPart.setBody(content);
    multiPart->append(httpPart);

    QUrl url(m_host);
    url.setPath(QStringLiteral("/api/v4/projects/%1/uploads").arg(m_project));
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("PRIVATE-TOKEN"), m_token);

    m_reply = m_networkManager->post(request, multiPart);
    multiPart->setParent(m_reply);
    connect(m_reply, &QNetworkReply::finished, this, &BugReportHandler::attachmentUploaded);
}

void BugReportHandler::createIssue()
{
    QJsonObject issueObj;
    issueObj.insert(QLatin1String("id"), m_project);
    issueObj.insert(QLatin1String("title"), m_report->title);
    issueObj.insert(QLatin1String("description"),
                    m_report->description + QLatin1String("\n\n") + m_attachmentsLinks.join(QLatin1String("\n\n")));
    issueObj.insert(QLatin1String("labels"), QLatin1String("BugReport"));
    const QByteArray issueData = QJsonDocument(issueObj).toJson();

    QUrl url(m_host);
    url.setPath(QStringLiteral("/api/v4/projects/%1/issues").arg(m_project));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QLatin1String("application/json")));
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(issueData.size()));
    request.setRawHeader(QByteArray("PRIVATE-TOKEN"), m_token);

    m_reply = m_networkManager->post(request, issueData);
    connect(m_reply, &QNetworkReply::finished, this, &BugReportHandler::issueCreated);
}

void BugReportHandler::attachmentUploaded()
{
    if (QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender())) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonParseError jsonError;
            const QByteArray rawData = reply->readAll();
            const QJsonObject data = QJsonDocument::fromJson(rawData, &jsonError).object();
            if (jsonError.error == QJsonParseError::NoError) {
                m_attachmentsLinks.append(data.value(QLatin1String("markdown")).toString());
                if (!m_attachments.isEmpty()) {
                    const auto attachment = m_attachments.dequeue();
                    uploadAttachment(attachment.first, attachment.second);
                } else {
                    createIssue();
                }
            } else {
                const QString errorString = jsonError.errorString();
                qWarning() << errorString;
                Q_EMIT error(errorString);
            }
        } else {
            const QString errorString = reply->errorString();
            qWarning() << errorString;
            Q_EMIT error(errorString);
        }

        reply->deleteLater();
    }
}

void BugReportHandler::issueCreated()
{
    if (QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender())) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonParseError jsonError;
            const QByteArray rawData = reply->readAll();
            const QJsonObject data = QJsonDocument::fromJson(rawData, &jsonError).object();
            if (jsonError.error != QJsonParseError::NoError) {
                const QString errorString = jsonError.errorString();
                qWarning() << errorString;
                Q_EMIT error(errorString);
            } else if (data.isEmpty()) {
                const QString errorString = tr("Empty reply data");
                qWarning() << errorString;
                Q_EMIT error(errorString);
            } else {
                Q_EMIT reportSent(data.value(QLatin1String("web_url")).toString());
            }
        } else {
            const QString errorString = reply->errorString();
            qWarning() << errorString;
            Q_EMIT error(errorString);
        }
        reply->deleteLater();
    }
}

void BugReportHandler::clear()
{
    m_report.clear();
    m_attachments.clear();
    m_attachmentsLinks.clear();
}

void BugReportHandler::sendReport(QSharedPointer<BugReport> report)
{
    if (m_report || !report)
        return;

    clear();
    m_report = report;

    typedef QQueue<QPair<QByteArray, QMimeType>> AttachmentQueue;
    auto futureWatcher = new QFutureWatcher<AttachmentQueue>();
    connect(futureWatcher, &QFutureWatcher<AttachmentQueue>::finished, this, [this, futureWatcher]() {
        m_attachments = futureWatcher->result();
        futureWatcher->deleteLater();

        QMimeDatabase db;
        uploadAttachment(m_report->log, db.mimeTypeForData(m_report->log));
    });

    QFuture<AttachmentQueue> future = QtConcurrent::run(
            [](const QList<QPixmap> &pixmaps) {
                AttachmentQueue result;
                QMimeDatabase db;
                const QMimeType mimeType = db.mimeTypeForName(QLatin1String("image/png"));
                for (const auto &pix : pixmaps) {
                    if (pix.isNull())
                        continue;

                    QBuffer buf;
                    if (!buf.open(QIODevice::WriteOnly)) {
                        qWarning() << buf.errorString();
                        continue;
                    }

                    if (!pix.save(&buf, mimeType.preferredSuffix().toLatin1().data())) {
                        qWarning() << "Error during pixmap saving:" << pix << mimeType;
                        continue;
                    }
                    buf.close();
                    result.enqueue(qMakePair(buf.buffer(), mimeType));
                }
                return result;
            },
            m_report->images);
    futureWatcher->setFuture(future);
}

} // namespace reports
