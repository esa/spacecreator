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

QUrl UrlComposer::composeGetIssuesUrl(const QString &projectID, const IssueRequestOptions &options) const
{
    QString address = composeUrl(UrlComposer::UrlTypes::GetIssues);
    address = address.arg(projectID);

    QUrl url(address);
    url.setQuery(options.urlQuery());
    return url;
}

QUrl UrlComposer::composeCreateIssueUrl(
        const QString &projectID, const QString &title, const QString &description, const QString &assignee) const
{
    QString address = composeUrl(UrlComposer::UrlTypes::CreateIssue);
    address = address.arg(projectID);

    const QMap<QByteArray, QVariant> data = { { "id", projectID }, { "title", title }, { "description", description },
        { "assignee_id", assignee }, { "labels", { "requirement" } } };

    QUrl url(address);
    url.setQuery(setQuery(data));
    return url;
}

QUrl UrlComposer::composeEditIssueUrl(int projectID, int issueID, const QString &title, const QString &description,
        const QString &assignee, const QString &state_event, const QStringList &labels) const
{
    QString address = composeUrl(UrlComposer::UrlTypes::EditIssue);
    address = address.arg(projectID, issueID);

    const QMap<QByteArray, QVariant> data = {
        { "id", projectID },
        { "issue_iid", issueID },
        { "title", title },
        { "description", description },
        { "assignee_ids", assignee },
        { "state_event", state_event }, // Only allows "reopen" and "close" states
        { "labels", labels },
    };

    QUrl url(address);
    url.setQuery(setQuery(data));
    return url;
}

QUrl UrlComposer::composeProjectLabelsUrl(
        const QString &projectID, const QString &with_counts, const QString &search) const
{
    QString address = composeUrl(UrlComposer::UrlTypes::ProjectLabels);
    address = address.arg(projectID);

    const QMap<QByteArray, QVariant> data = {
        { "id", projectID }, { "with_counts", with_counts }, { "search", search } // Search label terms
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
