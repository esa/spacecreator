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

#include "msccomment.h"

#include <QDebug>
#include <QMetaEnum>

namespace msc {

const QRegExp MscEntity::m_nameVerify("([A-Z]|[a-z]|\\d|_|\\.)+");

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
const QRegExp &MscEntity::nameVerifier()
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
        disconnect(m_comment, 0, this, 0);
    }

    m_comment = comment;
    if (m_comment != nullptr) {
        connect(m_comment, &MscComment::textChanged, this, &MscEntity::commentChanged);
    }
    Q_EMIT commentChanged();
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

QVector<cif::CifBlockShared> MscEntity::cifs() const
{
    return m_cifs;
}

void MscEntity::setCifs(const QVector<cif::CifBlockShared> &cifs)
{
    if (m_cifs != cifs) {
        m_cifs = cifs;
        Q_EMIT dataChanged();
        Q_EMIT cifTextChanged();
    }
}

void MscEntity::addCif(const cif::CifBlockShared &cif)
{
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
    cifTexts.reserve(m_cifs.size());
    for (const cif::CifBlockShared &cifBlock : m_cifs) {
        if (cifBlock->blockType() != cif::CifLine::CifType::Comment) {
            cifTexts << cifBlock->toString(tabsSize);
        }
    }

    return cifTexts.join("\n");
}

#ifdef QT_DEBUG
void MscEntity::dbgShowCifs() const
{
    const QMetaEnum &e = QMetaEnum::fromType<MscEntity::EntityType>();
    const QString currTypeName(e.valueToKey(int(entityType())));
    qDebug() << QString("%1[%2]->CIFs %3:").arg(name(), currTypeName).arg(m_cifs.size());
    for (int i = 0; i < m_cifs.size(); ++i) {
        qDebug() << QString("\t CIF # %1/%2").arg(i).arg(m_cifs.size());
        const cif::CifBlockShared &cifBlock = m_cifs.at(i);
        for (int j = 0; j < cifBlock->lines().size(); ++j)
            qDebug() << QString("\t\t CIF Line #%1/%2: %3")
                                .arg(j)
                                .arg(cifBlock->lines().size())
                                .arg(cifBlock->lines().at(j)->sourceLine());
    }
}
#endif

} // namespace msc
