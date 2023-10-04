#ifndef ISSUE_H
#define ISSUE_H

#include <QJsonDocument>
#include "QGitlabAPI_global.h"

class QGITLABAPI_EXPORT Issue
{
public:

    Issue();
    Issue(const QJsonObject &issue);

    QString mIssueID;
    QString mTitle;
    QString mDescription;
    QString mAssignee;
    QString mStatus;
    QString mState_event;
    QString mLabels;
};

#endif // ISSUE_H
