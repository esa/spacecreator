#pragma once

#include <QByteArray>
#include <QList>
#include <QMap>
#include <QUrlQuery>
#include <QVariant>

/**
 * @brief The IssueRequestOptions class holds all options for fetching issue data
 * For more documentation see https://docs.gitlab.com/ee/api/issues.html
 */
class IssueRequestOptions
{
public:
    QByteArray mAssignee; /// If not empty, only issues assigned to that assignee (username) are fetched
    QByteArray mAuthor; /// If not empty, only issues assigned to that assignee with that username are fetched
    QList<int> mIids; /// If not empty, return only the issues having the given iids (unique IDs for the whole server)
    QByteArray mScope = "all"; /// Return issues for the given scope: "created_by_me", "assigned_to_me" or "all".
    QByteArray mState = "opened"; /// Return "all" issues or just those that are "opened" or "closed"
    int mPage = 1; /// Number of the page to fetch. See pagination of gitlab API

    /**
     * @brief queryData Creates query data for the URL
     */
    QUrlQuery urlQuery() const;
};

