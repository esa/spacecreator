#ifndef GITLABCREDENTIALSDIALOG_H
#define GITLABCREDENTIALSDIALOG_H

#include <QAbstractButton>
#include <QDialog>

namespace Ui {
class GitlabCredentialsDialog;
}

class GitlabCredentialsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GitlabCredentialsDialog(QWidget *parent = nullptr);
    ~GitlabCredentialsDialog();
    QString getUrl() const;
    QString getToken() const;
    void setUrl(const QString& url);

Q_SIGNALS:
    void buttonClicked(QAbstractButton *button);
private:
    Ui::GitlabCredentialsDialog *ui;
};

#endif // GITLABCREDENTIALSDIALOG_H
