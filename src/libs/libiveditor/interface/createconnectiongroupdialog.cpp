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

#include "createconnectiongroupdialog.h"

#include "ui_createconnectiongroupdialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSignalMapper>

namespace aadlinterface {

CreateConnectionGroupDialog::CreateConnectionGroupDialog(
        const QList<aadl::AADLObjectConnectionGroup::CreationInfo> &groupCreationDataList, QWidget *parent)
    : QDialog(parent)
    , m_info(groupCreationDataList)
    , m_signalMapper(new QSignalMapper(this))
{
    connect(m_signalMapper, qOverload<int>(&QSignalMapper::mapped), this, [this](int idx) {
        if (const auto lineEdit = qobject_cast<QLineEdit *>(m_signalMapper->mapping(idx))) {
            m_info[idx].name = lineEdit->text().trimmed();
        }
    });

    auto labelConnection = [](const QList<QPointer<aadl::AADLObjectConnection>> connections) -> QString {
        QStringList result;
        for (const auto connection : connections) {
            if (connection) {
                result.append(QStringLiteral("%1.%2 <-> %3.%4")
                                      .arg(connection->sourceName(), connection->sourceInterfaceName(),
                                              connection->targetName(), connection->targetInterfaceName()));
            }
        }
        return result.join(QLatin1String(","));
    };

    auto formLayout = new QFormLayout;
    for (int idx = 0; idx < groupCreationDataList.size(); ++idx) {
        const aadl::AADLObjectConnectionGroup::CreationInfo &info = groupCreationDataList.value(idx);
        QLineEdit *lineEdit = new QLineEdit(this);
        connect(lineEdit, &QLineEdit::textEdited, m_signalMapper, qOverload<>(&QSignalMapper::map));
        m_signalMapper->setMapping(lineEdit, idx);
        formLayout->addRow(labelConnection(info.connections), lineEdit);
    }
    auto rootLayout = new QVBoxLayout;
    rootLayout->addLayout(formLayout);

    auto box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    rootLayout->addWidget(box);
    setLayout(rootLayout);
    adjustSize();
}

CreateConnectionGroupDialog::~CreateConnectionGroupDialog() { }

QList<aadl::AADLObjectConnectionGroup::CreationInfo> CreateConnectionGroupDialog::info() const
{
    return m_info;
}

} // namespace aadlinterface
