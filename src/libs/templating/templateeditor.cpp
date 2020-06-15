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

#include "templateeditor.h"

#include "stringtemplate.h"
#include "templatehighlighter.h"
#include "templatesyntaxhelpdialog.h"
#include "xmlhighlighter.h"

#include <QApplication>
#include <QBoxLayout>
#include <QBuffer>
#include <QCheckBox>
#include <QDebug>
#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QSplitter>
#include <QTabWidget>
#include <QTextStream>

namespace templating {

/**
 * @brief PreviewDialog::PreviewDialog ctor
 * @param saveHere the filename to save the document to.
 * @param parent
 */
TemplateEditor::TemplateEditor(const QString &saveHere, QWidget *parent)
    : QDialog(parent)
    , m_stringTemplate(templating::StringTemplate::create(this))
    , m_templatesTabWidget(new QTabWidget(this))
    , m_resultTextEdit(new QPlainTextEdit(this))
    , m_helpDialog(nullptr)
    , m_outFileName(saveHere)
{
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);
    setWindowState(Qt::WindowMaximized);
    setWindowTitle(tr("String Template Editor"));

    QDesktopWidget *desktop = QApplication::desktop();
    QRect geometry = desktop->availableGeometry(this);
    setMinimumSize(geometry.width() * 0.8, geometry.height() * 0.8);

    QFont textFont(QStringLiteral("Courier"));
    m_resultTextEdit->setFont(textFont);
    m_resultTextEdit->setWordWrapMode(QTextOption::NoWrap);
    new XMLHighlighter(m_resultTextEdit->document());

    m_templatesTabWidget->setTabPosition(QTabWidget::South);
    addTemplateEditor();

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(m_templatesTabWidget);
    splitter->addWidget(m_resultTextEdit);

    QDialogButtonBox *templateButtonBox = new QDialogButtonBox(this);
    templateButtonBox->setStandardButtons(
            QDialogButtonBox::Save | QDialogButtonBox::Open | QDialogButtonBox::Apply | QDialogButtonBox::Help);
    templateButtonBox->button(QDialogButtonBox::Save)->setText(tr("Save &Template As.."));
    templateButtonBox->button(QDialogButtonBox::Open)->setText(tr("&Open Template"));
    templateButtonBox->button(QDialogButtonBox::Apply)->setText(tr("&Apply and Save"));

    QCheckBox *validateXMLCheckBox = new QCheckBox(tr("Validate and Format XML-document"), this);
    validateXMLCheckBox->setChecked(true);

    QLabel *indentLabel = new QLabel(tr("Auto-formatting Indent:"), this);

    QSpinBox *indentSpinBox = new QSpinBox(this);
    indentSpinBox->setRange(0, 10);
    indentSpinBox->setValue(m_stringTemplate->autoFormattingIndent());

    QDialogButtonBox *resultButtonBox = new QDialogButtonBox(this);
    resultButtonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Close);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(templateButtonBox);
    hBoxLayout->addStretch();
    hBoxLayout->addWidget(validateXMLCheckBox);
    hBoxLayout->addWidget(indentLabel);
    hBoxLayout->addWidget(indentSpinBox);
    hBoxLayout->addWidget(resultButtonBox);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(splitter);
    vLayout->addLayout(hBoxLayout);
    setLayout(vLayout);

    connect(m_stringTemplate, &StringTemplate::errorOccurred, this, &TemplateEditor::onErrorOccurred,
            Qt::QueuedConnection);
    connect(templateButtonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this,
            &TemplateEditor::onApplyTemplate);
    connect(templateButtonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this,
            &TemplateEditor::onSaveTemplateAs);
    connect(templateButtonBox->button(QDialogButtonBox::Open), &QPushButton::clicked, this,
            &TemplateEditor::onOpenTemplate);
    connect(templateButtonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this,
            &TemplateEditor::onHelpRequested);
    connect(validateXMLCheckBox, &QCheckBox::toggled, this, &TemplateEditor::onValidateXMLToggled);
    connect(validateXMLCheckBox, &QCheckBox::toggled, indentSpinBox, &QSpinBox::setEnabled);
    connect(indentSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &TemplateEditor::onIndentChanged);
    connect(resultButtonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this,
            &TemplateEditor::onSaveResult);
    connect(resultButtonBox, &QDialogButtonBox::rejected, this, &TemplateEditor::reject);
}

