#ifndef GITLABREQUESTS_H
#define GITLABREQUESTS_H

#include <QWidget>

namespace Ui {
class GitlabRequests;
}

class GitlabRequests : public QWidget
{
    Q_OBJECT

public:
    explicit GitlabRequests(QWidget *parent = nullptr);
    ~GitlabRequests();
    void addRequest(const QString &reqID, const QString &description);

private:
    Ui::GitlabRequests *ui;
};

#endif // GITLABREQUESTS_H
