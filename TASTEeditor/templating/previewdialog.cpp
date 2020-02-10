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

#include "previewdialog.h"

#include "stringtemplate.h"
#include "xmlhighlighter.h"

#include <QTextEdit>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QSpinBox>
#include <QLabel>

namespace taste3 {
namespace templating {

PreviewDialog::PreviewDialog(QWidget *parent)
    : QDialog(parent)
    , m_stringTemplate(new templating::StringTemplate(this))
    , m_textEdit(new QTextEdit(this))
{
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);
    setWindowState(Qt::WindowMaximized);

    new XMLHighlighter(m_textEdit->document());

    QDesktopWidget *desktop = QApplication::desktop();
    QRect geometry = desktop->availableGeometry(this);
    setMinimumSize(geometry.width() * 0.8, geometry.height() * 0.8);

    m_indentWidget = new QWidget(this);

    QLabel *indentLabel = new QLabel(tr("Auto-formatting Indent:"), m_indentWidget);

    QSpinBox *indentSpinBox = new QSpinBox(m_indentWidget);
    indentSpinBox->setRange(0, 10);
    indentSpinBox->setValue(m_stringTemplate->autoFormattingIndent());

    QHBoxLayout *indentLayout = new QHBoxLayout();
    indentLayout->addWidget(indentLabel);
    indentLayout->addWidget(indentSpinBox);
    m_indentWidget->setLayout(indentLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Close);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(m_indentWidget);
    hBoxLayout->addStretch();
    hBoxLayout->addWidget(buttonBox);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(m_textEdit);
    vLayout->addLayout(hBoxLayout);
    setLayout(vLayout);

    connect(m_stringTemplate, &StringTemplate::errorOccurred, this, &PreviewDialog::onErrorOccurred, Qt::QueuedConnection);
    connect(indentSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &PreviewDialog::onIndentChanged);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PreviewDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PreviewDialog::reject);
}

/**
 * @brief PreviewDialog::parse generates ahd shows XML text document
 * @param grouppedObjects groupped objects which are used as replacement in template
 * @param templateFileName name of template file
 */
void PreviewDialog::parse(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName)
{
    const QString& result = m_stringTemplate->parseFile(grouppedObjects, templateFileName);
    m_textEdit->setText(result);
    m_indentWidget->setVisible(m_stringTemplate->isXml());
    open();
}

/**
 * @brief PreviewDialog::text returns generated and formatted XML text document
 * @return
 */
QString PreviewDialog::text() const
{
    return m_textEdit->toPlainText();
}

/**
 * @brief PreviewDialog::onErrorOccurred shows error message
 * if any error is occurred during generating of XML document
 * @param error
 */
void PreviewDialog::onErrorOccurred(const QString &error)
{
    QMessageBox::warning(this, tr("Error occurred"), error);
}

/**
 * @brief PreviewDialog::onIndentChanged sets a new indent and reformats XML text document
 * @param value new indent value
 */
void PreviewDialog::onIndentChanged(int value)
{
    m_stringTemplate->setAutoFormattingIndent(value);
    QString result = m_stringTemplate->formatText(m_textEdit->toPlainText());
    m_textEdit->setText(result);
}

} // ns processing
} // ns taste3