/**
 * @brief PreviewDialog::parse generates ahd shows XML text document
 * @param grouppedObjects groupped objects which are used as replacement in template
 * @param templateFileName name of template file
 */
bool TemplateEditor::parseTemplate(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName)
{
    m_grouppedObjects = grouppedObjects;
    m_templateFileName = templateFileName;

    open();
    return parseTemplate();
}

/**
 * @brief PreviewDialog::saveResultToFile saves parsed text to a file
 * @param fileName file name
 * @return whether the file is saved
 */
bool TemplateEditor::saveResultToFile(const QString &fileName)
{
    bool saved(false);
    QFile outputFile(fileName);
    if (outputFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        QTextStream stream(&outputFile);
        stream << resultText();
        m_outFileName = fileName;
        saved = true;
    } else {
        qWarning() << "Can't open file for writing:" << fileName << outputFile.errorString();
    }

    Q_EMIT fileSaved(fileName, saved);
    return saved;
}

/**
 * @brief PreviewDialog::text returns generated text (and can be formatted XML)
 * @return result text
 */
QString TemplateEditor::resultText() const
{
    return m_resultTextEdit->toPlainText();
}

/**
 * @brief PreviewDialog::onApplyTemplate saves template(s) and then parses them
 */
void TemplateEditor::onApplyTemplate()
{
    for (int tabIndex = 0; tabIndex < m_templatesTabWidget->count(); ++tabIndex) {
        QPlainTextEdit *templateTextEdit = static_cast<QPlainTextEdit *>(m_templatesTabWidget->widget(tabIndex));

        const QString &templateFileName = m_templatesTabWidget->tabText(tabIndex);
        const QString &templateFilePath = m_openedTemplates.value(templateFileName);
        QFile file(templateFilePath);
        if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
            onErrorOccurred(tr("Unable to open template file: %1").arg(templateFilePath));
            return;
        }

        QTextStream stream(&file);
        stream << templateTextEdit->toPlainText();
    }

    parseTemplate();
}

/**
 * @brief PreviewDialog::onSaveTemplateAs saves the current template into a new file
 */
void TemplateEditor::onSaveTemplateAs()
{
    const QString &templateFileName = m_templatesTabWidget->tabText(m_templatesTabWidget->currentIndex());
    const QString &templateFilePath = m_openedTemplates.value(templateFileName);

    const QString &newTemplateFileName = QFileDialog::getSaveFileName(this, tr("Save template as"), templateFilePath);
    if (newTemplateFileName.isEmpty())
        return;

    QFile file(newTemplateFileName);
    if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        onErrorOccurred(tr("Unable to open template file: %").arg(newTemplateFileName));
        return;
    }

    QPlainTextEdit *templateTextEdit = static_cast<QPlainTextEdit *>(m_templatesTabWidget->currentWidget());
    QTextStream stream(&file);
    stream << templateTextEdit->toPlainText();
}

/**
 * @brief PreviewDialog::onOpenTemplate opens a new template file
 */
void TemplateEditor::onOpenTemplate()
{
    const QString &newTemplateFileName =
            QFileDialog::getOpenFileName(this, tr("Choose a template file"), m_templateFileName);
    if (newTemplateFileName.isEmpty())
        return;

    parseTemplate(m_grouppedObjects, newTemplateFileName);
}

/**
 * @brief PreviewDialog::onHelpRequested shows help dialog
 */
void TemplateEditor::onHelpRequested()
{
    if (!m_helpDialog) {
        m_helpDialog = new TemplateSyntaxHelpDialog(this);
        m_helpDialog->setMinimumSize(m_resultTextEdit->size());
        QPoint pos = m_resultTextEdit->mapToGlobal(QPoint(0, 0));
        pos.setY(pos.y() - geometry().y());
        m_helpDialog->move(pos);
    }
    m_helpDialog->exec();
}

/**
 * @brief PreviewDialog::onSaveResult saves result test into a file
 */
void TemplateEditor::onSaveResult()
{
    const QString &outputFileName =
            QFileDialog::getSaveFileName(this, tr("Save result to file"), m_outFileName, QStringLiteral("*.xml"));
    if (outputFileName.isEmpty())
        return;

    saveResultToFile(outputFileName);
}

/**
 * @brief PreviewDialog::onErrorOccurred shows error message
 * if any error is occurred during generating of XML document
 * @param error
 */
