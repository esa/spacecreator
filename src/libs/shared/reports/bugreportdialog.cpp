/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "bugreportdialog.h"

#include "bugreporthandler.h"
#include "settingsappoption.h"
#include "ui_bugreportdialog.h"

#include <QFile>
#include <QPushButton>

static const char *defaultHost = "https://git.vikingsoftware.com";
static const int defaultProjectID = 60;

static const QString localName() { return "GroupBugReport"; }

namespace reports {

struct BugreportDialog::BugreportDialogPrivate {
    explicit BugreportDialogPrivate(const QList<QPixmap> &images)
        : reportHandler(nullptr)
        , images(images)
        , host("GroupBugReport/Host")
        , projectID("GroupBugReport/ProjectID")
        , accessToken("GroupBugReport/AccessToken")
    {
    }

    Ui::BugreportDialog ui;
    reports::BugReportHandler* reportHandler;
    const QList<QPixmap> images;

    shared::SettingsAppOption host;
    shared::SettingsAppOption projectID;
    shared::SettingsAppOption accessToken;
};

BugreportDialog::BugreportDialog(const QString &logPath, const QList<QPixmap> &images, QWidget *parent)
    : QDialog(parent)
    , d(new BugreportDialogPrivate(images))
{
    d->ui.setupUi(this);
    connect(d->ui.buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &BugreportDialog::sendReport);
    connect(d->ui.buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &QDialog::close);
    connect(d->ui.titleLineEdit, &QLineEdit::textChanged, this, &BugreportDialog::updateButtonBox);
    connect(d->ui.hostLineEdit, &QLineEdit::textChanged, this, &BugreportDialog::updateButtonBox);
    connect(d->ui.accessTokenLineEdit, &QLineEdit::textChanged, this, &BugreportDialog::updateButtonBox);
    connect(d->ui.projectLineEdit, &QLineEdit::textChanged, this, &BugreportDialog::updateButtonBox);

    const QString settingsHost = d->host.read().toString();
    d->ui.hostLineEdit->setText(settingsHost.isEmpty() ? QString(::defaultHost) : settingsHost);

    const QString settingsProjectID = d->projectID.read().toString();
    d->ui.projectLineEdit->setText(settingsProjectID.isEmpty() ? QString::number(::defaultProjectID) : settingsProjectID);

    d->ui.accessTokenLineEdit->setText(d->accessToken.read().toByteArray());

    QFile file(logPath);
    if (file.open(QIODevice::ReadOnly)) {
        d->ui.logTextEdit->setPlainText(file.readAll());
        file.close();
    }
}

BugreportDialog::~BugreportDialog()
{
    delete d;
}

void BugreportDialog::sendReport()
{
    auto applyButton = d->ui.buttonBox->button(QDialogButtonBox::Apply);
    if (applyButton)
        applyButton->setDisabled(true);

    delete d->reportHandler;

    d->reportHandler = new reports::BugReportHandler(d->ui.hostLineEdit->text(), d->ui.projectLineEdit->text(), d->ui.accessTokenLineEdit->text().toUtf8(), this);
    connect(d->reportHandler, &reports::BugReportHandler::error, this, &BugreportDialog::error);
    connect(d->reportHandler, &reports::BugReportHandler::reportSent, this, &BugreportDialog::reportSent);

    auto report = QSharedPointer<reports::BugReport>(new reports::BugReport);
    report->title = d->ui.titleLineEdit->text();
    report->log = d->ui.logTextEdit->toPlainText().toUtf8();
    report->description = d->ui.descriptionTextEdit->toPlainText();
    report->images = d->images;

    d->reportHandler->sendReport(report);
    d->ui.statusLabel->setText(tr("Sending report..."));
}

void BugreportDialog::error(const QString &errorString)
{
    d->ui.statusLabel->setText(errorString);
    auto applyButton = d->ui.buttonBox->button(QDialogButtonBox::Apply);
    if (applyButton)
        applyButton->setEnabled(true);
}

void BugreportDialog::reportSent(const QString &msg)
{
    const QString link = QStringLiteral("<a href=\"%1\">%1</a>").arg(msg);
    d->ui.statusLabel->setText(link);
    d->ui.buttonBox->button(QDialogButtonBox::Apply)->setDisabled(true);

    d->host.write(d->ui.hostLineEdit->text());
    d->projectID.write(d->ui.projectLineEdit->text());
    d->accessToken.write(d->ui.accessTokenLineEdit->text().toUtf8());
}

void BugreportDialog::updateButtonBox()
{
    auto applyButton = d->ui.buttonBox->button(QDialogButtonBox::Apply);
    applyButton->setDisabled(d->ui.hostLineEdit->text().isEmpty() || d->ui.projectLineEdit->text().isEmpty() || d->ui.accessTokenLineEdit->text().isEmpty() || d->ui.titleLineEdit->text().isEmpty());
}

}
