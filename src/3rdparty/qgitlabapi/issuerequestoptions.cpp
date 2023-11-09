#include "issuerequestoptions.h"

#include "urlcomposer.h"

using namespace gitlab;

QUrlQuery IssueRequestOptions::urlQuery() const
{
    QMap<QByteArray, QVariant> data;
    if (!mAssignee.isEmpty()) {
        data["assignee_username"] = mAssignee;
    }
    if (!mAuthor.isEmpty()) {
        data["author_username"] = mAuthor;
    }
    if (!mIids.isEmpty()) {
        data["assignee_username"] = QVariant::fromValue<QList<int>>(mIids);
    }
    if (!mScope.isEmpty()) {
        data["scope"] = mScope;
    }
    if (!mState.isEmpty()) {
        data["state"] = mState;
    }
    if (mPage <= 0) {
        data["page"] = 1;
    }
    data["per_page"] = 80;

    return UrlComposer::setQuery(data);
}
