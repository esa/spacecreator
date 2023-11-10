#include "requirementswidget.h"

#include "ui_requirementswidget.h"

#include <QDesktopServices>
#include <QHeaderView>
#include <QLineEdit>
#include <QSettings>
#include <QTableView>
#include <requirement.h>

using namespace requirement;

RequirementsWidget::RequirementsWidget(QByteArray requirementsUrl, QStringList requirementsIDs, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RequirementsWidget)
    , mReqManager(RequirementsManager::REPO_TYPE::GITLAB)
    , m_requirementsUrl(requirementsUrl)

{
    ui->setupUi(this);
    m_filterModel.setDynamicSortFilter(true);
    m_filterModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_filterModel.setFilterKeyColumn(-1);
    m_filterModel.setSourceModel(&m_model);
    ui->AllRequirements->setModel(&m_filterModel);
    ui->AllRequirements->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AllRequirements->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->AllRequirements->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->AllRequirements->horizontalHeader()->setStretchLastSection(false);
    m_model.setSelectedRequirementsIDs(requirementsIDs);
    connect(ui->AllRequirements, &QTableView::doubleClicked, this, &RequirementsWidget::openIssueLink);
    connect(&m_model, &requirement::RequirementsModel::dataChanged,
            [this](const QModelIndex &topLeft, const QModelIndex &BottomRight, const QList<int> &roles) {
                if (topLeft != BottomRight) {
                    return;
                }

                if (topLeft.column() == requirement::RequirementsModel::CHECKED && roles.contains(Qt::CheckStateRole)) {
                    auto index = m_model.index(topLeft.row(), requirement::RequirementsModel::REQUIREMENT_ID);
                    auto requirementID = m_model.data(index, Qt::DisplayRole).toString();
                    auto checked = m_model.data(topLeft, Qt::CheckStateRole).toBool();

                    emit requirementSelected(requirementID, checked);
                }
            });
    connect(ui->Refresh, &QPushButton::clicked, this, &RequirementsWidget::onLoginUpdate);
    connect(ui->UrlLineEdit, &QLineEdit::editingFinished, this, &RequirementsWidget::onChangeOfCredentials);
    connect(ui->TokenLineEdit, &QLineEdit::editingFinished, this, &RequirementsWidget::onChangeOfCredentials);

    connect(ui->filterLineEdit, &QLineEdit::textChanged, &m_filterModel, &QSortFilterProxyModel::setFilterFixedString);
    connect(ui->clearFilterButton, &QPushButton::clicked, ui->filterLineEdit, &QLineEdit::clear);
    connect(&mReqManager, &RequirementsManager::RequestedProjectID, this, [this](const QString &projectID) {
        static const QString anyAssignee("");
        static const QString anyAuthor("");
        mReqManager.requestRequirements(projectID, anyAssignee, anyAuthor);
    });

    connect(&mReqManager, &RequirementsManager::listOfRequirements, &m_model,
            &requirement::RequirementsModel::addRequirements);

    LoadSavedCredentials();
    onLoginUpdate();
}

void RequirementsWidget::LoadSavedCredentials()
{
    setUrl(m_requirementsUrl);
    QSettings settings;
    auto gitlabToken = settings.value(m_requirementsUrl + "__token").toString();
    setToken(gitlabToken);
}
RequirementsWidget::~RequirementsWidget()
{
    delete ui;
}

void RequirementsWidget::setUrl(const QString &url)
{
    ui->UrlLineEdit->setText(url);
}

void RequirementsWidget::setToken(const QString &token)
{
    ui->TokenLineEdit->setText(token);
}

void RequirementsWidget::onChangeOfCredentials()
{
    QSettings settings;
    if (!ui->UrlLineEdit->text().isEmpty()) {
        auto gitlabToken = settings.value(ui->UrlLineEdit->text() + "__token").toString();
        if ((gitlabToken != ui->TokenLineEdit->text()) || (ui->UrlLineEdit->text() != m_requirementsUrl)) {
            settings.remove(m_requirementsUrl + "__token");
            settings.setValue(ui->UrlLineEdit->text() + "__token", ui->TokenLineEdit->text());
            m_requirementsUrl = ui->UrlLineEdit->text().toUtf8();
        }
    } else {
        settings.remove(m_requirementsUrl + "__token");
        m_requirementsUrl.clear();
        setToken("");
    }
    emit requirementsUrlChanged(m_requirementsUrl);
}

void RequirementsWidget::onLoginUpdate()
{
    m_model.clear();
    QUrl api_url;
    api_url.setScheme("https");
    api_url.setHost(QUrl(ui->UrlLineEdit->text()).host());
    api_url.setPath("/api/v4/");

    mReqManager.setCredentials(api_url.toString(), ui->TokenLineEdit->text());

    auto projectName = QUrl(ui->UrlLineEdit->text()).path().split("/").last();
    mReqManager.RequestProjectID(projectName);
}

void RequirementsWidget::openIssueLink(const QModelIndex &index)
{
    QDesktopServices::openUrl(index.data(RequirementsModel::RoleNames::IssueLinkRole).toString());
}
