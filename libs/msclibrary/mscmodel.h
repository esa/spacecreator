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

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>

namespace msc {
class MscChart;
class MscDocument;

class MscModel : public QObject
{
    Q_OBJECT
public:
    explicit MscModel(QObject *parent = nullptr);
    ~MscModel();

    const QVector<MscDocument *> &documents() const;
    void addDocument(MscDocument *document);

    const QVector<MscChart *> &charts() const;
    void addChart(MscChart *chart);

    const QString &dataLanguage() const;
    void setDataLanguage(const QString &language);

    const QString &dataDefinitionString() const;
    void setDataDefinitionString(const QString &dataString);

    void clear();

Q_SIGNALS:
    void documentAdded(MscDocument *document);
    void chartAdded(MscChart *chart);
    void cleared();
    void dataLanguageChanged(const QString &dataLanguage);
    void dataDefinitionStringChanged(const QString &dataString);

private:
    QVector<MscDocument *> m_documents;
    QVector<MscChart *> m_charts;
    QString m_dataLanguage;
    QString m_dataDefinitionString;
};

} // namespace msc

#endif // MODEL_H
