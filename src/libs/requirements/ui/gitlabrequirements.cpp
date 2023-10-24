#include "gitlabrequirements.h"

#include "interfacedocument.h"
#include "ui_gitlabrequirements.h"

#include <QLineEdit>
#include <QSettings>

GitLabRequirements::GitLabRequirements(QPointer<ive::InterfaceDocument> document, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GitLabRequirements)
    , mReqManager(RequirementsManager::REPO_TYPE::GITLAB)
    , m_document(document)

{
    ui->setupUi(this);
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

void GitLabRequirements::deleteListContent()
{
    ui->AllRequirements->setRowCount(0);
}

void GitLabRequirements::addRequirementRow(const QString &issueID, const QString &description)
{
    static const int REQID = 0;
    static const int DESCRIPTION = 1;
    ui->AllRequirements->insertRow(ui->AllRequirements->rowCount());
    ui->AllRequirements->setItem(ui->AllRequirements->rowCount() - 1, REQID, new QTableWidgetItem(issueID));
    ui->AllRequirements->setItem(ui->AllRequirements->rowCount() - 1, DESCRIPTION, new QTableWidgetItem(description));
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

void GitLabRequirements::onLoginUpdate()
{
    mReqManager.setCredentials(ui->UrlLineEdit->text(), ui->TokenLineEdit->text());

    static const QString anyAssignee("");
    static const QString anyAuthor("");
    static const QStringList anyIssues = {};
    deleteListContent();
    mReqManager.requestRequirements(anyAssignee, anyAuthor, anyIssues);
    connect(&mReqManager, &RequirementsManager::listOfIssues, [this](QList<Issue> issues) {
        for (const auto &issue : issues) {
            addRequirementRow(issue.mIssueID, issue.mDescription);
        }
    });
}
