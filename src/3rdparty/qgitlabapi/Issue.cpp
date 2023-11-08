 #include "Issue.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QString>

Issue::Issue()
{
}

Issue::Issue(const QJsonObject &issue)
{
    mIssueIID = QString::number(issue["iid"].toInteger());
    mTitle = issue["title"].toString();
    mDescription = issue["description"].toString();
    mAuthor = issue["author"].toString();
    mAssignee = issue["assignee"].toString();
    mState = issue["state"].toString();
    mLabels = issue["labels"].toString();
    mIssueType = issue["issue_type"].toString();
    mCreatedAt = QDateTime::fromString(issue["created_at"].toString(), Qt::ISODate);
    mUpdatedAt = QDateTime::fromString(issue["updated_at"].toString(), Qt::ISODate);
    mNotesCount = issue["user_notes_count"].toInt();
}
