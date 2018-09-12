#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>
#include <QString>
#include <QVector>

namespace msc {
class MscChart;
class MscDocument;
class MscModel;
}

class QGraphicsScene;

class MainModel : public QObject
{
    Q_OBJECT
public:
    explicit MainModel(QObject *parent = nullptr);

    QGraphicsScene *graphicsScene() const;

    void fillView();

public slots:
    void loadFile(const QString &filename);

private:
    msc::MscChart *firstChart() const;
    msc::MscChart *firstChart(const QVector<msc::MscDocument *> docs) const;

    msc::MscModel *m_mscModel = nullptr;
    QGraphicsScene *m_scene = nullptr;
};

#endif // MAINMODEL_H
