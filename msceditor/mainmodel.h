#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>
#include <QString>
#include <QVector>

namespace msc {
class MscChart;
class MscDocument;

class InstanceItem;
}

class QGraphicsScene;

struct MainModelPrivate;

class MainModel : public QObject
{
    Q_OBJECT
public:
    explicit MainModel(QObject *parent = nullptr);
    ~MainModel();

    QGraphicsScene *graphicsScene() const;

public Q_SLOTS:
    void fillView();
    void loadFile(const QString &filename);

private:
    msc::MscChart *firstChart() const;
    msc::MscChart *firstChart(const QVector<msc::MscDocument *> docs) const;
    msc::InstanceItem *instanceItem(const QString &name) const;
    void clearMscModel();
    void clearScene();

    MainModelPrivate *const d;
};

#endif // MAINMODEL_H
