#ifndef GITLABREQUIREMENTS_H
#define GITLABREQUIREMENTS_H

#include <QWidget>

namespace Ui {
class GitLabRequirements;
}

class GitLabRequirements : public QWidget
{
    Q_OBJECT

public:
    explicit GitLabRequirements(QWidget *parent = nullptr);
    ~GitLabRequirements();


    void setUrl(const QString& url);
    void setToken(const QString& token);

Q_SIGNALS:
    void loginUpdate(const QString& url, const QString& token);

private:
    Ui::GitLabRequirements *ui;
};

#endif // GITLABREQUIREMENTS_H
