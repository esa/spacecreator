#include "urlcomposer.h"

#include "issuerequestoptions.h"

#include <QMap>
#include <QUrlQuery>
#include <QVariant>

using namespace gitlab;

UrlComposer::UrlComposer() { }

QUrlQuery UrlComposer::setQuery(const QMap<QByteArray, QVariant> &data)
{
    QUrlQuery query;
    auto keys = data.keys();
    for (const auto &key : keys) {
        if (data[key].typeId() == QMetaType::QStringList) {
            for (const auto &queryItem : data[key].toStringList()) {
                query.addQueryItem(key, queryItem);
            }
        } else {

            if (!data[key].toString().isEmpty()) {
                query.addQueryItem(key, data[key].toString());
            }
        }
    }

    return query;
}

void UrlComposer::setBaseURL(const QUrl &newBaseURL)
{
    mBaseURL = newBaseURL;
}

/*!
 * \brief ComposeGetIssuesUrl creates the full url to fetch issues from the server @see setBaseUrl
 * \param projectID The ID of the project. \see ComposeProjectUrl to get the ID from the project name
 * \param options All options/parameters for fetching the issues for the
 */
QUrl UrlComposer::composeGetIssuesUrl(const int &projectID, const IssueRequestOptions &options) const
{
    QString address = composeUrl(UrlComposer::UrlTypes::GetIssues);
    address = address.arg(QString::number(projectID));

    QUrl url(address);
    url.setQuery(options.urlQuery());
    return url;
}

QUrl UrlComposer::composeCreateIssueUrl(const int &projectID, const QString &title, const QString &description,
        const QStringList &labels, const QString &assignee) const
{
    QString address = composeUrl(UrlComposer::UrlTypes::CreateIssue);
    address = address.arg(QString::number(projectID));

    QMap<QByteArray, QVariant> data = { { "id", QString::number(projectID) }, { "title", title },
        { "description", description }, { "assignee_id", assignee } };
    if (!labels.isEmpty()) {
        data.insert("labels", labels.join(","));
    }

    QUrl url(address);
    url.setQuery(setQuery(data));
    return url;
}

QUrl UrlComposer::composeEditIssueUrl(const int &projectID, const int &issueID, const QString &title,
        const QString &description, const QString &assignee, const QString &state_event,
        const QStringList &labels) const
{
    QString address = composeUrl(UrlComposer::UrlTypes::EditIssue);
    address = address.arg(QString::number(projectID), QString::number(issueID));

    QMap<QByteArray, QVariant> data = { { "id", QString::number(projectID) },
        { "issue_iid", QString::number(issueID) } };

    if (!title.isEmpty()) {
        data.insert("title", title);
    }
    if (!description.isEmpty()) {
        data.insert("description", description);
    }
    if (!assignee.isEmpty()) {
        data.insert("assignee_ids", assignee);
    }
    if (!state_event.isEmpty()) {
        data.insert("state_event", state_event);
    }
    if (!labels.isEmpty()) {
        data.insert("labels", labels);
    }

    QUrl url(address);
    url.setQuery(setQuery(data));
    return url;
}

QUrl UrlComposer::composeProjectLabelsUrl(const int &projectID, const QString &with_counts, const QString &search) const
{
    QString address = composeUrl(UrlComposer::UrlTypes::ProjectLabels);
    address = address.arg(QString::number(projectID));

    const QMap<QByteArray, QVariant> data = {
        { "id", QString::number(projectID) }, { "with_counts", with_counts }, { "search", search } // Search label terms
    };

    QUrl url(address);
    url.setQuery(setQuery(data));
    return url;
}

QUrl UrlComposer::composeProjectUrl(const QString &projectName) const
{
    QString address = composeUrl(UrlComposer::UrlTypes::Projects);

    const QMap<QByteArray, QVariant> data = {
        { "search", projectName } // Search project name
    };

    QUrl url(address);
    url.setQuery(setQuery(data));
    return url;
}

QString UrlComposer::composeUrl(UrlTypes target) const
{
    QString address(mBaseURL.toString());
    switch (target) {
    case UrlTypes::GetIssues:
    case UrlComposer::UrlTypes::CreateIssue: {
        address += "/projects/%1/issues";
        break;
    }
    case UrlComposer::UrlTypes::EditIssue: {
        address += "/projects/%1/issues/%2";
        break;
    }
    case UrlComposer::UrlTypes::Projects: {
        address += "/projects";
        break;
    }
    case UrlComposer::UrlTypes::ProjectLabels: {
        address += "/projects/%1/labels";
        break;
    }
    }
    return address;
}
