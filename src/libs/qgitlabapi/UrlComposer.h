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
        ProjectLabels,

    };

    UrlComposer(QUrl url);
    QString ComposeUrl(UrlComposer::UrlTypes target) const;

    QUrl ComposeGetIssuesUrl(const QString &assignee,
                             const QString &author,
                             const QString &iids,
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

protected:
    QUrlQuery SetQuery(const QMap<QString, QVariant> data) const;

private:
    QUrl mBaseURL;
};

#endif // URLCOMPOSER_H
