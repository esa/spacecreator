#include "mainmodel.h"

#include "mscchart.h"
#include "mscdocument.h"
#include "mscfile.h"
#include "mscinstance.h"
#include "mscmessage.h"
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

    double x = 0.0;
    for (MscInstance *instance : chart->instances()) {
        auto *item = new InstanceItem();
        item->setName(instance->name());
        item->setX(x);
        m_scene->addItem(item);
        x += 100.0;
    }

    double y = 50.0;
    for (MscMessage *message : chart->messages()) {
        auto *item = new MessageItem();
        item->setName(message->name());
        item->setX(40.0);
        item->setWidth(100.0);
        item->setY(y);
        m_scene->addItem(item);
        y += 40.0;
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
