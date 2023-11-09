#ifndef URLCOMPOSER_H
#define URLCOMPOSER_H

#include <QUrl>

/**
 * Class to create URLs to fetch data from gitlab
 */
class UrlComposer
{
public:
    enum UrlTypes {
        GetIssues,
        CreateIssue,
        EditIssue,
        Projects,
        ProjectLabels,
    };

    UrlComposer();

    /**
     * @brief ComposeGetIssuesUrl creates the full url to fetch issues from the server @see setBaseUrl
     * @param projectID The ID of the project. \see ComposeProjectUrl to get the ID from the project name
     * @param assignee If not empty, only issues assigned to that assignee (username) are fetched
     * @param author If not empty, only issues assigned to that assignee with that username are fetched
     * @param iids If not empty, return only the issues having the given iid
     * @param scope Return issues for the given scope: "created_by_me", "assigned_to_me" or "all".
     * @param state Return "all" issues or just those that are "opened" or "closed"
     * @param page Numer of the page to fetch. See pagination
     */
    QUrl ComposeGetIssuesUrl(const QString &projectID, const QString &assignee, const QString &author,
            const QStringList &iids, const QString &scope = "all", const QString &state = "all", int page = 1) const;
    QUrl ComposeCreateIssueUrl(const QString &projectID,
                               const QString &title,
                               const QString &description,
                               const QString &assignee) const;
    QUrl ComposeEditIssueUrl(int projectID, int issueID, const QString &title = QString(),
            const QString &description = QString(), const QString &assignee = QString(),
            const QString &state_event = QString(), const QStringList &labels = QStringList()) const;

    QUrl ComposeProjectLabelsUrl(const QString &projectID,
                                 const QString &with_counts = "false",
                                 const QString &search = QString()) const;
    QUrl ComposeProjectUrl(const QString &projectName) const;

    void setBaseURL(const QUrl &newBaseURL);

protected:
    QUrlQuery SetQuery(const QMap<QString, QVariant> data) const;
    QString ComposeUrl(UrlComposer::UrlTypes target) const;

private:
    QUrl mBaseURL;
};

#endif // URLCOMPOSER_H
