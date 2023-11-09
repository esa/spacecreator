#include "gitlabrequirements.h"

#include <issue.h>

using namespace gitlab;

namespace requirement {

void GitLabRequirements::listOfIssues(const QList<gitlab::Issue> &issues)
{
    QList<Requirement> requirements;
    requirements.reserve(issues.size());
    for (const auto &issue : issues) {
        requirements.append(requirementFromIssue(issue));
    }
    Q_EMIT listOfRequirements(requirements);
}

Requirement GitLabRequirements::requirementFromIssue(const gitlab::Issue &issue)
{
    return { QString::number(issue.mIssueIID), issue.mTitle, issue.mDescription, QString::number(issue.mIssueIID),
        issue.mUrl };
}
}
