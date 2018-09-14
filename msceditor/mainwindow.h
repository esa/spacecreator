#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openFile();

private:
    void setupUi();

    Ui::MainWindow *ui = nullptr;
    MainModel *m_model = nullptr;
};

#endif // MAINWINDOW_H
