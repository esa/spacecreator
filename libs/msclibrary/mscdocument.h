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

#include "mscentity.h"

#include <QObject>
#include <QString>
#include <QVector>

namespace msc {
class MscChart;

class MscDocument : public MscEntity
{
    Q_OBJECT
public:
    enum HierarchyType
    {
        HierarchyLeaf,
        HierarchyIs,
        HierarchyAnd,
        HierarchyOr,
        HierarchyParallel,
        HierarchyRepeat,
        HierarchyException
    };

    explicit MscDocument(QObject *parent = nullptr);
    MscDocument(const QString &name, QObject *parent = nullptr);
    ~MscDocument() override;

    const QVector<MscDocument *> &documents() const;
    bool addDocument(MscDocument *document);

    const QVector<MscChart *> &charts() const;
    void addChart(MscChart *chart);

    void clear();

    MscDocument *parentDocument() const;

    MscEntity::EntityType entityType() const override;

    HierarchyType hierarchyType() const;
    void setHierarchyType(HierarchyType type);

    bool isSingleChildDocument() const;

Q_SIGNALS:
    void documentAdded(MscDocument *document);
    void chartAdded(MscChart *chart);
    void cleared();
    void hierarchyTypeChanged(HierarchyType type);

private:
    QVector<MscDocument *> m_documents;
    QVector<MscChart *> m_charts;

    HierarchyType m_hierarchyType = HierarchyAnd;
};

} // namespace msc
