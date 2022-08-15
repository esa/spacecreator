/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "filedialogdelegate.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

namespace ive {

FileDialogDelegate::FileDialogDelegate(
        const QString &defaultDirectory, const QString &caption, const QString &filter, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_defaultDirectory(defaultDirectory)
    , m_caption(caption)
    , m_filter(filter)
{
}

QWidget *FileDialogDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        auto fileDialog = new QFileDialog(parent, m_caption, m_defaultDirectory, m_filter);
        fileDialog->setMinimumSize(QSize(540, 480));
        fileDialog->setFileMode(QFileDialog::ExistingFile);
        return fileDialog;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void FileDialogDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto fileDialog = qobject_cast<QFileDialog *>(editor);
    if(fileDialog == nullptr){
        return;
    }

    QStringList fileName = fileDialog->selectedFiles();
    if(!fileName.isEmpty()){
        QFileInfo fileInfo(fileName[0]);
        model->setData(index, fileInfo.fileName());
    }
}

} // namespace ive