void TemplateEditor::onErrorOccurred(const QString &errorString)
{
    QMessageBox::warning(this, tr("Error occurred"), errorString);
}

/**
 * @brief PreviewDialog::onValidateXMLToggled sets validate XML flag and parses template text
 * @param validate
 */
void TemplateEditor::onValidateXMLToggled(bool validate)
{
    m_stringTemplate->setNeedValidateXMLDocument(validate);
    if (validate)
        onApplyTemplate();
}

/**
 * @brief PreviewDialog::onIndentChanged sets a new indent and reformats XML text document
 * @param value new indent value
 */
void TemplateEditor::onIndentChanged(int value)
{
    m_stringTemplate->setAutoFormattingIndent(value);
    const QString &result = m_stringTemplate->formatText(m_resultTextEdit->toPlainText());
    m_resultTextEdit->setPlainText(result);
}

/**
 * @brief PreviewDialog::addTemplateEditor creates and adds a new template editor
 * @param tabLabel label of tab
 * @return a new template editor
 */
QPlainTextEdit *TemplateEditor::addTemplateEditor(const QString &tabLabel)
{
    QPlainTextEdit *templateTextEdit = new QPlainTextEdit();
    templateTextEdit->setWordWrapMode(QTextOption::NoWrap);
    templateTextEdit->setFont(m_resultTextEdit->font());

    new TemplateHighlighter(templateTextEdit->document());

    m_templatesTabWidget->addTab(templateTextEdit, tabLabel);
    return templateTextEdit;
}

/**
 * @brief PreviewDialog::parseTemplate opens and parses template file
 * @return whether file is parsed successfully
 */
bool TemplateEditor::parseTemplate()
{
    bool parsed(false);

    m_openedTemplates.clear();

    const bool updatesWereEnabled = m_templatesTabWidget->updatesEnabled();
    m_templatesTabWidget->setUpdatesEnabled(false);

    int tabIndex = m_templatesTabWidget->count();
    while (m_templatesTabWidget->count() > 1)
        m_templatesTabWidget->removeTab(tabIndex--);

    QFile templateFile(m_templateFileName);
    if (!templateFile.open(QFile::ReadOnly | QFile::Text)) {
        const QString wrn =
                tr("Unable to open template file: % - %2").arg(m_templateFileName, templateFile.errorString());
        qWarning() << wrn;
        onErrorOccurred(wrn);
    } else {
        QPlainTextEdit *templateTextEdit = static_cast<QPlainTextEdit *>(m_templatesTabWidget->widget(0));
        const QString templateText = templateFile.readAll();
        templateTextEdit->setPlainText(templateText);

        const QFileInfo fileInfo(templateFile);
        m_openedTemplates[fileInfo.fileName()] = fileInfo.absoluteFilePath();
        m_templatesTabWidget->setTabText(0, fileInfo.fileName());

        openIncludedTemplates(templateText);

        QByteArray result;
        QBuffer buffer(&result);
        buffer.open(QIODevice::WriteOnly | QIODevice::Text);

        if (m_stringTemplate->parseFile(m_grouppedObjects, m_templateFileName, &buffer)) {
            m_resultTextEdit->setPlainText(QString::fromUtf8(result));
            parsed = true;
        }
    }

    m_templatesTabWidget->setUpdatesEnabled(updatesWereEnabled);

    return parsed;
}

/**
 * @brief PreviewDialog::openIncludedTemplates opens all included template files in template
 * @param templateText template text
 */
void TemplateEditor::openIncludedTemplates(const QString &templateText)
{
    QFileInfo fileInfo(m_templateFileName);

    QRegularExpression expression(QStringLiteral("(?<=\\b(include|extends)\\s\")\\w+\\.?\\w*(?=\")"));
    QRegularExpressionMatchIterator it = expression.globalMatch(templateText);

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        const QString templateName = match.captured();
        if (m_openedTemplates.contains(templateName))
            continue;

        QFile file(fileInfo.absolutePath() + QLatin1Char('/') + templateName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            m_openedTemplates[templateName] = file.fileName();
            QPlainTextEdit *includedTemplateTextEdit = addTemplateEditor(templateName);
            const QString &includedTemplateText = file.readAll();
            includedTemplateTextEdit->setPlainText(includedTemplateText);
            openIncludedTemplates(includedTemplateText);
        } else {
            onErrorOccurred(tr("Unable to open included template file: %").arg(templateName));
        }
    }
}

}
