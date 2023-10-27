#ifndef URLCOMPOSER_H
#define URLCOMPOSER_H

#include <QUrl>

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
    QString ComposeUrl(UrlComposer::UrlTypes target) const;

    QUrl ComposeGetIssuesUrl(const QString &projectID, const QString &assignee,
                             const QString &author,
                             const QStringList &iids,
                             const QString &scope = "all") const;
    QUrl ComposeCreateIssueUrl(const QString &projectID,
                               const QString &title,
                               const QString &description,
                               const QString &assignee) const;
    QUrl ComposeEditIssueUrl(const QString &projectID,
                             const QString &issueID,
                             const QString &title = QString(),
                             const QString &description = QString(),
                             const QString &assignee = QString(),
                             const QString &state_event = QString(),
                             const QString &labels = QString()) const;

    QUrl ComposeProjectLabelsUrl(const QString &projectID,
                                 const QString &with_counts = "false",
                                 const QString &search = QString()) const;
    QUrl ComposeProjectUrl(const QString &projectName) const;

    void setBaseURL(const QUrl &newBaseURL);

protected:
    QUrlQuery SetQuery(const QMap<QString, QVariant> data) const;

private:
    QUrl mBaseURL;
};

#endif // URLCOMPOSER_H
