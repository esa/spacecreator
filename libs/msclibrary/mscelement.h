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

#ifndef MSCELEMENT_H
#define MSCELEMENT_H

#include <QObject>
#include <QString>

namespace msc {

class MscElement : public QObject
{
    Q_OBJECT
public:
    explicit MscElement(QObject *parent = nullptr);
    MscElement(const QString &name, QObject *parent = nullptr);

    const QString &name() const;
    void setName(const QString &name);

Q_SIGNALS:
    void nameChanged(const QString &name);

private:
    QString m_name = tr("Untitled");
};

} // namespace msc

#endif // MSCELEMENT_H
