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

#include "mscmodel.h"

#include "mscchart.h"
#include "mscdocument.h"

namespace msc {

MscModel::MscModel(QObject *parent)
    : QObject(parent)
{
}

MscModel::~MscModel()
{
    clear();
}

const QVector<MscDocument *> &MscModel::documents() const
{
    return m_documents;
}

bool MscModel::addDocument(MscDocument *document)
{
    if (document == nullptr) {
        return false;
    }
    if (m_documents.contains(document)) {
        return false;
    }

    document->setParent(this);
    m_documents.append(document);
    connect(document, &MscDocument::dataChanged, this, &MscModel::dataChanged);
    Q_EMIT documentAdded(document);
    Q_EMIT dataChanged();

    return true;
}

void MscModel::removeDocument(MscDocument *document)
{
    if (document == nullptr) {
        return;
    }
    if (!m_documents.contains(document)) {
        return;
    }

    if (m_documents.removeAll(document)) {
        document->clear();
    }
}

const QVector<MscChart *> &MscModel::charts() const
{
    return m_charts;
}

void MscModel::addChart(MscChart *chart)
{
    if (chart == nullptr) {
        return;
    }
    if (m_charts.contains(chart)) {
        return;
    }

    chart->setParent(this);
    m_charts.append(chart);
    connect(chart, &MscChart::dataChanged, this, &MscModel::dataChanged);
    Q_EMIT chartAdded(chart);
    Q_EMIT dataChanged();
}

const QString &MscModel::dataLanguage() const
{
    return m_dataLanguage;
}

void MscModel::setDataLanguage(const QString &language)
{
    if (language == m_dataLanguage) {
        return;
    }

    m_dataLanguage = language;
    Q_EMIT dataLanguageChanged(m_dataLanguage);
    Q_EMIT dataChanged();
}

const QString &MscModel::dataDefinitionString() const
{
    return m_dataDefinitionString;
}

void MscModel::setDataDefinitionString(const QString &dataString)
{
    if (dataString == m_dataDefinitionString) {
        return;
    }

    m_dataDefinitionString = dataString;

    Q_EMIT dataDefinitionStringChanged(m_dataDefinitionString);
    Q_EMIT dataChanged();
}

const QVariantList &MscModel::asn1TypesData() const
{
    return m_asn1TypesData;
}

void MscModel::setAsn1TypesData(const QVariantList &asn1TypesData)
{
    m_asn1TypesData = asn1TypesData;
}

void MscModel::clear()
{
    qDeleteAll(m_documents);
    m_documents.clear();

    qDeleteAll(m_charts);
    m_charts.clear();

    setDataLanguage("");

    Q_EMIT cleared();
    Q_EMIT dataChanged();
}

} // namespace msc
