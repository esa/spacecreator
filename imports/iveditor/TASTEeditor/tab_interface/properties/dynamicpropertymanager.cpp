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

#include "dynamicpropertymanager.h"

#include "adddynamicpropertydialog.h"
#include "dynamicproperty.h"
#include "dynamicpropertyconfig.h"
#include "ui_dynamicpropertymanager.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
namespace taste3 {
namespace aadl {

DynamicPropertyManager::DynamicPropertyManager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DynamicPropertyManager)
{
    ui->setupUi(this);

    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &DynamicPropertyManager::updateErrorInfo);

    readJson(DynamicPropertyConfig::currentConfigPath());
}

DynamicPropertyManager::~DynamicPropertyManager()
{
    delete ui;
}

bool DynamicPropertyManager::readJson(const QString &from)
{
    if (from.isEmpty())
        return false;

    QFile jsonFile(from);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "File opening failed:" << from << jsonFile.errorString();
        return false;
    }

    ui->plainTextEdit->setPlainText(jsonFile.readAll());
    return true;
}

void DynamicPropertyManager::updateErrorInfo()
{
    QJsonParseError errorHandler;
    const QByteArray &jsonData = ui->plainTextEdit->toPlainText().toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &errorHandler);

    QColor textColor(Qt::black);
    QString errorText;
    bool success(true);
    m_usedNames.clear();

    if (!jsonData.isEmpty()) {
        success = errorHandler.error == QJsonParseError::NoError;
        if (!success) {
            errorText = errorHandler.errorString();
            qWarning() << errorText;
            textColor = Qt::red;
        } else {
            const QVector<DynamicProperty *> &attrs = DynamicPropertyConfig::parseAttributesList(jsonData);
            const int jsonArraySize = doc.array().size();
            success = jsonArraySize == attrs.size();

            if (success) {
                for (DynamicProperty *attr : attrs) {
                    if (m_usedNames.contains(attr->name())) {
                        errorText = tr("Duplicate names found: %1").arg(attr->name());
                        success = false;
                        break;
                    }
                    m_usedNames.append(attr->name());
                }
            }

            if (!success) {
                if (errorText.isEmpty())
                    errorText = "Please remove incomplete properties";
                qWarning() << "Properties stored (JSON):" << jsonArraySize << "; parsed:" << attrs.size();
                textColor = Qt::red;
            }
        }
    }

    ui->errorDisplay->setText(errorText);
    setTextColor(textColor);
    ui->btnNewProp->setEnabled(success || jsonData.isEmpty());
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(success);
}

void DynamicPropertyManager::setTextColor(const QColor &color)
{
    QPalette p(ui->plainTextEdit->palette());
    p.setColor(QPalette::Text, color);
    ui->plainTextEdit->setPalette(p);
}

void DynamicPropertyManager::on_btnNewProp_clicked()
{
    AddDynamicPropertyDialog *dlg = new AddDynamicPropertyDialog(m_usedNames, this);
    if (dlg->exec() == QDialog::Accepted) {
        if (DynamicProperty *attr = dlg->attribute()) {
            const QByteArray &jsonData = ui->plainTextEdit->toPlainText().toUtf8();
            QJsonArray jArr = QJsonDocument::fromJson(jsonData).array();
            jArr.append(attr->toJson());
            ui->plainTextEdit->setPlainText(QJsonDocument(jArr).toJson());
            ui->plainTextEdit->find(QString("\"name\": \"%1\",").arg(attr->name()));
        }
    }
    delete dlg;
}

void DynamicPropertyManager::accept()
{
    const QString &filePath = DynamicPropertyConfig::currentConfigPath();
    QFile out(filePath);
    if (out.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        const QByteArray &content = ui->plainTextEdit->toPlainText().toUtf8();
        out.write(content);
        out.close();
        DynamicPropertyConfig::instance()->init();
    } else {
        const QString warn(tr("Can't save file %1 - %2").arg(filePath, out.errorString()));
        qWarning() << warn;

        QMessageBox::warning(this, tr("Saving..."), warn);
    }

    QDialog::accept();
}

} // ns aadl
} // ns taste3
