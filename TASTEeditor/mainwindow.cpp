/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::init()
{
    ui->setupUi(this);

    initMenus();
}

void MainWindow::initMenus()
{
    initMenuFile();
    initMenuHelp();
}

void MainWindow::initMenuFile()
{
    m_menuFile = menuBar()->addMenu(tr("&File"));
    m_actOpenFile = m_menuFile->addAction(tr("Open"), this, &MainWindow::onOpenFileRequested, QKeySequence::Open);
    m_actCreateFile = m_menuFile->addAction(tr("Create"), this, &MainWindow::onCreateFileRequested, QKeySequence::New);
    m_actCloseFile = m_menuFile->addAction(tr("Close"), this, &MainWindow::onCloseFileRequested, QKeySequence::Close);
    m_menuFile->addSeparator();
    m_actQuit = m_menuFile->addAction(tr("Quit"), this, &MainWindow::onQuitRequested, QKeySequence::Quit);
}

void MainWindow::initMenuHelp()
{
    m_menuHelp = menuBar()->addMenu(tr("&Help"));
    m_actAbout = m_menuHelp->addAction(tr("About"), this, &MainWindow::onAboutRequested, QKeySequence::HelpContents);
}

void MainWindow::onOpenFileRequested()
{
    showNIY(Q_FUNC_INFO);
}

void MainWindow::onCreateFileRequested()
{
    showNIY(Q_FUNC_INFO);
}

bool MainWindow::onCloseFileRequested()
{
    showNIY(Q_FUNC_INFO);
    return true;
}

void MainWindow::onQuitRequested()
{
    showNIY(Q_FUNC_INFO);

    //    while(hasOpenDocs())
    //        if(!onCloseFileRequested())
    //            return;
}

void MainWindow::onAboutRequested()
{
    QString info = qApp->applicationName();

#ifdef VS_BUILD_HASH
    info += QString("@%1").arg(VS_BUILD_HASH);
#endif

    QMessageBox::information(this, tr("About"), info);
}

void MainWindow::showNIY(const QString &caller)
{
    const QString &message = QString("Not implemented yet:\n%1").arg(caller);
    qDebug() << message;
    QMessageBox::information(this, "NIY", message);
}
