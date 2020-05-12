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

#include "msccomment.h"

#include "cif/cifblockfactory.h"
#include "cif/cifline.h"
#include "cif/ciflines.h"

#include <QRect>

namespace msc {

MscComment::MscComment(QObject *parent)
    : MscInstanceEvent(parent)
{
    connect(this, &MscEntity::dataChanged, this, &MscComment::onDataChanged);
}

MscEntity::EntityType MscComment::entityType() const
{
    return MscEntity::EntityType::Comment;
}

cif::CifLine::CifType MscComment::mainCifType() const
{
    return isGlobal() ? cif::CifLine::CifType::Text : cif::CifLine::CifType::Comment;
}

bool MscComment::isGlobal() const
{
    return m_attachedEntity && m_attachedEntity->entityType() == MscEntity::EntityType::Chart;
}

QRect MscComment::rect() const
{
    return m_rect;
}

void MscComment::setRect(const QRect &rect)
{
    if (m_rect == rect) {
        return;
    }

    m_rect = rect;
    updateCifComment();
}

QString MscComment::text() const
{
    return m_text;
}

void MscComment::setText(const QString &comment)
{
    if (m_text == comment) {
        return;
    }

    m_text = comment;
    updateCifComment();
    Q_EMIT commentChanged();
}

MscEntity *MscComment::attachedEntity() const
{
    return m_attachedEntity;
}

void MscComment::attachTo(MscEntity *entity)
{
    if (m_attachedEntity == entity) {
        return;
    }

    if (m_attachedEntity) {
        m_attachedEntity->setComment(nullptr);
    }

    m_attachedEntity = entity;

    if (m_attachedEntity) {
        m_attachedEntity->setComment(this);
    }

    Q_EMIT isGlobalChanged();
}

bool MscComment::relatesTo(const MscInstance *instance) const
{
    Q_UNUSED(instance);
    return false;
}

QString MscComment::cifText(int tabsSize) const
{
    QStringList cifTexts;
    const QVector<cif::CifBlockShared> cifLines = cifs();
    cifTexts.reserve(cifLines.size());
    for (const cif::CifBlockShared &cifBlock : cifLines) {
        cifTexts << cifBlock->toString(tabsSize);
    }

    return cifTexts.join("\n");
}

void MscComment::updateCifComment()
{
    cif::CifBlockShared cifBlock = cifBlockByType(mainCifType());

    if (!m_rect.isNull()) {
        /// Comment geometry update
        cif::CifLineShared cifLine;
        if (isGlobal())
            cifLine = cif::CifLineShared(new cif::CifLineText());
        else
            cifLine = cif::CifLineShared(new cif::CifLineComment());

        if (cifBlock.isNull()) {
            if (isGlobal())
                cifBlock = cif::CifBlockFactory::createBlockText({ cif::CifLineShared(cifLine) });
            else
                cifBlock = cif::CifBlockFactory::createBlockComment({ cif::CifLineShared(cifLine) });
            addCif(cifBlock);
        } else if (!cifBlock->hasPayloadFor(mainCifType())) {
            cifBlock->addLine(cif::CifLineShared(cifLine));
        }

        const QVector<QPoint> points { m_rect.topLeft(), { m_rect.width(), m_rect.height() } };
        cifBlock->setPayload(QVariant::fromValue(points), mainCifType());
    } else if (cifBlock) {
        cifBlock->clearPayload(mainCifType());
    }

    /// Comment text update
    if (isGlobal() && !m_text.isEmpty() && m_rect.isValid()) {
        if (cifBlock.isNull()) {
            cifBlock = cif::CifBlockFactory::createBlockText({ cif::CifLineShared(new cif::CifLineText()) });
            addCif(cifBlock);
        } else if (!cifBlock->hasPayloadFor(mainCifType())) {
            cifBlock->addLine(cif::CifLineShared(new cif::CifLineText()));
        }

        if (!cifBlock->hasPayloadFor(cif::CifLine::CifType::GlobalComment))
            cifBlock->addLine(cif::CifLineShared(new cif::CifLineGlobalComment()));
        cifBlock->setPayload(QVariant::fromValue(m_text), cif::CifLine::CifType::GlobalComment);

        if (!cifBlock->hasPayloadFor(cif::CifLine::CifType::End))
            cifBlock->addLine(cif::CifLineShared(new cif::CifLineEnd()));
        cifBlock->setPayload(QVariant::fromValue(cif::CifLine::nameForType(cif::CifLine::CifType::Text)),
                cif::CifLine::CifType::End);
    } else if (cifBlock) {
        cifBlock->clearPayload(cif::CifLine::CifType::GlobalComment);
        cifBlock->clearPayload(cif::CifLine::CifType::End);
    }

    Q_EMIT dataChanged();
}

void MscComment::onDataChanged()
{
    cif::CifBlockShared cifBlock = cifBlockByType(mainCifType());
    if (cifBlock.isNull())
        return;

    if (cifBlock->hasPayloadFor(cif::CifLine::CifType::GlobalComment))
        m_text = cifBlock->payload(cif::CifLine::CifType::GlobalComment).toString();

    if (!cifBlock->hasPayloadFor(mainCifType()))
        return;

    const QVector<QPoint> &points = cifBlock->payload(mainCifType()).value<QVector<QPoint>>();
    if (points.size() != 2)
        return;

    const QPoint &pos = points.first();
    const QPoint &size = points.last();
    m_rect = { pos.x(), pos.y(), size.x(), size.y() };
}

} // namespace msc
