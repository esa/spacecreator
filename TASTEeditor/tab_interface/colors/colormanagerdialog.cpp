/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "colormanagerdialog.h"

#include "baseitems/common/utils.h"
#include "ui_colormanagerdialog.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace taste3 {
namespace aadl {

ColorManagerDialog::ColorManagerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ColorManagerDialog)
    , m_namesModel(new QStringListModel(this))
    , m_color { nullptr }
{
    ui->setupUi(this);
    setWindowTitle(tr("Color scheme"));

    connect(ui->listView, &QListView::clicked, this, &ColorManagerDialog::onColorHandlerSelected);

    loadFile(ColorManager::instance()->sourceFile());
}

ColorManagerDialog::~ColorManagerDialog()
{
    delete ui;
}

void ColorManagerDialog::loadFile(const QString &path)
{
    if (!QFile::exists(path)) {
        qWarning() << "File not exists" << path;
        return;
    }

    QFile jsonFile(path);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Open file filed:" << path << jsonFile.errorString();
        return;
    }

    const QByteArray &jsonData = jsonFile.readAll();
    QJsonParseError jsonErrHandler;
    const QJsonDocument &jsonDoc = QJsonDocument::fromJson(jsonData, &jsonErrHandler);
    if (jsonErrHandler.error != QJsonParseError::NoError) {
        qWarning() << "JSON parsing failed:" << jsonErrHandler.errorString();
        return;
    }

    const QJsonArray &jArr = jsonDoc.array();
    for (const QJsonValue &jsonVal : jArr) {
        const QJsonObject &jsonObj = jsonVal.toObject();
        const ColorManager::HandledColors colorType =
                ColorManager::HandledColors(jsonObj["color_type"].toInt(ColorManager::HandledColors::Unhandled));
        if (colorType != ColorManager::HandledColors::Unhandled) {
            m_colors[colorType] = ColorHandler::fromJson(jsonObj);
            m_colorNames[ColorManager::handledColorTypeName(colorType)] = colorType;
        }
    }

    m_namesModel->setStringList(m_colorNames.keys());
    ui->lePath->setText(ColorManager::instance()->sourceFile());
    ui->listView->setModel(m_namesModel);
}

void ColorManagerDialog::onColorHandlerSelected(const QModelIndex &id)
{
    m_color = nullptr;
    if (id.isValid()) {
        const QString &name = id.data().toString();
        if (m_colorNames.contains(name))
            m_color = &m_colors[m_colorNames[name]];
    }

    ui->colorHandlerEditor->setColorHandler(m_color);
}

void ColorManagerDialog::on_btnOpen_clicked()
{
    const QString file =
            QFileDialog::getOpenFileName(this, tr("Choose color scheme file"), ui->lePath->text(), "*.json");
    if (QFile::exists(file))
        loadFile(file);
}

void ColorManagerDialog::on_btnCreateNew_clicked()
{
    const QString file =
            QFileDialog::getSaveFileName(this, tr("Choose color scheme file"), ui->lePath->text(), "*.json");
    if (!file.isEmpty() && utils::copyResourceFile(":/colors/default_colors.json", file))
        loadFile(file);
}

void ColorManagerDialog::accept()
{
    const QString &filePath = ui->lePath->text();

    QJsonArray ja;
    for (auto ct : m_colors.keys()) {
        ColorHandler ch = m_colors[ct];
        QJsonObject jObj = ch.toJson();
        jObj["color_type"] = ct;
        ja.append(jObj);
    }

    QFile out(filePath);
    out.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    out.write(QJsonDocument(ja).toJson());
    out.close();

    ColorManager::instance()->setSourceFile(filePath);

    QDialog::accept();
}

} // ns aadl
} // ns taste3
