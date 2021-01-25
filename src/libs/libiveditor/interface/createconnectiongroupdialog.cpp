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

#include "aadlnamevalidator.h"
#include "ui_createconnectiongroupdialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QPlainTextEdit>
#include <QSignalMapper>

namespace ive {

CreateConnectionGroupDialog::CreateConnectionGroupDialog(
        const QList<ivm::AADLConnectionGroup::CreationInfo> &groupCreationDataList, QWidget *parent)
    : QDialog(parent)
    , m_info(groupCreationDataList)
    , m_signalMapper(new QSignalMapper(this))
{
    connect(m_signalMapper, qOverload<int>(&QSignalMapper::mapped), this, [this](int idx) {
        if (const auto textEdit = qobject_cast<QPlainTextEdit *>(m_signalMapper->mapping(idx))) {
            m_info[idx].name = textEdit->toPlainText().trimmed();
        }
    });

    auto labelConnection = [](const QList<QPointer<ivm::AADLConnection>> connections) -> QString {
        QStringList result;
        for (const auto &connection : connections) {
            if (connection) {
                const QString sourceName =
                        ivm::AADLNameValidator::decodeName(ivm::AADLObject::Type::Function, connection->sourceName());
                const QString sourceInterfaceName = ivm::AADLNameValidator::decodeName(
                        ivm::AADLObject::Type::RequiredInterface, connection->sourceInterfaceName());
                const QString targetName =
                        ivm::AADLNameValidator::decodeName(ivm::AADLObject::Type::Function, connection->targetName());
                const QString targetInterfaceName = ivm::AADLNameValidator::decodeName(
                        ivm::AADLObject::Type::ProvidedInterface, connection->targetInterfaceName());
                result.append(QStringLiteral("%1.%2 <-> %3.%4")
                                      .arg(sourceName, sourceInterfaceName, targetName, targetInterfaceName));
            }
        }
        return result.join(QLatin1Char('\n'));
    };

    auto formLayout = new QFormLayout;
    for (int idx = 0; idx < groupCreationDataList.size(); ++idx) {
        const ivm::AADLConnectionGroup::CreationInfo &info = groupCreationDataList.value(idx);
        auto textEdit = new QPlainTextEdit(this);
        connect(textEdit, &QPlainTextEdit::textChanged, m_signalMapper, qOverload<>(&QSignalMapper::map));
        m_signalMapper->setMapping(textEdit, idx);
        formLayout->addRow(labelConnection(info.connections), textEdit);
    }
    auto box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto rootLayout = new QVBoxLayout;
    rootLayout->addLayout(formLayout);
    rootLayout->addWidget(box);
    setLayout(rootLayout);
    resize(sizeHint().width(), minimumSizeHint().height());
}

CreateConnectionGroupDialog::~CreateConnectionGroupDialog() { }

QList<ivm::AADLConnectionGroup::CreationInfo> CreateConnectionGroupDialog::info() const
{
    return m_info;
}

} // namespace ive
