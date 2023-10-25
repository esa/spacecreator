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
    m_filterModel.setDynamicSortFilter(true);
    m_filterModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_filterModel.setFilterKeyColumn(-1);
    m_filterModel.setSourceModel(&m_model);
    ui->AllRequirements->setModel(&m_filterModel);

    LoadSavedCredentials();
    onLoginUpdate();
    connect(ui->Refresh, &QPushButton::clicked, this, &GitLabRequirements::onLoginUpdate);
    connect(ui->UrlLineEdit, &QLineEdit::editingFinished, this, &GitLabRequirements::onChangeOfCredentials);
    connect(ui->TokenLineEdit, &QLineEdit::editingFinished, this, &GitLabRequirements::onChangeOfCredentials);

    connect(ui->filterLineEdit, &QLineEdit::textChanged, &m_filterModel, &QSortFilterProxyModel::setFilterFixedString);
    connect(ui->clearFilterButton, &QPushButton::clicked, ui->filterLineEdit, &QLineEdit::clear);
}

void GitLabRequirements::LoadSavedCredentials()
{
    setUrl(m_document->requirementsURL());
    QSettings settings;
    auto gitlabToken = settings.value(m_document->requirementsURL() + "__token").toString();
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
        if ((gitlabToken != ui->TokenLineEdit->text()) || (ui->UrlLineEdit->text() != m_document->requirementsURL())) {
            settings.remove(m_document->requirementsURL() + "__token");
            settings.setValue(ui->UrlLineEdit->text() + "__token", ui->TokenLineEdit->text());
            m_document->setRequirementsURL(ui->UrlLineEdit->text());
        }
    } else {
        settings.remove(m_document->requirementsURL() + "__token");
        m_document->setRequirementsURL("");
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
