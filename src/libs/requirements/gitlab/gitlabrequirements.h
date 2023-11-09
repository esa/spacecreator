#pragma once

#include "requirement.h"

#include <QObject>
#include <issue.h>

namespace requirement {

class GitLabRequirements : public QObject
{
    Q_OBJECT
public:
    void listOfIssues(const QList<gitlab::Issue> &issues);

    static Requirement requirementFromIssue(const gitlab::Issue &issue);

Q_SIGNALS:
    void listOfRequirements(const QList<Requirement> &);
};

}
