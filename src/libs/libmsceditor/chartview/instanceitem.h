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

#include "baseitems/interactiveobject.h"

#include <QLineF>
#include <QPointer>
#include <QRectF>

class QPainterPath;

namespace msc {

namespace ui {
class GripPoint;
}

class MscInstance;
class MscChart;

class ChartLayoutManager;
class InstanceHeadItem;
class InstanceEndItem;
class MessageItem;
class TextItem;

/*!
   \brief The InstanceItem class is the graphical representation of an MSC instance.

   \note The CIF geometry (for user defined positions) only applies to the X. All the Y information is calculated
   automatically. As the user can only control the horizontal position.
 */
class InstanceItem : public InteractiveObject
{
    Q_OBJECT

public:
    explicit InstanceItem(MscInstance *instance, ChartLayoutManager *chartLayoutManager, MscChart *chart = nullptr,
            QGraphicsItem *parent = nullptr);

    MscInstance *modelItem() const;

    QString name() const;
    QString kind() const;

    void setAxisHeight(qreal height);

    qreal axisHeight() const;
    QLineF axis() const;

    QPainterPath shape() const override;
    void setGeometry(const QRectF &geometry);

    QRectF extendedSceneBoundingRect() const;

    static InstanceItem *createDefaultItem(
            ChartLayoutManager *model, MscInstance *instance, MscChart *chart, const QPointF &pos);

    QPair<QPointF, bool> commentPoint() const override;

    void postCreatePolishing() override;

    void applyCif() override;
    void updateCif() override;

    QRectF kindBox() const;

    InstanceHeadItem *headerItem() const;
    InstanceEndItem *endItem() const;

    static qreal defaultAxisHeight();

    QPointF leftCreatorTarget() const;
    QPointF rightCreatorTarget() const;

public Q_SLOTS:
    void setName(const QString &name);
    void setDenominatorAndKind(const QString &kind);
    void setExplicitStop(bool exStop);
    void rebuildLayout() override;
    void syncHeightToChartBox();

Q_SIGNALS:
    void initialNameAccepted(MscEntity *entity);

protected:
    void onManualMoveProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;

    void initGripPoints() override;
    cif::CifLine::CifType mainCifType() const override;

#ifdef QT_DEBUG
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
#endif

private Q_SLOTS:
    void onNameEdited(const QString &newName);
    void onKindEdited(const QString &newKind);
    void checkforInitialName();
    void checkIVFunction();

private:
    qreal m_axisHeight = defaultAxisHeight();

    Q_INVOKABLE void reflectTextLayoutChange();

    void updatePropertyString(const QLatin1String &property, const QString &value);

    QVector<QPoint> prepareChangePositionCommand() const;

    bool ivFunctionOk() const;

    msc::MscInstance *m_instance = nullptr;
    QPointer<msc::MscChart> m_chart;
    QGraphicsLineItem *m_axisSymbol = nullptr;
    InstanceHeadItem *m_headSymbol = nullptr;
    InstanceEndItem *m_endSymbol = nullptr;
    QString m_initialName;
};

} // namespace msc
