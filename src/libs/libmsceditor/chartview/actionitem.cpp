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
#include "cif/cifblockfactory.h"
#include "cif/ciflines.h"
#include "colors/colormanager.h"
#include "commands/cmdactioninformaltext.h"
#include "datastatement.h"
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
    : EventItem(action, chartLayoutManager, parent)
    , m_action(action)
    , m_textItem(new ActionTextItem(this))
{
    Q_ASSERT(m_action != nullptr);

    setFlags(ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsSelectable);

    shared::ColorHandler color = shared::ColorManager::instance()->colorsForItem(shared::ColorManager::Action);
    m_textItem->setFramed(true);
    m_textItem->setFramePen(color.pen());
    m_textItem->setBackground(color.brush());
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

void ActionItem::applyCif()
{
    if (const cif::CifBlockShared &cifBlock = cifBlockByType(mainCifType())) {
        const QVector<QPoint> &cifPoints = cifBlock->payload().value<QVector<QPoint>>();
        if (cifPoints.size() == 2) {
            bool converted(false);
            const QVector<QPointF> &scenePoints = CoordinatesConverter::cifToScene(cifPoints, &converted);

            // All we care about is the Y - the rest is handled automatically
            setY(scenePoints.at(0).y());
            rebuildLayout();
        }
    }
}

void ActionItem::updateCif()
{
    if (!geometryManagedByCif()) {
        cif::CifBlockShared emptyCif = cif::CifBlockFactory::createBlockAction();
        emptyCif->addLine(cif::CifLineShared(new cif::CifLineAction()));
        m_entity->addCif(emptyCif);
    }

    const QRectF currentBBox = sceneBoundingRect();
    QRect bBoxCif;
    if (!CoordinatesConverter::sceneToCif(currentBBox, bBoxCif)) {
        qWarning() << Q_FUNC_INFO << "Can't convert bounding box coordinates to CIF";
        return;
    }

    cif::CifBlockShared cifBlock = cifBlockByType(mainCifType());
    Q_ASSERT(cifBlock != nullptr);

    const QVector<QPoint> &storedCif = cifBlock->payload().value<QVector<QPoint>>();
    const QVector<QPoint> newCif { bBoxCif.topLeft(), QPoint(bBoxCif.width(), bBoxCif.height()) };
    if (cifChangedEnough(storedCif, newCif)) {
        cifBlock->setPayload(QVariant::fromValue(newCif), mainCifType());
        Q_EMIT cifChanged();
    }
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
    m_textItem->setTextWrapMode(QTextOption::ManualWrap);
    const QSizeF nameSize(m_textItem->boundingRect().size());
    prepareGeometryChange();

    if (nameSize.width() > MAX_TEXT_WIDTH) {
        m_textItem->setTextWrapMode(QTextOption::WrapAnywhere);
        m_textItem->adjustSize();
    }

    setBoundingRect(m_textItem->boundingRect());
    centerOnTargetH();
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

} // namespace msc
