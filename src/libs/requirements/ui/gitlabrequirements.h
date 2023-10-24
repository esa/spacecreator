#ifndef GITLABREQUIREMENTS_H
#define GITLABREQUIREMENTS_H

#include "requirementsmanager.h"

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
    void deleteListContent();

protected:
    void LoadSavedCredentials();
    void addRequirementRow(const QString &issueID, const QString &description);
    void onChangeOfCredentials();

protected Q_SLOTS:
    void onLoginUpdate();

private:
    Ui::GitLabRequirements *ui;
    RequirementsManager mReqManager;
    QPointer<ive::InterfaceDocument> m_document;
};

#endif // GITLABREQUIREMENTS_H
