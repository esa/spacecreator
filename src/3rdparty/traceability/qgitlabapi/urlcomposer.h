#pragma once

#include <QUrl>

namespace gitlab {

class IssueRequestOptions;

/**
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

    static QUrlQuery setQuery(const QMap<QByteArray, QVariant> &data);

    /**
     * @brief ComposeGetIssuesUrl creates the full url to fetch issues from the server @see setBaseUrl
     * @param projectID The ID of the project. \see ComposeProjectUrl to get the ID from the project name
     * @param options All options/parameters for fetching the issues for the
     */
    QUrl composeGetIssuesUrl(const QString &projectID, const IssueRequestOptions &options) const;
    QUrl composeCreateIssueUrl(
            const QString &projectID, const QString &title, const QString &description, const QString &assignee) const;
    QUrl composeEditIssueUrl(int projectID, int issueID, const QString &title = QString(),
            const QString &description = QString(), const QString &assignee = QString(),
            const QString &state_event = QString(), const QStringList &labels = QStringList()) const;

    QUrl composeProjectLabelsUrl(
            const QString &projectID, const QString &with_counts = "false", const QString &search = QString()) const;
    QUrl composeProjectUrl(const QString &projectName) const;

    void setBaseURL(const QUrl &newBaseURL);

protected:
    QString composeUrl(UrlComposer::UrlTypes target) const;

private:
    QUrl mBaseURL;
};

}
