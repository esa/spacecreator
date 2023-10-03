#include "gitlabrequirements.h"
#include "ui_gitlabrequirements.h"

GitLabRequirements::GitLabRequirements(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GitLabRequirements)
{
    ui->setupUi(this);

    connect(ui->Refresh, &QPushButton::clicked, [this](){
        emit loginUpdate(ui->UrlLineEdit->text(), ui->TokenLineEdit->text());
    });

}

GitLabRequirements::~GitLabRequirements()
{
    delete ui;
}


void GitLabRequirements::setUrl(const QString &url)
{
    ui->UrlLineEdit->setText(url);
}

void GitLabRequirements::setToken(const QString &token)
{
    ui->TokenLineEdit->setText(token);
}

