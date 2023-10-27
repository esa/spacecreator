#include "gitlabrequirements.h"

#include "interfacedocument.h"
#include "ui_gitlabrequirements.h"

#include <QDesktopServices>
#include <QLineEdit>
#include <QSettings>
#include <QTableView>
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

    connect(ui->AllRequirements, &QTableView::doubleClicked, this, &GitLabRequirements::openIssueLink);
    connect(ui->Refresh, &QPushButton::clicked, this, &GitLabRequirements::onLoginUpdate);
    connect(ui->UrlLineEdit, &QLineEdit::editingFinished, this, &GitLabRequirements::onChangeOfCredentials);
    connect(ui->TokenLineEdit, &QLineEdit::editingFinished, this, &GitLabRequirements::onChangeOfCredentials);

    connect(ui->filterLineEdit, &QLineEdit::textChanged, &m_filterModel, &QSortFilterProxyModel::setFilterFixedString);
    connect(ui->clearFilterButton, &QPushButton::clicked, ui->filterLineEdit, &QLineEdit::clear);
    connect(&mReqManager, &RequirementsManager::RequestedProjectID, this, [this](const QString &projectID) {
        static const QString anyAssignee("");
        static const QString anyAuthor("");
        static const QStringList anyIssues = {};
        mReqManager.requestRequirements(projectID, anyAssignee, anyAuthor, anyIssues);
    });

    connect(&mReqManager, &RequirementsManager::listOfIssues, this, [this](QList<Issue> issues) {
        QList<Requirement> requirements;
        requirements.reserve(issues.size());
        for (const auto &issue : issues) {
            requirements.append(requirementFromIssue(issue));
        }
        m_model.addRequirements(requirements);
    });

    LoadSavedCredentials();
    onLoginUpdate();
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
    auto issue_url = m_document->requirementsURL() + "/-/issues/" + issue.mIssueID;
    return { issue.mIssueID, issue.mTitle, issue.mDescription, issue.mIssueID, issue_url};
}

void GitLabRequirements::onLoginUpdate()
{
    m_model.clear();

    mReqManager.setCredentials(ui->UrlLineEdit->text(), ui->TokenLineEdit->text());

    auto projectName = QUrl(ui->UrlLineEdit->text()).path().split("/").last();
    mReqManager.RequestProjectID(projectName);
}

void GitLabRequirements::openIssueLink(const QModelIndex &index)
{
   QDesktopServices::openUrl(index.data(Qt::UserRole).toString());
}
