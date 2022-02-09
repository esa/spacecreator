/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "listtreedialog.h"

#include <QStandardItemModel>
#include <QVBoxLayout>

ListTreeDialog::ListTreeDialog()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    m_tree = new QTreeView(this);
    layout->addWidget(m_tree);

    m_button = new QPushButton(this);
    m_button->setText("select");
    layout->addWidget(m_button);
}

QPushButton *ListTreeDialog::button()
{
    return m_button;
}

QList<QString> *ListTreeDialog::selectedItems()
{
    return &m_selectedItems;
}

QStandardItemModel *ListTreeDialog::model()
{
    return m_model;
}

void ListTreeDialog::setModel(QStandardItemModel *const model)
{
    m_model = model;
    m_tree->setModel(m_model);
}

void ListTreeDialog::setButtonText(const QString &buttonText)
{
    m_button->setText(buttonText);
}

void ListTreeDialog::setButtonHandler(const std::function<void()> &func)
{
    QObject::connect(m_button, &QPushButton::pressed, func);
}
