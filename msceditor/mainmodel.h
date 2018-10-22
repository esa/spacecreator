#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>
#include <QString>
#include <QVector>

namespace msc {
class MscChart;
class MscDocument;
class MscModel;

class InstanceItem;
class MessageItem;
}

class QGraphicsScene;

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

    msc::MscModel *m_mscModel = nullptr;
    QGraphicsScene *m_scene = nullptr;

    QVector<msc::InstanceItem *> m_instanceItems;
    QVector<msc::MessageItem *> m_messageItems;
};

#endif // MAINMODEL_H
