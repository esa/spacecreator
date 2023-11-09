#ifndef ISSUE_H
#define ISSUE_H

#include "QGitlabAPI_global.h"

#include <QDateTime>
#include <QJsonDocument>

/**
 * @brief The Issue class holds data for a single issue
 */
class QGITLABAPI_EXPORT Issue
{
public:

    Issue();
    Issue(const QJsonObject &issue);

    QString mIssueIID; // unique ID within this project
    QString mTitle;
    QString mDescription;
    QString mAuthor;
    QString mAssignee;
    QString mState;
    QString mState_event; /// @note should that be part of this class?
    QString mLabels;
    QString mIssueType;
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    int mNotesCount;
};

#endif // ISSUE_H
