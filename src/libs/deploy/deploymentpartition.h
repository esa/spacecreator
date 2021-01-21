#ifndef DEPLOYMENTPARTITION_H
#define DEPLOYMENTPARTITION_H

#include <QObject>

class DeploymentPartition : public QObject
{
    Q_OBJECT
public:
    explicit DeploymentPartition(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DEPLOYMENTPARTITION_H