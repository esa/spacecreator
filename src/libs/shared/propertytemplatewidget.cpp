/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "propertytemplatewidget.h"

#include "addpropertytemplatedialog.h"
#include "errorhub.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"
#include "xmlhighlighter.h"

#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStandardPaths>

namespace shared {

PropertyTemplateWidget::PropertyTemplateWidget(QWidget *parent)
    : QWidget(parent)
{
    auto gridLayout = new QGridLayout(this);
    m_plainTextEdit = new QPlainTextEdit(this);
    new shared::XMLHighlighter(m_plainTextEdit->document());
    connect(m_plainTextEdit, &QPlainTextEdit::textChanged, this, &PropertyTemplateWidget::updateErrorInfo);
    gridLayout->addWidget(m_plainTextEdit, 0, 0, 3, 1);

    m_templateConfigTypes = new QComboBox(this);
    connect(m_templateConfigTypes, qOverload<int>(&QComboBox::currentIndexChanged), this,
            [this]() { readConfig(currentTemplateConfig()->configPath()); });
    gridLayout->addWidget(m_templateConfigTypes, 0, 1, 1, 1);

    m_btnNewProp = new QPushButton(tr("New Property"), this);
    connect(m_btnNewProp, &QPushButton::clicked, this, &PropertyTemplateWidget::createProperty);
    gridLayout->addWidget(m_btnNewProp, 1, 1, 1, 1);

    auto verticalSpacer = new QSpacerItem(20, 225, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer, 2, 1, 1, 1);

    m_errorDisplay = new QLabel(this);
    gridLayout->addWidget(m_errorDisplay, 3, 0, 1, 2);

    setLayout(gridLayout);
}

void PropertyTemplateWidget::save()
{
    if (m_error || !currentTemplateConfig()) {
        return;
    }

    const QString &filePath = currentTemplateConfig()->configPath();
    QFile out(filePath);
    if (out.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        const QByteArray &content = m_plainTextEdit->toPlainText().toUtf8();
        out.write(content);
        out.close();
        currentTemplateConfig()->init(filePath);
    } else {
        const QString warn(tr("Can't save file %1 - %2").arg(filePath, out.errorString()));
        qWarning() << warn;

        QMessageBox::warning(this, tr("Saving..."), warn);
    }
}

bool PropertyTemplateWidget::readConfig(const QString &from)
{
    if (from.isEmpty())
        return false;

    QFile configFile(from);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        shared::ErrorHub::addError(
                shared::ErrorItem::Error, tr("File opening failed: %1").arg(configFile.errorString()), from);
        return false;
    }

    m_plainTextEdit->setPlainText(configFile.readAll());
    return true;
}

void PropertyTemplateWidget::setTextColor(const QColor &color)
{
    QPalette p(m_plainTextEdit->palette());
    p.setColor(QPalette::Text, color);
    m_plainTextEdit->setPalette(p);
}

bool PropertyTemplateWidget::hasError() const
{
    return m_error;
}

void PropertyTemplateWidget::setPropertyTemplateConfigs(const QList<PropertyTemplateConfig *> &propTemplateConfigs)
{
    m_templateConfigTypes->clear();
    m_propTemplateConfigs = propTemplateConfigs;
    for (auto config : propTemplateConfigs) {
        m_templateConfigTypes->addItem(config->title());
    }
    m_templateConfigTypes->setCurrentIndex(0);
}

void PropertyTemplateWidget::updateErrorInfo()
{
    if (!currentTemplateConfig()) {
        qCritical() << "There is no valid attributes template config";
        return;
    }

    const QString &xmlData = m_plainTextEdit->toPlainText();

    QColor textColor(Qt::black);
    m_usedNames.clear();

    QString errorMsg;
    int errorLine = -1;
    int errorColumn = -1;

    if (!xmlData.isEmpty()) {
        const QList<shared::PropertyTemplate *> &attrs =
                currentTemplateConfig()->parseAttributesList(xmlData, &errorMsg, &errorLine, &errorColumn);

        if (!errorMsg.isEmpty()) {
            qWarning() << errorMsg << errorLine << errorColumn;
            textColor = Qt::red;
        } else {
            for (shared::PropertyTemplate *attr : attrs) {
                if (m_usedNames.contains(attr->name())) {
                    errorMsg = tr("Duplicate names found: %1").arg(attr->name());
                    break;
                }
                m_usedNames.append(attr->name());
            }
        }
    }

    m_errorDisplay->setText(errorMsg);
    setTextColor(textColor);
    m_btnNewProp->setEnabled(errorMsg.isEmpty() || xmlData.isEmpty());

    m_error = !errorMsg.isEmpty();
    Q_EMIT hasErrorChanged(m_error);
}

void PropertyTemplateWidget::createProperty()
{
    shared::AddPropertyTemplateDialog *dlg =
            new shared::AddPropertyTemplateDialog(currentTemplateConfig(), m_usedNames, this);
    if (dlg->exec() == QDialog::Accepted) {
        if (PropertyTemplate *attr = dlg->attribute()) {
            const QString &xmlData = m_plainTextEdit->toPlainText();
            QDomDocument doc;
            if (doc.setContent(xmlData)) {
                QDomElement rootElement = doc.documentElement();
                rootElement.appendChild(attr->toXml(&doc));

                m_plainTextEdit->setPlainText(doc.toString());
                m_plainTextEdit->find(QString("name=\"%1\"").arg(attr->name()));
            }
        }
    }
    delete dlg;
}

PropertyTemplateConfig *PropertyTemplateWidget::currentTemplateConfig() const
{
    return m_propTemplateConfigs.value(m_templateConfigTypes->currentIndex());
}

} // namespace shared
