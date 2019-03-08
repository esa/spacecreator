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

#include "cifline.h"

namespace msc {
namespace cif {

class CifLineAction : public CifLineTwoPointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineCall : public CifLineTwoPointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineComment : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineCondition : public CifLineTwoPointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineCreate : public CifLineTwoPointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineCollapsed : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineEnd : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineHyperLink : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineInstance : public CifLineThreePointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineImport : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineKeep : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineLastModified : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineMessage : public CifLineThreePointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineMscDocument : public CifLineTwoPointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineModified : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineMscPageSize : public CifLineOnePointHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineNested : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLinePosition : public CifLineOnePointHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLinePreview : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineReset : public CifLineTwoPointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineSet : public CifLineTwoPointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineStop : public CifLineTwoPointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineSubmsc : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineSpecific : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineText : public CifLineTwoPointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineTimeout : public CifLineTwoPointsHolder
{
public:
    CifLine::CifType lineType() const override;
};
class CifLineTextMode : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};
class CifLineTextName : public CifLine
{
public:
    CifLine::CifType lineType() const override;
    bool initFrom(const QString &sourceLine) override;
    QVariant payload() const override;
    void setPayload(const QVariant &p) override;
};

} // ns cif
} // ns msc
