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
class DocumentItemModel;
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

    msc::DocumentItemModel *documentItemModel() const;

public Q_SLOTS:
    void showFirstChart();
    void fillView(msc::MscChart *chart);
    bool loadFile(const QString &filename);

private:
    msc::MscChart *firstChart() const;
    msc::MscChart *firstChart(const QVector<msc::MscDocument *> docs) const;
    msc::InstanceItem *instanceItem(const QString &name) const;
    void clearMscModel();
    void clearScene();

    MainModelPrivate *const d;
};

#endif // MAINMODEL_H
