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

#include <QObject>
#include <QVector>

namespace msc {
class MscChart;
class MscDocument;
class MscMessage;

class MscModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dataLanguage READ dataLanguage WRITE setDataLanguage NOTIFY dataLanguageChanged)
    Q_PROPERTY(QString dataDefinitionString READ dataDefinitionString WRITE setDataDefinitionString NOTIFY
                    dataDefinitionStringChanged)
    Q_PROPERTY(QVector<msc::MscDocument *> documents READ documents NOTIFY dataChanged)
    Q_PROPERTY(QVector<msc::MscChart *> charts READ charts NOTIFY dataChanged)

public:
    explicit MscModel(QObject *parent = nullptr);
    ~MscModel();

    const QVector<MscDocument *> &documents() const;
    bool addDocument(MscDocument *document);
    void removeDocument(MscDocument *document);

    const QVector<MscChart *> &charts() const;
    void addChart(MscChart *chart);

    const QString &dataLanguage() const;
    void setDataLanguage(const QString &language);

    const QString &dataDefinitionString() const;
    void setDataDefinitionString(const QString &dataString);

    const QVariantList &asn1TypesData() const;
    void setAsn1TypesData(const QVariantList &asn1TypesData);

    void clear();

    void checkInstanceNames();

    bool checkparameterAsn1Compliance(const QString &parameter, const QString &typeName) const;
    bool checkMessageAsn1Compliance(const msc::MscMessage &message) const;

Q_SIGNALS:
    void dataChanged();
    void documentAdded(msc::MscDocument *document);
    void chartAdded(msc::MscChart *chart);
    void cleared();
    void dataLanguageChanged(const QString &dataLanguage);
    void dataDefinitionStringChanged(const QString &dataString);
    void asn1DataChanged();

private:
    QVector<MscDocument *> m_documents;
    QVector<MscChart *> m_charts;
    QString m_dataLanguage;
    QString m_dataDefinitionString;
    QVariantList m_asn1TypesData;
};

}
