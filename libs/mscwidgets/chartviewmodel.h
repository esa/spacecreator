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

#ifndef CHARTVIEWMODEL_H
#define CHARTVIEWMODEL_H

#include <QObject>

#include <memory>

class QGraphicsScene;

namespace msc {
class MscChart;
class InstanceItem;

struct ChartViewModelPrivate;

class ChartViewModel : public QObject
{
    Q_OBJECT
public:
    explicit ChartViewModel(QObject *parent = nullptr);
    ~ChartViewModel();

    QGraphicsScene *graphicsScene() const;

    msc::MscChart *currentChart() const;

    void clearScene();

public Q_SLOTS:
    void fillView(msc::MscChart *chart);

Q_SIGNALS:
    void currentChartChagend(msc::MscChart *chart);

private Q_SLOTS:
    Q_INVOKABLE void layoutItems();

private:
    msc::InstanceItem *instanceItem(const QString &name) const;

    std::unique_ptr<ChartViewModelPrivate> const d;
};

} // namespace msc

#endif // CHARTVIEWMODEL_H
