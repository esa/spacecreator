#include "bugreportdialog.h"

#include "bugreporthandler.h"
#include "settings/appoptions.h"
#include "ui_bugreportdialog.h"

#include <QFile>
#include <QPushButton>

static const QString kDefaultHost = QLatin1String("https://git.vikingsoftware.com");
static const QString kDefaultProjectID = QLatin1String("60");

BugreportDialog::BugreportDialog(const QString &logPath, const QList<QPixmap> &images, QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::BugreportDialog)
    , m_images(images)
{
    m_ui->setupUi(this);
    connect(m_ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this,
            &BugreportDialog::sendReport);
    connect(m_ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &QDialog::close);
    connect(m_ui->titleLineEdit, &QLineEdit::textChanged, this, &BugreportDialog::updateButtonBox);
    connect(m_ui->hostLineEdit, &QLineEdit::textChanged, this, &BugreportDialog::updateButtonBox);
    connect(m_ui->accessTokenLineEdit, &QLineEdit::textChanged, this, &BugreportDialog::updateButtonBox);
    connect(m_ui->projectLineEdit, &QLineEdit::textChanged, this, &BugreportDialog::updateButtonBox);

    const QString settingsHost = taste3::AppOptions::BugReport.Host.read().toString();
    m_ui->hostLineEdit->setText(settingsHost.isEmpty() ? kDefaultHost : settingsHost);

    const QString settingsProjectID = taste3::AppOptions::BugReport.ProjectID.read().toString();
    m_ui->projectLineEdit->setText(settingsProjectID.isEmpty() ? kDefaultProjectID : settingsProjectID);

    m_ui->accessTokenLineEdit->setText(taste3::AppOptions::BugReport.AccessToken.read().toByteArray());

    QFile file(logPath);
    if (file.open(QIODevice::ReadOnly)) {
        m_ui->logTextEdit->setPlainText(file.readAll());
        file.close();
    }
}

BugreportDialog::~BugreportDialog()
{
    delete m_ui;
}

void BugreportDialog::sendReport()
{
    auto applyButton = m_ui->buttonBox->button(QDialogButtonBox::Apply);
    if (applyButton)
        applyButton->setDisabled(true);

    delete m_reportHandler;

    m_reportHandler = new reports::BugReportHandler(m_ui->hostLineEdit->text(), m_ui->projectLineEdit->text(),
                                                    m_ui->accessTokenLineEdit->text().toUtf8(), this);
    connect(m_reportHandler, &reports::BugReportHandler::error, this, &BugreportDialog::error);
    connect(m_reportHandler, &reports::BugReportHandler::reportSent, this, &BugreportDialog::reportSent);

    auto report = QSharedPointer<reports::BugReport>(new reports::BugReport);
    report->title = m_ui->titleLineEdit->text();
    report->log = m_ui->logTextEdit->toPlainText().toUtf8();
    report->description = m_ui->descriptionTextEdit->toPlainText();
    report->images = m_images;

    m_reportHandler->sendReport(report);
    m_ui->statusLabel->setText(tr("Sending report..."));
}

void BugreportDialog::error(const QString &errorString)
{
    m_ui->statusLabel->setText(errorString);
    auto applyButton = m_ui->buttonBox->button(QDialogButtonBox::Apply);
    if (applyButton)
        applyButton->setEnabled(true);
}

void BugreportDialog::reportSent(const QString &msg)
{
    const QString link = QStringLiteral("<a href=\"%1\">%1</a>").arg(msg);
    m_ui->statusLabel->setText(link);
    auto applyButton = m_ui->buttonBox->button(QDialogButtonBox::Apply);
    if (applyButton)
        applyButton->setDisabled(true);

    taste3::AppOptions::BugReport.Host.write(m_ui->hostLineEdit->text());
    taste3::AppOptions::BugReport.ProjectID.write(m_ui->projectLineEdit->text());
    taste3::AppOptions::BugReport.AccessToken.write(m_ui->accessTokenLineEdit->text().toUtf8());
}

void BugreportDialog::updateButtonBox()
{
    auto applyButton = m_ui->buttonBox->button(QDialogButtonBox::Apply);
    if (!applyButton)
        return;

    applyButton->setDisabled(m_ui->hostLineEdit->text().isEmpty() || m_ui->projectLineEdit->text().isEmpty()
                             || m_ui->accessTokenLineEdit->text().isEmpty() || m_ui->titleLineEdit->text().isEmpty());
}
