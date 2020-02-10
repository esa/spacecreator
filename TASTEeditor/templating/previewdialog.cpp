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
#include "templatehighlighter.h"
#include "xmlhighlighter.h"

#include <QTextEdit>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QSpinBox>
#include <QLabel>
#include <QSplitter>
#include <QPushButton>
#include <QTextStream>

namespace taste3 {
namespace templating {

PreviewDialog::PreviewDialog(QWidget *parent)
    : QDialog(parent)
    , m_stringTemplate(new templating::StringTemplate(this))
    , m_templateTextEdit(new QTextEdit(this))
    , m_resultTextEdit(new QTextEdit(this))
{
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);
    setWindowState(Qt::WindowMaximized);

    QFont textFont(QStringLiteral("Courier"));
    m_templateTextEdit->setFont(textFont);
    m_resultTextEdit->setFont(textFont);

    new TemplateHighlighter(m_templateTextEdit->document());
    new XMLHighlighter(m_resultTextEdit->document());

    QDesktopWidget *desktop = QApplication::desktop();
    QRect geometry = desktop->availableGeometry(this);
    setMinimumSize(geometry.width() * 0.8, geometry.height() * 0.8);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(m_templateTextEdit);
    splitter->addWidget(m_resultTextEdit);

    QDialogButtonBox *templateButtonBox = new QDialogButtonBox(this);
    templateButtonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Open | QDialogButtonBox::Apply);
    templateButtonBox->button(QDialogButtonBox::Save)->setText(tr("Save &Template"));
    templateButtonBox->button(QDialogButtonBox::Open)->setText(tr("&Open Template"));

    QLabel *indentLabel = new QLabel(tr("XML Auto-formatting Indent:"), this);

    QSpinBox *indentSpinBox = new QSpinBox(this);
    indentSpinBox->setRange(0, 10);
    indentSpinBox->setValue(m_stringTemplate->autoFormattingIndent());

    QDialogButtonBox *resultButtonBox = new QDialogButtonBox(this);
    resultButtonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Close);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(templateButtonBox);
    hBoxLayout->addStretch(2);
    hBoxLayout->addWidget(indentLabel);
    hBoxLayout->addWidget(indentSpinBox);
    hBoxLayout->addStretch(1);
    hBoxLayout->addWidget(resultButtonBox);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(splitter);
    vLayout->addLayout(hBoxLayout);
    setLayout(vLayout);

    connect(m_stringTemplate, &StringTemplate::errorOccurred, this, &PreviewDialog::onErrorOccurred, Qt::QueuedConnection);
    connect(templateButtonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &PreviewDialog::onApplyTemplate);
    connect(templateButtonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &PreviewDialog::onSaveTemplate);
    connect(templateButtonBox->button(QDialogButtonBox::Open), &QPushButton::clicked, this, &PreviewDialog::onOpenTemplate);
    connect(templateButtonBox, &QDialogButtonBox::rejected, this, &PreviewDialog::reject);
    connect(indentSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &PreviewDialog::onIndentChanged);
    connect(resultButtonBox, &QDialogButtonBox::accepted, this, &PreviewDialog::accept);
    connect(resultButtonBox, &QDialogButtonBox::rejected, this, &PreviewDialog::reject);
}

/**
 * @brief PreviewDialog::parse generates ahd shows XML text document
 * @param grouppedObjects groupped objects which are used as replacement in template
 * @param templateFileName name of template file
 */
void PreviewDialog::parse(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName)
{
    QFile templateFile(templateFileName);
    if (templateFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&templateFile);
        m_templateTextEdit->setText(stream.readAll());

        const QString& result = m_stringTemplate->parseFile(grouppedObjects, templateFileName);
        m_resultTextEdit->setText(result);
    }
    else {
        m_templateTextEdit->clear();
        m_resultTextEdit->clear();
        onErrorOccurred(tr("Unable to open template file: %").arg(templateFileName));
    }

    open();
}

/**
 * @brief PreviewDialog::text returns generated and formatted XML text document
 * @return
 */
QString PreviewDialog::text() const
{
    return m_resultTextEdit->toPlainText();
}

void PreviewDialog::onApplyTemplate()
{

}

void PreviewDialog::onSaveTemplate()
{

}

void PreviewDialog::onOpenTemplate()
{

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
    const QString& result = m_stringTemplate->formatText(m_resultTextEdit->toPlainText());
    m_resultTextEdit->setText(result);
}

} // ns processing
} // ns taste3
