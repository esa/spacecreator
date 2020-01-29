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
    new XMLHighlighter(m_textEdit->document());

    QDesktopWidget *desktop = QApplication::desktop();
    QRect geometry = desktop->availableGeometry(this);
    setMinimumSize(geometry.width() * 0.8, geometry.height() * 0.8);
    m_textEdit->setReadOnly(true);

    QLabel *indentLabel = new QLabel(tr("Auto-formatting Indent:"), this);

    QSpinBox *indentSpinBox = new QSpinBox(this);
    indentSpinBox->setRange(0, 10);
    indentSpinBox->setValue(m_stringTemplate->autoFormattingIndent());

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Close);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(indentLabel);
    hBoxLayout->addWidget(indentSpinBox);
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

QString PreviewDialog::parse(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName)
{
    QString result = m_stringTemplate->parseFile(grouppedObjects, templateFileName);
    m_textEdit->setText(result);
    return exec() ? m_textEdit->toPlainText() : QString();
}

void PreviewDialog::onErrorOccurred(const QString &error)
{
    QMessageBox::warning(this, tr("Error occurred"), error);
}

void PreviewDialog::onIndentChanged(int value)
{
    m_stringTemplate->setAutoFormattingIndent(value);
    QString result = m_stringTemplate->formatText(m_textEdit->toPlainText());
    m_textEdit->setText(result);
}

} // ns processing
} // ns taste3
