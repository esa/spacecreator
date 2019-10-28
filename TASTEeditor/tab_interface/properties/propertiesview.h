#pragma once

#include <QWidget>

namespace Ui {
class PropertiesView;
}

class QTableView;

namespace taste3 {
namespace aadl {

class PropertiesListModel;
class PropertiesView : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesView(QWidget *parent = nullptr);
    ~PropertiesView();

    void setModel(PropertiesListModel* model);
    QTableView* tableView() const;

private slots:
    void onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_btnAdd_clicked();
    void on_btnDel_clicked();

private:
    Ui::PropertiesView *ui;
    PropertiesListModel* m_model;
};

} // namespace aadl
} // namespace taste3
