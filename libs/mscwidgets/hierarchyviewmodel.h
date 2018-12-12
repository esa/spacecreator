#ifndef HIERARCHYVIEWMODEL_H
#define HIERARCHYVIEWMODEL_H

#include <QObject>

class QGraphicsScene;

namespace msc {

class MscModel;

class HierarchyViewModel : public QObject
{
    Q_OBJECT
public:
    explicit HierarchyViewModel(QObject *parent = nullptr);
    ~HierarchyViewModel() override;

    QGraphicsScene* graphicsScene() const;

    void setModel(MscModel* model);

private Q_SLOTS:
    void modelDeleted();

private:
    struct HierarchyViewModelPrivate;
    std::unique_ptr<HierarchyViewModelPrivate> const d;
};

}

#endif // HIERARCHYVIEWMODEL_H
