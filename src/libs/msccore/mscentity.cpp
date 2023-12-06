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

#include "mscentity.h"

#include "cif/cifblockfactory.h"
#include "cif/ciflines.h"
#include "msccomment.h"

#include <QDebug>
#include <QMetaEnum>

namespace msc {

const QRegularExpression MscEntity::m_nameVerify("([A-Z]|[a-z]|\\d|_|\\.)+");

/*!
   \class msc::MscEntity
   \brief base class for all MSC entities
 */

const QString MscEntity::DefaultName = QObject::tr("Untitled");

MscEntity::MscEntity(QObject *parent)
    : MscEntity(DefaultName, parent)
{
}

MscEntity::MscEntity(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_id(QUuid::createUuid())
{
}

const QString &MscEntity::name() const
{
    return m_name;
}

void MscEntity::setName(const QString &name)
{
    if (name == m_name || name.isEmpty()) {
        return;
    }

    m_name = name;
    Q_EMIT nameChanged(m_name);
    Q_EMIT dataChanged();
}

/*!
   Returns a regular expression that can be used to check if names are correct for the msc specification
 */
const QRegularExpression &MscEntity::nameVerifier()
{
    return m_nameVerify;
}

QUuid MscEntity::internalId() const
{
    return m_id;
}

MscComment *MscEntity::comment() const
{
    return m_comment;
}

void MscEntity::setComment(MscComment *comment)
{
    if (comment == m_comment) {
        return;
    }

    if (m_comment) {
        disconnect(m_comment, nullptr, this, nullptr);
    }

    m_comment = comment;
    if (m_comment != nullptr) {
        connect(m_comment, &MscComment::textChanged, this, &MscEntity::commentChanged);
    }
    Q_EMIT commentChanged();
}

QObject *MscEntity::commentObj() const
{
    return m_comment;
}

void MscEntity::setCommentObj(QObject *obj)
{
    if (auto com = qobject_cast<MscComment *>(obj)) {
        setComment(com);
    }
}

QString MscEntity::commentString() const
{
    if (!m_comment) {
        return {};
    }

    return m_comment->text();
}

MscComment *MscEntity::setCommentString(const QString &comment)
{
    if (!m_comment) {
        setComment(new MscComment(this));
        m_comment->attachTo(this);
    }
    m_comment->setText(comment);
    return m_comment;
}

/*!
   Return the type of this entity as string
 */
QString MscEntity::entityTypeName() const
{
    const QMetaEnum &e = QMetaEnum::fromType<MscEntity::EntityType>();
    return e.valueToKey(int(entityType()));
}

QVector<cif::CifBlockShared> MscEntity::cifs() const
{
    if (m_requirements.isEmpty()) {
        return m_cifs;
    }

    QVector<cif::CifBlockShared> allCifs = m_cifs;
    allCifs.append(requirementsCifBlock());
    return allCifs;
}

void MscEntity::setCifs(const QVector<cif::CifBlockShared> &cifs)
{
    if (m_cifs != cifs) {
        m_cifs.clear();
        bool requirementsCleared = false;
        for (const cif::CifBlockShared &block : cifs) {
            if (block->blockType() != cif::CifLine::CifType::Requirement) {
                m_cifs.append(block);
            } else {
                if (!requirementsCleared) {
                    m_requirements.clear();
                    requirementsCleared = true;
                }
                m_requirements.append(requirementsFromCifBlock(block));
            }
        }

        Q_EMIT dataChanged();
        Q_EMIT cifTextChanged();
    }
}

void MscEntity::addCif(const cif::CifBlockShared &cif)
{
    if (cif->blockType() == cif::CifLine::CifType::Requirement) {
        m_requirements.append(requirementsFromCifBlock(cif));
        Q_EMIT dataChanged();
        Q_EMIT cifTextChanged();
        return;
    }

    if (!m_cifs.contains(cif)) {
        m_cifs.append(cif);
        Q_EMIT dataChanged();
        Q_EMIT cifTextChanged();
    }
}

cif::CifBlockShared MscEntity::cifBlockByType(cif::CifLine::CifType type) const
{
    for (const cif::CifBlockShared &block : cifs())
        if (block->blockType() == type)
            return block;
    return cif::CifBlockShared();
}

void MscEntity::clearCifs()
{
    if (!m_cifs.isEmpty()) {
        m_cifs.clear();
        Q_EMIT dataChanged();
        Q_EMIT cifTextChanged();
    }
}

QString MscEntity::cifText(int tabsSize) const
{
    QStringList cifTexts;
    const QVector<cif::CifBlockShared> &cif = cifs();
    cifTexts.reserve(cif.size());
    for (const cif::CifBlockShared &cifBlock : cif) {
        if (cifBlock->blockType() != cif::CifLine::CifType::Comment) {
            cifTexts << cifBlock->toString(tabsSize);
        }
    }

    return cifTexts.join("\n");
}

QStringList MscEntity::requirements() const
{
    return m_requirements;
}

void MscEntity::setRequirements(const QStringList &requirements)
{
    if (requirements == m_requirements) {
        return;
    }

    m_requirements = requirements;
    Q_EMIT dataChanged();
    Q_EMIT cifTextChanged();
}

#ifdef QT_DEBUG
void MscEntity::dbgShowCifs() const
{
    const QVector<cif::CifBlockShared> &cif = cifs();
    qDebug() << QString("%1[%2]->CIFs %3:").arg(name(), entityTypeName()).arg(cif.size());
    for (int i = 0; i < cif.size(); ++i) {
        qDebug() << QString("\t CIF # %1/%2").arg(i).arg(cif.size());
        const cif::CifBlockShared &cifBlock = cif.at(i);
        for (int j = 0; j < cifBlock->lines().size(); ++j)
            qDebug() << QString("\t\t CIF Line #%1/%2: %3")
                                .arg(j)
                                .arg(cifBlock->lines().size())
                                .arg(cifBlock->lines().at(j)->sourceLine());
    }
}
#endif

QString MscEntity::toDbgString() const
{
    return QString("%1('%2')").arg(entityTypeName(), m_name);
}

cif::CifBlockShared MscEntity::requirementsCifBlock() const
{
    if (m_requirements.isEmpty()) {
        return {};
    }

    cif::CifBlockShared requirementCif =
            cif::CifBlockFactory::createBlock({ cif::CifLineShared(new cif::CifLineRequirement()) });
    const QString req = m_requirements.join(",");
    requirementCif->setPayload(QVariant::fromValue(req), cif::CifLine::CifType::Requirement);

    return requirementCif;
}

QStringList MscEntity::requirementsFromCifBlock(const cif::CifBlockShared &cif) const
{
    const QVariant &variant = cif->payload(cif::CifLine::CifType::Requirement);
    const QString data = variant.toString();
    if (data.isEmpty()) {
        return {};
    }

    return data.split(',');
}

} // namespace msc

QDebug operator<<(QDebug debug, const msc::MscEntity &entity)
{
    QDebugStateSaver saver(debug);
    debug << entity.toDbgString();
    return debug;
}

QDebug operator<<(QDebug debug, const msc::MscEntity *entity)
{
    QDebugStateSaver saver(debug);
    if (entity) {
        debug << entity->toDbgString();
    } else {
        debug << QString("%1(nullptr)").arg(msc::MscEntity::staticMetaObject.className());
    }
    return debug;
}
