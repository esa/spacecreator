/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "abstracttabdocument.h"

#include <QMap>
#include <QObject>
#include <QPointer>
#include <QTabWidget>
#include <QVector>

namespace taste3 {
namespace document {

class DocumentsManager : public QObject
{
    Q_OBJECT
public:
    explicit DocumentsManager(QTabWidget *tabWidget, QObject *parent = nullptr);

    bool addDocument(document::AbstractTabDocument *doc);
    bool removeDocument(int docId);
    bool removeDocument(document::AbstractTabDocument *doc);
    AbstractTabDocument *docById(int id) const;
    int currentDocId() const;
    AbstractTabDocument *currentDoc() const;
    int docCount() const;

    QVector<document::AbstractTabDocument *> documents() const;

signals:
    void currentDocIdChanged(int currentDocId);

protected:
    static int constexpr InvalidDocId { -1 };
    QPointer<QTabWidget> m_tabs;
    QVector<document::AbstractTabDocument *> m_documents;
    QMap<document::AbstractTabDocument *, QWidget *> m_views;

    int docId(document::AbstractTabDocument *doc) const;
    bool isValidDocId(int id) const;
};

} // ns document
} // ns taste3
