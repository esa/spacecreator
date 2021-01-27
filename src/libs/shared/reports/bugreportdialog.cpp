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
#include "settingsmanager.h"
#include "ui_bugreportdialog.h"

#include <QFile>
#include <QPushButton>

namespace reports {

static const char *defaultHost = "https://git.vikingsoftware.com";
static const int defaultProjectID = 60;

struct BugreportDialog::BugreportDialogPrivate {
    explicit BugreportDialogPrivate(BugreportDialog *dialog, const QString &logPath, const QList<QPixmap> &images)
        : reportHandler(nullptr)
        , images(images)
    {
        ui.setupUi(dialog);

        const QString host = shared::SettingsManager::load<QString>(
                shared::SettingsManager::BugReport::Host, QString::fromLatin1(defaultHost));
        ui.hostLineEdit->setText(host);

        const int projID =
                shared::SettingsManager::load<int>(shared::SettingsManager::BugReport::ProjectID, defaultProjectID);
        ui.projectLineEdit->setText(QString::number(projID));

        const QString accessToken =
                shared::SettingsManager::load<QByteArray>(shared::SettingsManager::BugReport::AccessToken);
        ui.accessTokenLineEdit->setText(accessToken);

        if (!logPath.isEmpty()) {
            QFile file(logPath);
            if (file.open(QIODevice::ReadOnly)) {
                ui.logTextEdit->setPlainText(file.readAll());
                file.close();
            }
        } else {
            ui.logTextEdit->hide();
            ui.logTextEditLabel->hide();
        }

        connect(ui.buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, dialog,
                &BugreportDialog::sendReport);
        connect(ui.buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, dialog, &QDialog::close);
        connect(ui.titleLineEdit, &QLineEdit::textChanged, dialog, &BugreportDialog::updateButtonBox);
        connect(ui.hostLineEdit, &QLineEdit::textChanged, dialog, &BugreportDialog::updateButtonBox);
        connect(ui.accessTokenLineEdit, &QLineEdit::textChanged, dialog, &BugreportDialog::updateButtonBox);
        connect(ui.projectLineEdit, &QLineEdit::textChanged, dialog, &BugreportDialog::updateButtonBox);
    }

    Ui::BugreportDialog ui;
    reports::BugReportHandler *reportHandler;
    const QList<QPixmap> images;
};

BugreportDialog::BugreportDialog(const QString &logPath, const QList<QPixmap> &images, QWidget *parent)
    : QDialog(parent)
    , d(new BugreportDialogPrivate(this, logPath, images))
{
    updateButtonBox();
}

BugreportDialog::BugreportDialog(const QList<QPixmap> &images, QWidget *parent)
    : BugreportDialog(QString(), images, parent)
{
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

    d->reportHandler = new reports::BugReportHandler(d->ui.hostLineEdit->text(), d->ui.projectLineEdit->text(),
            d->ui.accessTokenLineEdit->text().toUtf8(), this);
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

    shared::SettingsManager::store<QString>(shared::SettingsManager::BugReport::Host, d->ui.hostLineEdit->text());
    shared::SettingsManager::store<int>(
            shared::SettingsManager::BugReport::ProjectID, d->ui.projectLineEdit->text().toInt());
    shared::SettingsManager::store<QByteArray>(
            shared::SettingsManager::BugReport::AccessToken, d->ui.accessTokenLineEdit->text().toUtf8());
}

void BugreportDialog::updateButtonBox()
{
    auto applyButton = d->ui.buttonBox->button(QDialogButtonBox::Apply);
    applyButton->setDisabled(d->ui.hostLineEdit->text().isEmpty() || d->ui.projectLineEdit->text().isEmpty()
            || d->ui.accessTokenLineEdit->text().isEmpty() || d->ui.titleLineEdit->text().isEmpty());
}

}
