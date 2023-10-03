#include "gitlabrequirements.h"
#include "ui_gitlabrequirements.h"

GitLabRequirements::GitLabRequirements(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GitLabRequirements)
{
    ui->setupUi(this);
}

GitLabRequirements::~GitLabRequirements()
{
    delete ui;
}
