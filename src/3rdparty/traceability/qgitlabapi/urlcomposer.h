#pragma once

#include <QUrl>

namespace gitlab {

class IssueRequestOptions;

/*!
 * Class to create URLs to fetch data from gitlab
 */
class UrlComposer
{
public:
    enum UrlTypes
    {
        GetIssues,
        CreateIssue,
        EditIssue,
        Projects,
        ProjectLabels,
    };

    UrlComposer();

    static QUrlQuery setQuery(const QMap<QString, QVariant> &data);

    QUrl composeGetIssuesUrl(const int &projectID, const IssueRequestOptions &options) const;
    QUrl composeCreateIssueUrl(const int &projectID, const QString &title, const QString &description,
            const QStringList &labels, const QString &assignee) const;
    QUrl composeEditIssueUrl(const int &projectID, const int &issueID, const QString &title = QString(),
            const QString &description = QString(), const QString &assignee = QString(),
            const QString &state_event = QString(), const QStringList &labels = QStringList()) const;

    QUrl composeProjectLabelsUrl(
            const int &projectID, const QString &with_counts = "false", const QString &search = QString()) const;
    QUrl composeProjectUrl(const QString &projectName) const;

    void setBaseURL(const QUrl &newBaseURL);

protected:
    QString composeUrl(UrlComposer::UrlTypes target) const;

private:
    QUrl mBaseURL;
};

}
