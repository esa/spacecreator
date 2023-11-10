#ifndef REQUIREMENTSMANAGER_H
#define REQUIREMENTSMANAGER_H

#include "requirement.h"

#include <QObject>
#include <memory>

class RequirementsManager: public QObject
{
    Q_OBJECT
public:

    enum class REPO_TYPE
    {
        GITLAB
    };

    RequirementsManager(REPO_TYPE repoType);
    ~RequirementsManager();
    void setCredentials(const QString &url, const QString &token);
    void requestRequirements(const QString &projectID, const QString &assignee, const QString &author) const;
    void createRequirement(const QString &projectID, const QString &title, const QString &description) const;
    void requestProjectID(const QString &projectName);

Q_SIGNALS:
    void listOfRequirements(const QList<requirement::Requirement> &);
    void requestedProjectID(QString);
    void connectionError(QString errorString);

private:
    class RequirementsManagerPrivate;
    std::unique_ptr<RequirementsManagerPrivate> d;
};

#endif // REQUIREMENTSMANAGER_H
