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

#ifndef HIERARCHYVIEWMODEL_H
#define HIERARCHYVIEWMODEL_H

#include <QObject>
#include <memory>

class QGraphicsScene;

namespace msc {

class MscDocument;
class MscModel;

class HierarchyViewModel : public QObject
{
    Q_OBJECT
public:
    explicit HierarchyViewModel(QObject *parent = nullptr);
    ~HierarchyViewModel() override;

    QGraphicsScene *graphicsScene() const;

    void setModel(MscModel *model);

Q_SIGNALS:
    void documentDoubleClicked(msc::MscDocument *document);

private Q_SLOTS:
    void modelDeleted();

private:
    struct HierarchyViewModelPrivate;
    std::unique_ptr<HierarchyViewModelPrivate> const d;
};

}

#endif // HIERARCHYVIEWMODEL_H
