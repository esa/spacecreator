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

#include "mscdocument.h"
#include "mscchart.h"

namespace msc {

MscDocument::MscDocument(QObject *parent)
    : QObject(parent)
{
}

MscDocument::MscDocument(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
{
}

MscDocument::~MscDocument()
{
    clear();
}

const QString &MscDocument::name() const
{
    return m_name;
}

void MscDocument::setName(const QString &name)
{
    if (name == m_name) {
        return;
    }

    m_name = name;
    Q_EMIT nameChanged(m_name);
}

const QVector<MscDocument *> &MscDocument::documents() const
{
    return m_documents;
}

void MscDocument::addDocument(MscDocument *document)
{
    if (document == nullptr) {
        return;
    }
    if (m_documents.contains(document)) {
        return;
    }

    document->setParent(this);
    m_documents.append(document);
    Q_EMIT documentAdded(document);
}

const QVector<MscChart *> &MscDocument::charts() const
{
    return m_charts;
}

void MscDocument::addChart(MscChart *chart)
{
    if (chart == nullptr) {
        return;
    }
    if (m_charts.contains(chart)) {
        return;
    }

    chart->setParent(this);
    m_charts.append(chart);
    Q_EMIT chartAdded(chart);
}

void MscDocument::clear()
{
    m_name.clear();

    qDeleteAll(m_documents);
    m_documents.clear();

    qDeleteAll(m_charts);
    m_charts.clear();

    Q_EMIT cleared();
}

} // namespace msc
