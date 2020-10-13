/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "asn1fileview.h"

#include "asn1xmlparser.h"
#include "file.h"
#include "ui_asn1fileview.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>

namespace asn1 {

/*!
  \class msc::ASN1FileView
  \brief Show the contents of ASN.1 files.

  This is the widget that is used in the main window to show the contents of ASN.1 files.
*/

ASN1FileView::ASN1FileView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ASN1FileView)
{
    ui->setupUi(this);
    connect(ui->selectFileButton, &QPushButton::clicked, this, &ASN1FileView::selectFile);
}

ASN1FileView::~ASN1FileView()
{
    delete ui;
}

QSize ASN1FileView::sizeHint() const
{
    if (fileContentVisible()) {
        return QSize(200, 200);
    } else {
        return QWidget::sizeHint();
    }
}

/*!
   Returns the asn1 file name without any path.
 */
QString ASN1FileView::fileName() const
{
    return ui->filenameLabel->text();
}

/*!
   Returns if the asn file content is shown
 */
bool ASN1FileView::fileContentVisible() const
{
    return ui->textEdit != nullptr;
}

/*!
   Sets if the ASN file content should be shown
 */
void ASN1FileView::setFileContentVisible(bool visible)
{
    if (fileContentVisible() == visible) {
        return;
    }

    ui->textEdit->setVisible(visible);
    if (visible) {
        ui->textEdit = new QTextEdit(this);
        ui->textEdit->setReadOnly(true);
        ui->gridLayout->addWidget(ui->textEdit, 1, 0, 1, 1);
        fillPreview();
    } else {
        delete ui->textEdit;
        ui->textEdit = nullptr;
    }
    Q_EMIT fileContentVisibleChanged();
}

/*!
 * Update the current directory to \a directory.
 */
void ASN1FileView::setDirectory(const QString &directory)
{
    m_directory = directory;
    fillPreview();
}

/*!
   Sets the file name without any path.
 */
void ASN1FileView::setFileName(const QString &fileName)
{
    ui->filenameLabel->setText(fileName);
    fillPreview();
}

/*!
   Sets the ASN1 full file name including the path.
 */
void ASN1FileView::setFile(const QFileInfo &file)
{
    m_directory = file.absolutePath();
    ui->filenameLabel->setText(file.fileName());
    fillPreview();
}

/*!
 * open a file dialog for the user to select a new file.
 *
 * Before calling this function, the model must be set.
 */
void ASN1FileView::selectFile()
{
    QString file =
            QFileDialog::getOpenFileName(this, tr("ASN.1 file"), QString(), QStringLiteral("ASN1 files (*.asn *.ASN)"));
    if (!file.isEmpty()) {
        QFileInfo fi(file);
        if (ui->filenameLabel->text() != fi.fileName()) {
            setFile(fi);
            Q_EMIT asn1Selected(file);
        }
    }
}

/*!
 * Show the contents of the ASN1 file in the preview
 */
void ASN1FileView::fillPreview()
{
    if (!fileContentVisible()) {
        return;
    }

    QString filename = m_directory + QDir::separator() + ui->filenameLabel->text();
    QFileInfo fi(filename);
    if (!fi.exists()) {
        ui->textEdit->clear();
        return;
    }

    ui->textEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    Asn1Acn::Asn1XMLParser parser;
    QString html = parser.asn1AsHtml(filename);
    if (html.isEmpty()) {
        QFile file(filename);
        if (file.exists()) {
            if (file.open(QIODevice::ReadOnly))
                ui->textEdit->setText(file.readAll());
        }
    } else {
        ui->textEdit->setHtml(html);
    }
}

}
