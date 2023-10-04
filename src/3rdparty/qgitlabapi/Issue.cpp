 #include "Issue.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QString>

Issue::Issue()
{

}

Issue::Issue(const QJsonObject &issue)
{
    mIssueID = QString::number(issue["iid"].toInteger());
    mTitle = issue["title"].toString();
    mDescription = issue["description"].toString();
    mAssignee = issue["assignee"].toString();
    mStatus = issue["state"].toString();
    mLabels = issue["labels"].toString();
}
