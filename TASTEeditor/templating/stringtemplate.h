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

#ifndef STRINGTEMPLATE_H
#define STRINGTEMPLATE_H

#include <QObject>

namespace Grantlee {
    class Engine;
}

namespace taste3 {

namespace aadl {
    class AADLObject;
}

namespace templating {

class StringTemplate : public QObject
{
    Q_OBJECT
public:
    explicit StringTemplate(QObject *parent = nullptr);

    QString parseFile(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName);

    QString formatText(const QString &text);

    int autoFormattingIndent() const;

public slots:
    void setAutoFormattingIndent(int autoFormattingIndent);

signals:
    void errorOccurred(const QString &errorString);

private:
    Grantlee::Engine *m_engine;
    int m_autoFormattingIndent;
};

} // ns processing
} // ns taste3

#endif // STRINGTEMPLATE_H
