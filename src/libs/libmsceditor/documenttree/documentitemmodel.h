/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include <QAbstractItemModel>
#include <QPointer>

namespace msc {
class MscChart;
class MscDocument;
class MscModel;
class MscCommandsStack;

class DocumentItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DocumentItemModel(QObject *parent = nullptr);

    void setMscModel(MscModel *mscModel);
    MscModel *mscModel() const;

    void setUndoStack(MscCommandsStack *undoStack);

    QModelIndex index(MscDocument *document) const;

    void updateHierarchyType(const QModelIndex &index, const QVariant &hierarchyType);

    // QAbstractItemModel interface
public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private Q_SLOTS:
    void onEntityDataChanged();

private:
    void connectDocument(msc::MscDocument *document);
    void disconnectDocument(msc::MscDocument *document);

    QPointer<MscModel> m_mscModel;
    QPointer<MscCommandsStack> m_undoStack;
};

} // namespace msc
