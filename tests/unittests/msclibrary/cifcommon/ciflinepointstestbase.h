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

#include "cif/ciflines.h"

class CifLinePointsTestBase : public QObject
{
    Q_OBJECT
protected:
    static const QString m_separatorCommaSpace;
    static const QString m_separatorComma;
    static const QString m_separatorSpace;
    static const QString m_separatorNone;

    virtual msc::cif::CifLineShared createCifLine() const = 0;
    virtual QString lineTag() const;
    virtual int expectedPointsAmount() const;

    virtual QString generateTestLine(int pointsCount, const QString &separator) const;

    void checkInitFfromValid(const QString &source);

    virtual void checkDefault();
    virtual void checkInitFromValidSeparatorComma();
    virtual void checkInitFromValidSeparatorSpace();
    virtual void checkInitFromValidSeparatorCommaSpace();
    virtual void checkInitFromValidSeparatorNo();
    virtual void checkInitFromInsufficient();
    virtual void checkInitFromExcessive();
    virtual void checkToString();
};
