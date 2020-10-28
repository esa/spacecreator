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

#include "actionitem.h"

#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/textitem.h"
#include "chartlayoutmanager.h"
#include "commands/cmdactioninformaltext.h"
#include "datastatement.h"
#include "instanceitem.h"
#include "mscaction.h"
#include "mscchartviewconstants.h"
#include "msccommandsstack.h"
#include "mscreader.h"
#include "ui/grippointshandler.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <cmath>

namespace msc {

class ActionTextItem : public TextItem
{
public:
    ActionTextItem(QGraphicsItem *parent = nullptr)
        : TextItem(parent)
    {
    }

protected:
    bool validateText(const QString &text) const override
    {
        MscReader reader;
        QStringList errors;
        // test plain informal action, or data statements
        QString testDoc = QString("msc c1;instance i1;action %1;endinstance;endmsc;").arg(text);
        try {
            reader.parseText(testDoc, &errors);
        } catch (...) {
            // test quoted informal action
            testDoc = QString("msc c1;instance i1;action '%1';endinstance;endmsc;").arg(text);
            try {
                reader.parseText(testDoc, &errors);
            } catch (...) {
                return false;
            }
        }
        return errors.isEmpty();
    }
};

/*!
 * \class msc::ActionItem
 *
 * This class shows an action
 */
ActionItem::ActionItem(msc::MscAction *action, ChartLayoutManager *chartLayoutManager, QGraphicsItem *parent)
    : InteractiveObject(action, chartLayoutManager, parent)
    , m_action(action)
    , m_textItem(new ActionTextItem(this))
{
    Q_ASSERT(m_action != nullptr);

    setFlags(ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsSelectable);

    m_textItem->setFramed(true);
    m_textItem->setEditable(true);
    m_textItem->setHtml(actionText());
    m_textItem->setFlag(QGraphicsItem::ItemStacksBehindParent, true);

    connect(m_action, &msc::MscAction::informalActionChanged, this, &msc::ActionItem::setActionText);

    connect(m_textItem, &TextItem::edited, this, &ActionItem::onTextEdited, Qt::QueuedConnection);
    connect(m_textItem, &TextItem::textChanged, this, &ActionItem::scheduleLayoutUpdate);

    setBoundingRect(m_textItem->boundingRect());
    scheduleLayoutUpdate();
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

/*!
 * \brief ActionItem::setInstance Update the instance this action is placed on
 * \param instance
 */
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

    scheduleLayoutUpdate();
}

void ActionItem::setActionText(const QString &text)
{
    if (!m_action) {
        return;
    }

    m_action->setInformalAction(text);
    m_textItem->setHtml(actionText());

    scheduleLayoutUpdate();
}

void ActionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    InteractiveObject::mousePressEvent(event);
    setSelected(true);
    event->accept();
}

void ActionItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    InteractiveObject::mouseDoubleClickEvent(event);
    m_textItem->enableEditMode();
    event->accept();
}

void ActionItem::onManualMoveProgress(shared::ui::GripPoint *, const QPointF &from, const QPointF &to)
{
    moveBy(0., to.y() - from.y());
}

void ActionItem::onManualMoveFinish(shared::ui::GripPoint *, const QPointF &, const QPointF &)
{
    Q_EMIT moved(this);
}

void ActionItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    gripPointsHandler()->setUsedPoints({ shared::ui::GripPoint::Location::Center });
}

void ActionItem::onTextEdited(const QString &text)
{
    if (text.isEmpty()) {
        return;
    }

    m_chartLayoutManager->undoStack()->push(
            new cmd::CmdActionInformalText(this->modelItem(), text, m_chartLayoutManager));
}

void ActionItem::rebuildLayout()
{
    if (!m_instance)
        return;

    m_textItem->setTextWrapMode(QTextOption::ManualWrap);
    const QSizeF nameSize(m_textItem->boundingRect().size());
    prepareGeometryChange();

    if (nameSize.width() > MAX_TEXT_WIDTH) {
        m_textItem->setTextWrapMode(QTextOption::WrapAnywhere);
        m_textItem->adjustSize();
    }

    setBoundingRect(m_textItem->boundingRect());
    const double x = m_instance->centerInScene().x() - boundingRect().width() / 2;
    if (std::abs(x - this->x()) > 1e-3) {
        setX(x);
    }
}

void ActionItem::onInstanceMoved(const QPointF &from, const QPointF &to)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    instantLayoutUpdate();
}

QString ActionItem::actionText() const
{
    if (m_action->actionType() == MscAction::ActionType::Informal) {
        return m_action->informalAction();
    } else {
        QString text;
        for (const msc::DataStatement *statement : m_action->dataStatements()) {
            if (!text.isEmpty()) {
                text += ", ";
            }
            switch (statement->type()) {
            case msc::DataStatement::StatementType::Define:
                text += "<b>define</b> " + statement->variableString();
                break;
            case msc::DataStatement::StatementType::UnDefine:
                text += "<b>undefine</b> " + statement->variableString();
                break;
            case msc::DataStatement::StatementType::Binding:
                qWarning() << "Binding actions is not yet supported";
                continue;
            }
        }
        return text;
    }
}

cif::CifLine::CifType ActionItem::mainCifType() const
{
    return cif::CifLine::CifType::Action;
}

void ActionItem::applyCif()
{
    if (const cif::CifBlockShared &cifBlock = cifBlockByType(cif::CifLine::CifType::Action)) {
        const QVector<QPoint> &cifPoints = cifBlock->payload().value<QVector<QPoint>>();
        if (cifPoints.size() == 2) {
            bool converted(false);
            const QVector<QPointF> &scenePoints = CoordinatesConverter::cifToScene(cifPoints, &converted);

            const QPointF &textBoxTopLeft = scenePoints.at(0);
            const QPointF &textBoxSize = scenePoints.at(1);

            QSignalBlocker keepSilent(this);
            setBoundingRect(m_textItem->boundingRect());

            m_textItem->setExplicitSize({ textBoxSize.x(), textBoxSize.y() });
            const QPointF shift = textBoxTopLeft - pos();

            moveBy(shift.x(), shift.y());
        }
    }
}

} // namespace msc
