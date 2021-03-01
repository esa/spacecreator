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

#include "colorsettingswidget.h"

#include "common.h"
#include "ui_colorsettingswidget.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QStringListModel>

namespace shared {

ColorSettingsWidget::ColorSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ColorSettingsWidget)
    , m_namesModel(new QStringListModel(this))
    , m_originalFile(ColorManager::instance()->sourceFile())
{
    ui->setupUi(this);
    ui->listView->setModel(m_namesModel);

    connect(ui->listView->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            &ColorSettingsWidget::onColorHandlerSelected);
    connect(ui->colorHandlerEditor, &ColorHandlerEditor::colorHandlerValueChanged, ColorManager::instance(),
            &ColorManager::colorsUpdated);

    openFile(ColorManager::instance()->sourceFile());
}

ColorSettingsWidget::~ColorSettingsWidget()
{
    delete ui;
}

void ColorSettingsWidget::saveSettings()
{
    const QString &filePath = ui->lePath->text();

    const bool ok = ColorManager::instance()->save(filePath);
    if (!ok) {
        QMessageBox::warning(this, tr("Error saving"), tr("Unable to save file %1").arg(filePath));
        return;
    }

    ColorManager::instance()->setSourceFile(filePath);
}

bool ColorSettingsWidget::loadFile(const QString &path)
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

    fillModel();

    ui->listView->setCurrentIndex(ui->listView->model()->index(currentRow, 0));
    return !m_colorNames.empty();
}

void ColorSettingsWidget::openFile(const QString &path)
{
    if (loadFile(path))
        ui->lePath->setText(path);
}

void ColorSettingsWidget::setFilterGroup(const QString &group)
{
    if (group == m_filterGroup) {
        return;
    }

    m_filterGroup = group;
    fillModel();
}

void ColorSettingsWidget::onColorHandlerSelected(const QModelIndex &id)
{
    if (id.isValid()) {
        const QString &name = id.data().toString();
        m_color = ColorManager::instance()->colorsForItem(m_colorNames.value(name));
        ui->colorHandlerEditor->setColorHandler(&m_color);
    } else {
        ui->colorHandlerEditor->setColorHandler(nullptr);
    }
}

void ColorSettingsWidget::on_btnOpen_clicked()
{
    const QString file =
            QFileDialog::getOpenFileName(this, tr("Choose color scheme file"), ui->lePath->text(), "*.json");
    openFile(file);
}

void ColorSettingsWidget::on_btnCreateNew_clicked()
{
    QFileDialog dialog(this, tr("Choose color scheme file"), ui->lePath->text(), "*.json");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(".json");
    if (dialog.exec() == QDialog::Accepted) {
        const QString file = dialog.selectedUrls().value(0).toLocalFile();
        if (shared::copyResourceFile(
                    ColorManager::defaultColorsResourceFile(), file, shared::FileCopyingMode::Overwrite)) {
            openFile(file);
        }
    }
}

void ColorSettingsWidget::fillModel()
{
    QStringList items;
    for (auto it = m_colorNames.begin(); it != m_colorNames.end(); ++it) {
        ColorHandler color = ColorManager::instance()->colorsForItem(it.value());
        if (m_filterGroup.isEmpty() || color.group() == m_filterGroup) {
            items.append(it.key());
        }
    }
    m_namesModel->setStringList(items);
}

}
