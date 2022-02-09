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

#include "mscchart.h"
#include "mscentity.h"
#include "mscmessagedeclaration.h"
#include "mscmessagedeclarationlist.h"

#include <QObject>
#include <QString>
#include <QVector>

namespace msc {

class MscDocument : public MscEntity
{
    Q_OBJECT
    Q_PROPERTY(msc::MscDocument::HierarchyType hierarchyType READ hierarchyType NOTIFY hierarchyTypeChanged)
    Q_PROPERTY(QString hierarchyTypeString READ hierarchyTypeString NOTIFY hierarchyTypeChanged)
    Q_PROPERTY(QVector<msc::MscDocument *> documents READ documents NOTIFY documentsChanged)
    Q_PROPERTY(QVector<msc::MscChart *> charts READ charts NOTIFY chartsChanged)
    Q_PROPERTY(QList<msc::MscMessageDeclaration *> declarations READ declarations CONSTANT)

public:
    enum HierarchyType
    {
        HierarchyLeaf,
        HierarchyIs,
        HierarchyAnd,
        HierarchyOr,
        HierarchyParallel,
        HierarchyRepeat,
        HierarchyException,
        HierarchyUnkown /// Only used when loaded from a file
    };
    Q_ENUM(HierarchyType);

    explicit MscDocument(QObject *parent = nullptr);
    MscDocument(const QString &name, QObject *parent = nullptr);
    ~MscDocument() override;

    const QVector<MscDocument *> &documents() const;
    bool addDocument(MscDocument *document, int index = -1);
    bool addDocument(MscDocument *document, bool checkType, int index = -1);
    void removeDocument(MscDocument *document, bool clear = true);

    const QVector<MscChart *> &charts() const;
    void addChart(MscChart *chart);

    MscMessageDeclarationList *messageDeclarations() const;
    MscMessageDeclarationList *messageDeclarations();
    QList<msc::MscMessageDeclaration *> declarations() const;

    void clear();
    void clearCharts();

    MscDocument *parentDocument() const;

    MscEntity::EntityType entityType() const override;

    HierarchyType hierarchyType() const;
    void setHierarchyType(HierarchyType type);
    QString hierarchyTypeString() const;

    bool isSingleChildDocument() const;
    bool isAddChildEnable() const;

    int maxInstanceNameNumber() const;
    int setInstanceNameNumbers(int nextNumber);

Q_SIGNALS:
    void documentsChanged();
    void documentAdded(msc::MscDocument *document);
    void documentRemoved(msc::MscDocument *document);
    void chartsChanged();
    void chartAdded(msc::MscChart *chart);
    void cleared();
    void hierarchyTypeChanged(msc::MscDocument::HierarchyType type);
    void documentRemovedFrom(msc::MscDocument *document, QObject *parentObject);

private:
    QVector<MscDocument *> m_documents;
    QVector<MscChart *> m_charts;
    MscMessageDeclarationList *m_messageDeclarations = nullptr;

    HierarchyType m_hierarchyType = HierarchyAnd;
};

}

Q_DECLARE_METATYPE(msc::MscDocument *)
Q_DECLARE_METATYPE(QVector<msc::MscDocument *>)
