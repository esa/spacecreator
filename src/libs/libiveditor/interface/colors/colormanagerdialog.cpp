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

#include "common.h"
#include "ui_colormanagerdialog.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>

namespace aadlinterface {

ColorManagerDialog::ColorManagerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ColorManagerDialog)
    , m_namesModel(new QStringListModel(this))
    , m_originalFile(ColorManager::instance()->sourceFile())
{
    ui->setupUi(this);
    setWindowTitle(tr("Color scheme"));
    ui->listView->setModel(m_namesModel);

    connect(ui->listView->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            &ColorManagerDialog::onColorHandlerSelected);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &ColorManagerDialog::onDialogButtonClicked);
    connect(ui->colorHandlerEditor, &ColorHandlerEditor::colorHandlerValueChanged, ColorManager::instance(),
            &ColorManager::colorsUpdated);

    openFile(ColorManager::instance()->sourceFile());
}

ColorManagerDialog::~ColorManagerDialog()
{
    delete ui;
}

bool ColorManagerDialog::loadFile(const QString &path)
{
    if (!QFile::exists(path)) {
        qWarning() << "File not exists" << path;
        return false;
    }

    if (!ColorManager::instance()->setSourceFile(path))
        return false;

    m_colorNames.clear();

    for (auto handledColor : ColorManager::instance()->handledColors())
        m_colorNames[ColorManager::handledColorTypeName(handledColor)] = handledColor;

    const int currentRow = ui->listView->currentIndex().isValid() ? ui->listView->currentIndex().row() : 0;

    m_namesModel->setStringList(m_colorNames.keys());

    ui->listView->setCurrentIndex(ui->listView->model()->index(currentRow, 0));
    return m_colorNames.size();
}

void ColorManagerDialog::onColorHandlerSelected(const QModelIndex &id)
{
    if (id.isValid()) {
        const QString &name = id.data().toString();
        m_color = ColorManager::instance()->colorsForItem(m_colorNames.value(name));
        ui->colorHandlerEditor->setColorHandler(&m_color);
    } else {
        ui->colorHandlerEditor->setColorHandler(nullptr);
    }
}

void ColorManagerDialog::on_btnOpen_clicked()
{
    const QString file =
            QFileDialog::getOpenFileName(this, tr("Choose color scheme file"), ui->lePath->text(), "*.json");
    openFile(file);
}

void ColorManagerDialog::on_btnCreateNew_clicked()
{
    const QString file =
            QFileDialog::getSaveFileName(this, tr("Choose color scheme file"), ui->lePath->text(), "*.json");
    if (shared::copyResourceFile(ColorManager::defaultColorsResourceFile(), file, shared::FileCopyingMode::Overwrite))
        openFile(file);
}

void ColorManagerDialog::onDialogButtonClicked(QAbstractButton *button)
{
    if (ui->buttonBox->button(QDialogButtonBox::RestoreDefaults) == button)
        loadFile(ColorManager::defaultColorsResourceFile());
}

void ColorManagerDialog::openFile(const QString &path)
{
    if (loadFile(path))
        ui->lePath->setText(path);
}

void ColorManagerDialog::accept()
{
    const QString &filePath = ui->lePath->text();

    QJsonArray ja;
    for (auto ct : ColorManager::instance()->handledColors()) {
        ColorHandler ch = ColorManager::instance()->colorsForItem(ct);
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

void ColorManagerDialog::reject()
{
    ColorManager::instance()->setSourceFile(m_originalFile);
    QDialog::reject();
}

}
