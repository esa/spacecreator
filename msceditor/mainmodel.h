/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

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

    void fillView();

public Q_SLOTS:
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
