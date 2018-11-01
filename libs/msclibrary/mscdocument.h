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

#ifndef MSCDOCUMENT_H
#define MSCDOCUMENT_H

#include <QObject>
#include <QString>
#include <QVector>

namespace msc {
class MscChart;

class MscDocument : public QObject
{
    Q_OBJECT
public:
    explicit MscDocument(QObject *parent = nullptr);
    MscDocument(const QString &name, QObject *parent = nullptr);
    ~MscDocument();

    const QString &name() const;
    void setName(const QString &name);

    const QVector<MscDocument *> &documents() const;
    void addDocument(MscDocument *document);

    const QVector<MscChart *> &charts() const;
    void addChart(MscChart *chart);

    void clear();

    MscDocument *parentDocument() const;

Q_SIGNALS:
    void nameChanged(const QString &name);
    void documentAdded(MscDocument *document);
    void chartAdded(MscChart *chart);
    void cleared();

private:
    QString m_name = tr("Untitled");
    QVector<MscDocument *> m_documents;
    QVector<MscChart *> m_charts;
};

} // namespace msc

#endif // MSCDOCUMENT_H
