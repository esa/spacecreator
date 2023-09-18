#include "gitlabcredentialsdialog.h"
#include "ui_gitlabcredentialsdialog.h"

GitlabCredentialsDialog::GitlabCredentialsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GitlabCredentialsDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &GitlabCredentialsDialog::buttonClicked);
}

GitlabCredentialsDialog::~GitlabCredentialsDialog()
{
    delete ui;
}

QString GitlabCredentialsDialog::getUrl() const
{
    return ui->gitlabUrlLineEdit->text();
}

QString GitlabCredentialsDialog::getToken() const
{
    return ui->gitlabTokenLineEdit->text();
}

void GitlabCredentialsDialog::setUrl(const QString &url)
{
    ui->gitlabUrlLineEdit->setText(url);
}
