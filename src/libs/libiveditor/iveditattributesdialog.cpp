/*
   Copyright (C) 2022-2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "interface/attributedelegate.h"
#include "iveditattributesdialog.h"
#include "iveditattributesmodel.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QTabWidget>
#include <QTableView>
#include <QVBoxLayout>

class AttributeDelegate : public shared::AttributeDelegate {
public:
    using shared::AttributeDelegate::AttributeDelegate;

    QString tokenNameFromIndex(const QModelIndex &index) const override
    {
        return index.model()->headerData(index.column(), Qt::Horizontal, Qt::UserRole).toString();
    }
};

namespace ive
{

struct IVEditAttributesDialog::Private {
    QTabWidget *tabWidget;
    QTableView *functionsTable;
    QTableView *interfacesTable;
};

IVEditAttributesDialog::IVEditAttributesDialog(QAbstractItemModel *functionsModel, QAbstractItemModel *interfacesModel, QWidget *parent)
    : QDialog(parent)
    , d(new Private)
{
    auto layout = new QVBoxLayout(this);
    setLayout(layout);
    resize(640, 480);

    d->tabWidget = new QTabWidget(this);
    layout->addWidget(d->tabWidget);

    auto buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QPushButton* button =  buttonBox->addButton(tr("Save Attributes..."), QDialogButtonBox::ActionRole);
    connect(button, &QPushButton::clicked, this, [this]{
        auto dialog = new QFileDialog(this);
        connect(dialog, &QDialog::finished, dialog, &QObject::deleteLater);
        connect(dialog, &QFileDialog::fileSelected, this, &IVEditAttributesDialog::saveAttributes);
        dialog->setAcceptMode(QFileDialog::AcceptSave);
        dialog->setNameFilter(tr("JSON files (*.json)"));
        dialog->setModal(true);
        dialog->show();
    });

    button = buttonBox->addButton(tr("Load Attributes..."), QDialogButtonBox::ActionRole);
    connect(button, &QPushButton::clicked, this, [this]{
        auto dialog = new QFileDialog(this);
        connect(dialog, &QDialog::finished, dialog, &QObject::deleteLater);
        connect(dialog, &QFileDialog::fileSelected, this, &IVEditAttributesDialog::loadAttributes);
        dialog->setFileMode(QFileDialog::ExistingFile);
        dialog->setNameFilter(tr("JSON files (*.json)"));
        dialog->setModal(true);
        dialog->show();
    });

    layout->addWidget(buttonBox);

    d->functionsTable = new QTableView(d->tabWidget);
    d->functionsTable->setItemDelegate(new AttributeDelegate(d->functionsTable));
    d->functionsTable->setModel(functionsModel);

    d->interfacesTable = new QTableView(d->tabWidget);
    d->interfacesTable->setItemDelegate(new AttributeDelegate(d->interfacesTable));
    d->interfacesTable->setModel(interfacesModel);

    d->tabWidget->addTab(d->functionsTable, tr("Functions' attributes"));
    d->tabWidget->addTab(d->interfacesTable, tr("Interfaces' attributes"));

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

IVEditAttributesDialog::~IVEditAttributesDialog()
{
    delete d;
    d = nullptr;
}

void IVEditAttributesDialog::saveAttributes(const QString &filePath)
{
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);

    auto functionsModel = static_cast<IVEditAttributesModel*>(d->functionsTable->model());
    auto interfacesModel = static_cast<IVEditAttributesModel*>(d->interfacesTable->model());

    QJsonObject jsonRoot;
    jsonRoot.insert("functions", functionsModel->saveAttributes());
    jsonRoot.insert("interfaces", interfacesModel->saveAttributes());

    const QJsonDocument document(jsonRoot);
    file.write(document.toJson(QJsonDocument::Indented));
}

void IVEditAttributesDialog::loadAttributes(const QString &filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);

    const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    if (!document.isObject()) {
        qWarning("Cannot load JSON: root not an object");
        return;
    }

    auto functionsModel = static_cast<IVEditAttributesModel*>(d->functionsTable->model());
    auto interfacesModel = static_cast<IVEditAttributesModel*>(d->interfacesTable->model());

    const QJsonObject root = document.object();
    functionsModel->loadAttributes(root.value("functions").toArray());
    interfacesModel->loadAttributes(root.value("interfaces").toArray());
}

}
