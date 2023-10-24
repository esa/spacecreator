#include "UrlComposer.h"
#include "qvariant.h"

#include <QMap>
#include <QUrlQuery>

UrlComposer::UrlComposer()
{}

QString UrlComposer::ComposeUrl(UrlTypes target) const
{
    QString address(mBaseURL.toString());
    switch (target) {
    case UrlTypes::GetIssues: {
        address += "/issues";
        break;
    }
    case UrlComposer::UrlTypes::CreateIssue: {
        address += "/projects/%1/issues";
        break;
    }
    case UrlComposer::UrlTypes::EditIssue: {
        address += "/projects/%1/issues/%2";
        break;
    }
    case ProjectLabels: {
        address += "/projects/%1/labels";
        break;
    }
    }
    return address;
}

QUrlQuery UrlComposer::SetQuery(const QMap<QString, QVariant> data) const
{
    QUrlQuery query;
    auto keys = data.keys();
    for (const auto &key : keys)
        if (data[key].typeId() == QMetaType::QStringList)
        {
            for (const auto& queryItem : data[key].toStringList())
            {
               query.addQueryItem(key, queryItem);
            }
        }
        else
        {
            if (!data[key].toString().isEmpty())
                query.addQueryItem(key, data[key].toString());
        }



    return query;
}

void UrlComposer::setBaseURL(const QUrl &newBaseURL)
{
    mBaseURL = newBaseURL;
}

QUrl UrlComposer::ComposeGetIssuesUrl(const QString &assignee,
                                      const QString &author,
                                      const QStringList &iids,
                                      const QString &scope) const
{
    QUrl url(ComposeUrl(UrlComposer::UrlTypes::GetIssues));

    const QMap<QString, QVariant> data = {
        {"scope", scope},
        {"iids[]", iids},
        {"assignee_username", assignee},
        {"author_username", author},
    };

    url.setQuery(SetQuery(data));
    return url;
}

QUrl UrlComposer::ComposeCreateIssueUrl(const QString &projectID,
                                        const QString &title,
                                        const QString &description,
                                        const QString &assignee) const
{
    QString address = ComposeUrl(UrlComposer::UrlTypes::CreateIssue);
    address = address.arg(projectID);

    const QMap<QString, QVariant> data = {{"id", projectID},
                                         {"title", title},
                                         {"description", description},
                                         {"assignee_id", assignee}};

    QUrl url(address);
    url.setQuery(SetQuery(data));
    return url;
}

QUrl UrlComposer::ComposeEditIssueUrl(const QString &projectID,
                                      const QString &issueID,
                                      const QString &title,
                                      const QString &description,
                                      const QString &assignee,
                                      const QString &state_event,
                                      const QString &labels) const
{
    QString address = ComposeUrl(UrlComposer::UrlTypes::EditIssue);
    address = address.arg(projectID, issueID);

    const QMap<QString, QVariant> data = {{"id", projectID},
                                         {"issue_iid", issueID},
                                         {"title", title},
                                         {"description", description},
                                         {"assignee_ids", assignee},
                                         {"state_event", state_event}, // Only allows "reopen" and "close" states
                                         {"labels", labels}};

    QUrl url(address);
    url.setQuery(SetQuery(data));
    return url;
}

QUrl UrlComposer::ComposeProjectLabelsUrl(const QString &projectID,
                                          const QString &with_counts,
                                          const QString &search) const
{
    QString address = ComposeUrl(UrlComposer::UrlTypes::ProjectLabels);
    address = address.arg(projectID);

    const QMap<QString, QVariant> data = {
        {"id", projectID},
        {"with_counts", with_counts},
        {"search", search} // Search label terms
    };

    QUrl url(address);
    url.setQuery(SetQuery(data));
    return url;
}
