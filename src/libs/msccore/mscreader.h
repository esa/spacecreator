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

#include <QString>
#include <QStringList>

namespace antlr4 {
class ANTLRInputStream;
}

namespace msc {
class MscModel;
class MscDocument;

class MscReader
{
public:
    enum class NOTIFY {
        USE_ERROR_HUB,
        NO_HUB
    };

    MscReader(NOTIFY notifyErrorHub);

    MscModel *parseFile(const QString &filename, QStringList *errorMessages = nullptr);
    MscModel *parseText(const QString &text, QStringList *errorMessages = nullptr);

    const QStringList &getErrorMessages() const;

private:
    MscModel *parse(antlr4::ANTLRInputStream &input, QStringList *errorMessages = nullptr);
    void checkDocumentHierarchy(MscDocument *doc);

    QStringList m_errorMessages;
    NOTIFY m_notifyErrorHub = NOTIFY::NO_HUB;
};

}
