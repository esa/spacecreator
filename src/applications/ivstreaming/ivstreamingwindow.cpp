/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivstreamingwindow.h"

#include "interfacedocument.h"
#include "ui_ivstreamingwindow.h"

#include <QApplication>

namespace iv {

IVStreamingWindow::IVStreamingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IVStreamingWindow)
    , m_document(new ive::InterfaceDocument(this))
{
    ui->setupUi(this);
    ui->graphicsView->setInteractive(false);
    ui->graphicsView->setScene(m_document->scene());
    m_document->load("./interfaceview.xml");
}

IVStreamingWindow::~IVStreamingWindow()
{
    delete ui;
}

/**
 * Starts the websocket for remote control at the port \p
 * Default port is 34633
 */
bool IVStreamingWindow::startRemoteControl(quint16 port)
{
    return true;
}

void IVStreamingWindow::closeEvent(QCloseEvent *e)
{
    QMainWindow::closeEvent(e);
    QApplication::quit();
}

}
