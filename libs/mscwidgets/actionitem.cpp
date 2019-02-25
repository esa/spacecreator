/*
   Copyright (C) 201 European Space Agency - <maxime.perrotin@esa.int>

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

#include "actionitem.h"

#include "baseitems/textitem.h"
#include "commands/common/commandsstack.h"
#include "instanceitem.h"
#include "mscaction.h"

#include <QDebug>
#include <QPainter>

namespace msc {

ActionItem::ActionItem(msc::MscAction *action, QGraphicsItem *parent)
    : InteractiveObject(action, parent)
    , m_action(action)
    , m_textItem(new NameItem(this))
{
    Q_ASSERT(m_action != nullptr);

    setFlags(ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsSelectable);

    m_textItem->setFramed(true);
    m_textItem->setEditable(true);
    m_textItem->setHtml(actionText());

    connect(m_action, &msc::MscAction::informalActionChanged, this, &msc::ActionItem::setActionText);

    connect(m_textItem, &TextItem::edited, this, &ActionItem::onTextEdited, Qt::QueuedConnection);
    connect(m_textItem, &TextItem::keyPressed, this, &ActionItem::updateLayout);

    m_boundingRect = m_textItem->boundingRect();
    updateLayout();
}

MscAction *ActionItem::modelItem() const
{
    return m_action;
}

void ActionItem::connectObjects(InstanceItem *instance, qreal y)
{
    setY(y);
    setInstance(instance);
}

void ActionItem::setInstance(InstanceItem *instance)
{
    if (instance == m_instance) {
        return;
    }

    if (m_instance) {
        disconnect(m_instance, nullptr, this, nullptr);
    }

    m_instance = instance;
    if (m_instance) {
        connect(m_instance, &InteractiveObject::relocated, this, &ActionItem::onInstanceMoved, Qt::DirectConnection);
        m_action->setInstance(m_instance->modelItem());
    } else {
        m_action->setInstance(nullptr);
    }

    updateLayout();
}

void ActionItem::setActionText(const QString &text)
{
    if (!m_action) {
        return;
    }

    m_action->setInformalAction(text);
    m_textItem->setHtml(actionText());

    updateLayout();
}

void ActionItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    InteractiveObject::mouseDoubleClickEvent(event);
    m_textItem->enableEditMode();
}

void ActionItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gp->location() == GripPoint::Location::Center) {
        const QPointF &delta = to - from;
        setPos(pos() + delta);
    }
}

void ActionItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

void ActionItem::prepareHoverMark()
{
    InteractiveObject::prepareHoverMark();
    m_gripPoints->setUsedPoints({ GripPoint::Location::Center });

    connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeFinish, this,
            &ActionItem::onManualGeometryChangeFinished, Qt::UniqueConnection);
}

void ActionItem::onTextEdited(const QString &text)
{
    if (text.isEmpty()) {
        return;
    }

    using namespace msc::cmd;
    CommandsStack::push(InformatActionText, { QVariant::fromValue<MscEntity *>(this->modelItem()), text });
}

void ActionItem::rebuildLayout()
{
    if (!m_instance) {
        return;
    }

    prepareGeometryChange();

    m_boundingRect = m_textItem->boundingRect();
    const double x = m_instance->centerInScene().x() - m_boundingRect.width() / 2;
    setX(x);
}

void ActionItem::onInstanceMoved(const QPointF &from, const QPointF &to)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    updateLayout();
}

void ActionItem::onManualGeometryChangeFinished(GripPoint::Location pos, const QPointF &, const QPointF &)
{
    if (pos == GripPoint::Location::Center) {
        Q_EMIT moved(this);
    }
}

QString ActionItem::actionText() const
{
    if (m_action->actionType() == MscAction::ActionType::Informal) {
        return m_action->informalAction();
    } else {
        QString text;
        for (const MscAction::DataStatement &statement : m_action->dataStatements()) {
            if (!text.isEmpty()) {
                text += ", ";
            }
            switch (statement.m_type) {
            case MscAction::DataStatement::StatementType::Define:
                text += "<b>define</b> " + statement.m_variableString;
                break;
            case MscAction::DataStatement::StatementType::UnDefine:
                text += "<b>undefine</b> " + statement.m_variableString;
                break;
            case MscAction::DataStatement::StatementType::Binding:
                qWarning() << "Binding actions is not yet supported";
                continue;
            }
        }
        return text;
    }
}

} // namespace msc
