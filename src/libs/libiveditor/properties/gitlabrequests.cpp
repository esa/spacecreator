#include "gitlabrequests.h"

#include "ui_gitlabrequests.h"

constexpr int REQID = 0;
constexpr int DESCRIPTION = 1;
GitlabRequests::GitlabRequests(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GitlabRequests)
{
    ui->setupUi(this);
}

GitlabRequests::~GitlabRequests()
{
    delete ui;
}

void GitlabRequests::addRequest(const QString &reqID, const QString &description)
{
    const int row = ui->RequestsTable->rowCount();
    ui->RequestsTable->insertRow(row);
    ui->RequestsTable->setItem(row, REQID, new QTableWidgetItem(reqID));
    ui->RequestsTable->setItem(row, DESCRIPTION, new QTableWidgetItem(description));
}
