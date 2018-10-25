/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainmodel.h"

#include <documentitemmodel.h>
#include <mscchart.h>

#include <QApplication>
#include <QComboBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QItemSelectionModel>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_model(new MainModel(this))
{
    setupUi();

    connect(ui->action_Open_file, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionQuit, &QAction::triggered, QApplication::instance(), &QApplication::quit);
    connect(ui->actionAbout_Qt, &QAction::triggered, QApplication::instance(), &QApplication::aboutQt);

    ui->graphicsView->setScene(m_model->graphicsScene());
    ui->documentTreeView->setModel(m_model->documentItemModel());
    connect(ui->documentTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::showSelection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("MSC"), QString("../../msceditor/examples"), QString("*.msc"));
    if (!filename.isEmpty()) {
        bool ok = m_model->loadFile(filename);
        if (ok) {
            ui->documentTreeView->expandAll();
            QFileInfo fileInfo(filename);
            setWindowTitle(tr("MSC Editor") + " - " + fileInfo.fileName());
        } else {
            setWindowTitle(tr("MSC Editor"));
        }
    }
}

void MainWindow::showSelection(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (!current.isValid()) {
        return;
    }

    auto *obj = static_cast<QObject *>(current.internalPointer());
    auto chart = dynamic_cast<msc::MscChart *>(obj);

    if (chart) {
        m_model->fillView(chart);
    }
}

void MainWindow::setupUi()
{
    ui->setupUi(this);

    // toolbar
    ui->action_Open_file->setIcon(this->style()->standardIcon(QStyle::SP_DirOpenIcon));

    // status bar
    auto zoomBox = new QComboBox(ui->statusBar);
    zoomBox->addItem(" 50 %");
    zoomBox->addItem("100 %");
    zoomBox->addItem("200 %");
    zoomBox->setCurrentIndex(1);
    connect(zoomBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&](int index) {
        double percent = 100.0;
        if (index == 0) {
            percent = 50.0;
        }
        if (index == 2) {
            percent = 200.0;
        }
        ui->graphicsView->setZoom(percent);
    });
    statusBar()->addWidget(zoomBox);
}
