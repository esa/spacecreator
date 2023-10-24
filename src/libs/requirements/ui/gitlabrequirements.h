#ifndef GITLABREQUIREMENTS_H
#define GITLABREQUIREMENTS_H

#include "requirementsfiltermodel.h"
#include "requirementsmanager.h"
#include "requirementsmodel.h"

#include <QPointer>
#include <QWidget>

namespace Ui {
class GitLabRequirements;
}

namespace ive {
class InterfaceDocument;
}

class RequirementsManager;

class GitLabRequirements : public QWidget
{
    Q_OBJECT

public:
    explicit GitLabRequirements(QPointer<ive::InterfaceDocument> document, QWidget *parent = nullptr);
    ~GitLabRequirements();

    void setUrl(const QString &url);
    void setToken(const QString &token);

protected:
    void LoadSavedCredentials();
    void onChangeOfCredentials();
    requirement::Requirement requirementFromIssue(const Issue &issue) const;

protected Q_SLOTS:
    void onLoginUpdate();

private:
    Ui::GitLabRequirements *ui;
    RequirementsManager mReqManager;
    requirement::RequirementsModel m_model;
    requirement::RequirementsFilterModel m_filterModel;
    QPointer<ive::InterfaceDocument> m_document;
};

#endif // GITLABREQUIREMENTS_H
