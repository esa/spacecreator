#include "gitlabrequirements.h"

#include "interfacedocument.h"
#include "ui_gitlabrequirements.h"

#include <QLineEdit>
#include <QSettings>
#include <requirement.h>

using namespace requirement;

GitLabRequirements::GitLabRequirements(QPointer<ive::InterfaceDocument> document, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GitLabRequirements)
    , mReqManager(RequirementsManager::REPO_TYPE::GITLAB)
    , m_document(document)

{
    ui->setupUi(this);
    ui->AllRequirements->setModel(&m_model);
    LoadSavedCredentials();
    onLoginUpdate();
    connect(ui->Refresh, &QPushButton::clicked, this, &GitLabRequirements::onLoginUpdate);
    connect(ui->UrlLineEdit, &QLineEdit::editingFinished, this, &GitLabRequirements::onChangeOfCredentials);
    connect(ui->TokenLineEdit, &QLineEdit::editingFinished, this, &GitLabRequirements::onChangeOfCredentials);
}

void GitLabRequirements::LoadSavedCredentials()
{
    setUrl(m_document->requestsURL());
    QSettings settings;
    auto gitlabToken = settings.value(m_document->requestsURL() + "__token").toString();
    setToken(gitlabToken);
}
GitLabRequirements::~GitLabRequirements()
{
    delete ui;
}

void GitLabRequirements::setUrl(const QString &url)
{
    ui->UrlLineEdit->setText(url);
}

void GitLabRequirements::setToken(const QString &token)
{
    ui->TokenLineEdit->setText(token);
}

void GitLabRequirements::onChangeOfCredentials()
{
    QSettings settings;
    if (!ui->UrlLineEdit->text().isEmpty()) {
        auto gitlabToken = settings.value(ui->UrlLineEdit->text() + "__token").toString();
        if ((gitlabToken != ui->TokenLineEdit->text()) || (ui->UrlLineEdit->text() != m_document->requestsURL())) {
            settings.remove(m_document->requestsURL() + "__token");
            settings.setValue(ui->UrlLineEdit->text() + "__token", ui->TokenLineEdit->text());
        }
    } else {
        settings.remove(m_document->requestsURL() + "__token");
        m_document->setRequestURL("");
        setToken("");
    }
}

Requirement GitLabRequirements::requirementFromIssue(const Issue &issue) const
{
    return { issue.mIssueID, issue.mTitle, issue.mDescription, issue.mIssueID };
}

void GitLabRequirements::onLoginUpdate()
{
    mReqManager.setCredentials(ui->UrlLineEdit->text(), ui->TokenLineEdit->text());

    static const QString anyAssignee("");
    static const QString anyAuthor("");
    static const QStringList anyIssues = {};
    m_model.setRequirements({});
    mReqManager.requestRequirements(anyAssignee, anyAuthor, anyIssues);
    connect(&mReqManager, &RequirementsManager::listOfIssues, [this](QList<Issue> issues) {
        QList<Requirement> requirements;
        requirements.reserve(issues.size());
        for (const auto &issue : issues) {
            requirements.append(requirementFromIssue(issue));
        }
        m_model.setRequirements(requirements);
    });
}
