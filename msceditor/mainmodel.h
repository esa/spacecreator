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
#include <memory>

namespace msc {
class ChartViewModel;
class DocumentItemModel;
class MscChart;
class MscDocument;
class MscModel;

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

    void initialModel();

    QGraphicsScene *graphicsScene() const;
    QGraphicsScene *hierarchyScene() const;

    msc::DocumentItemModel *documentItemModel() const;

    QStringList errorMessages() const;

    msc::ChartViewModel &chartViewModel() const;

    QString modelText() const;
    msc::MscModel *mscModel() const;

Q_SIGNALS:
    void showChartVew();
    void modelDataChanged();
    void documentClicked(const msc::MscDocument *document);
    void selectionChanged(const msc::MscDocument *document);

public Q_SLOTS:
    void showFirstChart();
    bool loadFile(const QString &filename);
    void saveMsc(const QString &fileName);

private Q_SLOTS:
    void showChartFromDocument(msc::MscDocument *document);

private:
    msc::MscChart *firstChart() const;
    msc::MscChart *firstChart(const QVector<msc::MscDocument *> docs) const;
    void clearMscModel();
    void setNewModel(msc::MscModel *model);

    std::unique_ptr<MainModelPrivate> const d;
};

#endif // MAINMODEL_H
