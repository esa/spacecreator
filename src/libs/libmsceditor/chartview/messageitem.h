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

#include "baseitems/common/objectanchor.h"
#include "baseitems/eventitem.h"
#include "chartlayoutmanager.h"
#include "instanceitem.h"

#include <QPointer>
#include <mscmessage.h>

namespace msc {
class LabeledArrowItem;

class MessageItem : public EventItem
{
    Q_OBJECT
public:
    struct GeometryNotificationBlocker {
        explicit GeometryNotificationBlocker(MessageItem *target);
        ~GeometryNotificationBlocker();

    private:
        const QPointer<MessageItem> m_target;
        const bool m_storedPositionChangeIgnored;

        GeometryNotificationBlocker() = delete;
        GeometryNotificationBlocker(const GeometryNotificationBlocker &other) = delete;
        GeometryNotificationBlocker &operator=(const GeometryNotificationBlocker &) = delete;
    };

    explicit MessageItem(MscMessage *message, ChartLayoutManager *chartLayoutManager, InstanceItem *source = nullptr,
            InstanceItem *target = nullptr, QGraphicsItem *parent = nullptr);

    MscMessage *modelItem() const;

    void setInstances(InstanceItem *sourceInstance, InstanceItem *targetInstance);
    bool setSourceInstanceItem(InstanceItem *sourceInstance);
    InstanceItem *sourceInstanceItem() const;
    bool setTargetInstanceItem(InstanceItem *targetInstance);
    InstanceItem *targetInstanceItem() const;

    QString name() const;

    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    QRectF textBoundingRect() const;
    QPair<QPointF, bool> commentPoint() const override;

    void postCreatePolishing() override;

    QPointF head() const;
    void setHead(const QPointF &head, ObjectAnchor::Snap snap);
    void setHeadPosition(const QPointF &head);
    QPointF tail() const;
    void setTail(const QPointF &tail, ObjectAnchor::Snap snap);
    void setTailPosition(const QPointF &tail);
    void moveToYPosition(qreal yPos);

    bool isHorizontal() const;

    bool isAutoResizable() const;
    void setAutoResizable(bool resizable);

    static MessageItem *createDefaultItem(
            MscMessage *message, ChartLayoutManager *chartLayoutManager, const QPointF &pos);

    void performSnap();

    bool isCreator() const;

    void addMessagePoint(const QPointF &scenePoint);
    QVector<QPointF> messagePoints() const;
    void setMessagePoints(const QVector<QPointF> &scenePoints);

    void applyCif() override;
    void updateCif() override;

    QString displayedText() const;

    QPointF extendToNearestEdge(const QPointF &shiftMe) const;

    qreal instanceTopArea(MscInstance *instance) const override;
    qreal instanceBottomArea(MscInstance *instance) const override;

    QHash<msc::MscInstance *, int> visualIndices() const override;

Q_SIGNALS:
    void retargeted(msc::MessageItem *, const QPointF &pos, msc::MscMessage::EndType endType);

public Q_SLOTS:
    void setPositionChangeIgnored(bool ignored);
    void onChartBoxChanged();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void onManualMoveProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onManualResizeProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void updateGripPoints() override;

    void initGripPoints() override;
    cif::CifLine::CifType mainCifType() const override;

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

private Q_SLOTS:
    void onTextChanged();
    void showMessageDialog();
    void rebuildLayout() override;
    void commitGeometryChange();
    void onRenamed(const QString &title);
    void onManualGeometryChangeFinished(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to);
    void updateDisplayText();
    void checkIVConnection();

private:
    QString displayTextFromModel() const;

    bool updateSource(const QPointF &to, ObjectAnchor::Snap snap, InstanceItem *keepInstance = nullptr);
    bool updateTarget(const QPointF &to, ObjectAnchor::Snap snap, InstanceItem *keepInstance = nullptr);
    bool updateSourceAndTarget(const QPointF &shift);
    void updateTooltip();

    bool ignorePositionChange() const;
    bool proceedPositionChange() const;

    InstanceItem *hoveredInstance(const QPointF &hoverPoint, bool extendToBottom = false) const;

    void updateMessagePoints();

    cif::CifBlockShared mainCifBlock() const;
    cif::CifBlockShared positionCifBlock() const;

    void extendGlobalMessage();
    void applyTargetPointFromInstance();

    bool wannabeGlobal() const;
    QPointF validatePoint(const QPointF &requestedPoint, bool isSource, const QPointF &oppositePoint);

    bool ivConnectionOk() const;

    QPointer<msc::MscMessage> m_message = nullptr;
    LabeledArrowItem *m_arrowItem = nullptr;
    QPointer<InstanceItem> m_sourceInstance = nullptr;
    QPointer<InstanceItem> m_targetInstance = nullptr;
    bool m_posChangeIgnored = false;
    bool m_autoResize = true;
    bool m_preventRecursion = false;
    QVector<QPointF> m_originalMessagePoints;
};

} // namespace msc
