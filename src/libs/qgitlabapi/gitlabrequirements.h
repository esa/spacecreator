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


private:
    Ui::GitLabRequirements *ui;
};

#endif // GITLABREQUIREMENTS_H
