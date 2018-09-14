#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainmodel.h"

#include <QApplication>
#include <QComboBox>
#include <QFileDialog>

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
    statusBar()
            ->addWidget(zoomBox);
}
