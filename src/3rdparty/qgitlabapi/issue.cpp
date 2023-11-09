#include "issue.h"

#include <QJsonArray>
#include <QString>

Issue::Issue()
{
}

Issue::Issue(const QJsonObject &issue)
{
    mIssueID = issue["id"].toInteger();
    mIssueIID = issue["iid"].toInteger();
    mTitle = issue["title"].toString();
    mDescription = issue["description"].toString();
    mAuthor = issue["author"].toString();
    mAssignee = issue["assignee"].toString();
    mState = issue["state"].toString();
    QVariant v = issue["labels"].toString();
    QString s = v.toString();
    mLabels.clear();
    for (const QJsonValueRef &value : issue["labels"].toArray()) {
        QString label = value.toString();
        if (!label.isEmpty()) {
            mLabels.append(label);
        }
    }
    mIssueType = issue["issue_type"].toString();
    mCreatedAt = QDateTime::fromString(issue["created_at"].toString(), Qt::ISODate);
    mUpdatedAt = QDateTime::fromString(issue["updated_at"].toString(), Qt::ISODate);
    mNotesCount = issue["user_notes_count"].toInt();
}
