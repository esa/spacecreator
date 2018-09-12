#include "mainmodel.h"

#include "mscchart.h"
#include "mscdocument.h"
#include "mscfile.h"
#include "mscinstance.h"
#include "mscmodel.h"

#include "instanceitem.h"
#include "messageitem.h"

#include <QGraphicsScene>

using namespace msc;

MainModel::MainModel(QObject *parent)
    : QObject(parent)
    , m_scene(new QGraphicsScene(this))
{
}

QGraphicsScene *MainModel::graphicsScene() const
{
    return m_scene;
}

void MainModel::fillView()
{
    m_scene->clear();

    MscChart *chart = firstChart();
    if (chart == nullptr) {
        return;
    }

    double x = 0;
    for (MscInstance *instance : chart->instances()) {
        InstanceItem *iteam = new InstanceItem();
        iteam->setName(instance->name());
        iteam->setX(x);
        m_scene->addItem(iteam);
        x += 100.0;
    }
}

void MainModel::loadFile(const QString &filename)
{
    m_scene->clear();
    delete m_mscModel;

    msc::MscFile file;
    try {
        m_mscModel = file.parseFile(filename);
    } catch (...) {
    }

    fillView();
}

MscChart *MainModel::firstChart() const
{
    if (m_mscModel == nullptr) {
        return nullptr;
    }

    if (!m_mscModel->charts().empty()) {
        return m_mscModel->charts().at(0);
    }

    return firstChart(m_mscModel->documents());
}

MscChart *MainModel::firstChart(const QVector<MscDocument *> docs) const
{
    for (MscDocument *doc : docs) {
        if (!doc->charts().isEmpty()) {
            return doc->charts().at(0);
        }
        auto ret = firstChart(doc->documents());
        if (ret != nullptr) {
            return ret;
        }
    }
    return nullptr;
}
