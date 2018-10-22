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

MainModel::~MainModel()
{
    delete m_scene;
    delete m_mscModel;
}

QGraphicsScene *MainModel::graphicsScene() const
{
    return m_scene;
}

void MainModel::fillView()
{
    m_scene->clear();
    m_instanceItems.clear();
    m_messageItems.clear();

    MscChart *chart = firstChart();
    if (chart == nullptr) {
        return;
    }

    double x = 100.0;
    for (MscInstance *instance : chart->instances()) {
        auto *item = new InstanceItem(instance);
        item->setKind(instance->kind());
        item->setX(x);
        m_scene->addItem(item);
        m_instanceItems.append(item);
        x += 100.0;
    }

    double y = 50.0;
    for (MscMessage *message : chart->messages()) {
        auto *item = new MessageItem(message);
        item->setY(y);

        if (message->sourceInstance() != nullptr) {
            QString sourceName = message->sourceInstance()->name();
            InstanceItem *instItem = instanceItem(sourceName);
            if (instItem != nullptr) {
                item->setSourceInstanceItem(instItem);
            }
        }
        if (message->targetInstance() != nullptr) {
            QString targetName = message->targetInstance()->name();
            InstanceItem *instItem = instanceItem(targetName);
            if (instItem != nullptr) {
                item->setTargetInstanceItem(instItem);
            }
        }

        m_scene->addItem(item);
        m_messageItems.append(item);
        y += 40.0;
    }

    for (InstanceItem *item : m_instanceItems) {
        item->setAxisHeight(y);
    }
}

void MainModel::loadFile(const QString &filename)
{
    m_scene->clear();
    delete m_mscModel;
    m_mscModel = nullptr;

    msc::MscFile file;
    try {
        m_mscModel = file.parseFile(filename);
    } catch (...) {
        //print error message
        return;
    }

    connect(m_mscModel, &msc::MscModel::documentAdded, this, &MainModel::fillView);
    connect(m_mscModel, &msc::MscModel::chartAdded, this, &MainModel::fillView);
    connect(m_mscModel, &msc::MscModel::cleared, this, &MainModel::fillView);

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

InstanceItem *MainModel::instanceItem(const QString &name) const
{
    for (QGraphicsItem *item : m_scene->items()) {
        InstanceItem *instance = dynamic_cast<InstanceItem *>(item);
        if (instance != nullptr) {
            if (instance->name() == name) {
                return instance;
            }
        }
    }
    return nullptr;
}
