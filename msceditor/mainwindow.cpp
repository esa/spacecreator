#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainmodel.h"

#include <QApplication>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_model(new MainModel(this))
{
    ui->setupUi(this);

    connect(ui->action_Open_file, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionQuit, &QAction::triggered, QApplication::instance(), &QApplication::quit);
    connect(ui->actionAbout_Qt, &QAction::triggered, QApplication::instance(), &QApplication::aboutQt);

    ui->graphicsView->setScene(m_model->graphicsScene());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("MSC"), QString("../../msceditor/examples"), QString("*.msc"));
    if (!filename.isEmpty()) {
        m_model->loadFile(filename);
    }
}
