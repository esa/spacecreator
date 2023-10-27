/*
  Copyright (C) 2018-2021 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see
  <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "veinteractiveobject.h"

#include "commands/cmdentitygeometrychange.h"
#include "commandsstackbase.h"
#include "graphicsviewutils.h"
#include "settingsmanager.h"
#include "textitem.h"
#include "veobject.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QTimer>

namespace shared {
namespace ui {

static const qreal kClickTreshold = 5;

VEInteractiveObject::VEInteractiveObject(VEObject *entity, QGraphicsItem *parent)
    : InteractiveObjectBase(parent)
    , m_dataObject(entity)
    , m_textItem(nullptr)
{
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges
            | QGraphicsItem::ItemIsSelectable);

    setCursor(Qt::ArrowCursor);

    connect(shared::ColorManager::instance(), &shared::ColorManager::colorsUpdated, this,
            &VEInteractiveObject::applyColorScheme);
    connect(m_dataObject, &VEObject::markerChanged, this, &VEInteractiveObject::applyColorScheme);

    connect(this, &VEInteractiveObject::boundingBoxChanged, this, &VEInteractiveObject::updateTextPosition);
}

void VEInteractiveObject::init()
{
    m_textItem = initTextItem();
    applyColorScheme();
}

void VEInteractiveObject::applyColorScheme()
{
    const shared::ColorHandler h = colorHandler();
    setPen(h.pen());
    setFont(h.font());
    setBrush(h.brush());

    if (m_textItem) {
        if (h.textAlignment() != ColorHandler::Default) {
            m_textItem->setTextAlignment(static_cast<Qt::Alignment>(h.textAlignment()));
        }
        m_textItem->setFont(h.font());
        m_textItem->setDefaultTextColor(h.textColor());
        updateTextPosition();
    }

    update();
}

TextItem *VEInteractiveObject::initTextItem()
{
    auto textItem = new TextItem(this);
    textItem->setEditable(true);
    textItem->setFont(font());
    textItem->setBackground(Qt::transparent);
    textItem->setTextWrapMode(QTextOption::NoWrap);
    textItem->setTextInteractionFlags(Qt::TextBrowserInteraction);
    textItem->setOpenExternalLinks(true);
    return textItem;
}

shared::ColorHandler VEInteractiveObject::colorHandler() const
{
    using namespace StyleAttribute;
    shared::ColorHandler h = InteractiveObjectBase::colorHandler();
    // Read color from entity (the model) and set it on the colorhandler
    shared::VEObject *ivObj = entity();
    if (!ivObj->isMarked()) {
        if (ivObj->hasEntityAttribute(QLatin1String("color"))) {
            h.detach();
        } else {
            const auto attrs = ivObj->entityAttributes();
            const bool hasStyleAttr = std::any_of(attrs.cbegin(), attrs.cend(),
                    [](const auto &attr) { return attr.name().startsWith(QLatin1String("Style::")); });
            if (hasStyleAttr) {
                h.detach();
            }
        }

        if (ivObj->hasEntityAttribute(QLatin1String("color"))) { // keep single custom color
            h.setFillType(shared::ColorHandler::FillType::Color);
            h.setBrushColor0(QColor(ivObj->entityAttributeValue<QString>(QLatin1String("color"))));
        }

        bool ok;
        if (ivObj->hasEntityAttribute(kPenWidth)) {
            const qreal penWidth = ivObj->entityAttributeValue(kPenWidth).toDouble(&ok);
            if (ok) {
                h.setPenWidth(penWidth);
            }
        }
        if (ivObj->hasEntityAttribute(kPenColor)) {
            const QString penColorName = ivObj->entityAttributeValue<QString>(kPenColor);
            if (QColor::isValidColorName(penColorName)) {
                h.setPenColor(penColorName);
            }
        }
        if (ivObj->hasEntityAttribute(kPenStyle)) {
            const QString penStyle = ivObj->entityAttributeValue<QString>(kPenStyle);
            h.setPenStyle(shared::typeFromName<Qt::PenStyle>(penStyle));
        }

        if (ivObj->hasEntityAttribute(kBrushFillType)) {
            const QString brushFillType = ivObj->entityAttributeValue<QString>(kBrushFillType);
            auto a = shared::typeFromName<ColorHandler::FillType>(brushFillType);
            h.setFillType(a);
        }
        if (ivObj->hasEntityAttribute(kBrushColor0)) {
            const QString brushColor0Name = ivObj->entityAttributeValue<QString>(kBrushColor0);
            if (QColor::isValidColorName(brushColor0Name)) {
                h.setBrushColor0(brushColor0Name);
            }
        }
        if (ivObj->hasEntityAttribute(kBrushColor1)) {
            const QString brushColor1Name = ivObj->entityAttributeValue<QString>(kBrushColor1);
            if (QColor::isValidColorName(brushColor1Name)) {
                h.setBrushColor1(brushColor1Name);
            }
        }

        if (ivObj->hasEntityAttribute(kTextColor)) {
            const QString textColorName = ivObj->entityAttributeValue<QString>(kTextColor);
            if (QColor::isValidColorName(textColorName)) {
                h.setTextColor(textColorName);
            }
        }
        if (ivObj->hasEntityAttribute(kTextAlignment)) {
            const QString textAlignment = ivObj->entityAttributeValue<QString>(kTextAlignment);
            h.setTextAlignment(shared::typeFromName<ColorHandler::Alignment>(textAlignment));
        }

        QFont handlerFont = h.font();
        if (ivObj->hasEntityAttribute(kFontFamily)) {
            const QString fontFamily = ivObj->entityAttributeValue<QString>(kFontFamily);
            handlerFont.setFamily(fontFamily);
        }
        if (ivObj->hasEntityAttribute(kFontPointSize)) {
            const qreal fontPointSize = ivObj->entityAttributeValue(kFontPointSize).toDouble(&ok);
            if (ok) {
                handlerFont.setPointSizeF(fontPointSize);
            }
        }
        if (ivObj->hasEntityAttribute(kFontWeight)) {
            const QString fontWeight = ivObj->entityAttributeValue<QString>(kFontWeight);
            handlerFont.setWeight(shared::typeFromName<QFont::Weight>(fontWeight));
        }
        h.setFont(handlerFont);
    }
    return h;
}

shared::VEObject *VEInteractiveObject::entity() const
{
    return m_dataObject;
}

void VEInteractiveObject::updateEntity()
{
    if (!m_commandsStack) {
        qWarning() << Q_FUNC_INFO << "No command stack set in shared::ui::VEInteractiveObject";
        return;
    }

    const auto changeGeometryCmd = new cmd::CmdEntityGeometryChange(prepareChangeCoordinatesCommandParams());
    m_commandsStack->push(changeGeometryCmd);
}

void VEInteractiveObject::updateText()
{
    if (!m_textItem) {
        return;
    }

    const QString text = entity()->titleUI();
    if (Qt::mightBeRichText(text)) {
        if (text != m_textItem->toHtml()) {
            m_textItem->setHtml(text);
        }
    } else if (text != m_textItem->toPlainText()) {
        m_textItem->setPlainText(text);
    }

    // This class cannot know what kind of text is being rendered, so
    // positioning is left to the children through a pure virtual method
    updateTextPosition();
}

void VEInteractiveObject::onSelectionChanged(bool isSelected)
{
    setZValue(itemLevel(isSelected));
    if (isSelected) {
        showGripPoints();
        updateGripPoints();
    } else {
        hideGripPoints();
    }
}

void VEInteractiveObject::childBoundingBoxChanged()
{
    scheduleLayoutUpdate();
}

void VEInteractiveObject::rebuildLayout()
{
    updateGripPoints();
    applyColorScheme();
}

void VEInteractiveObject::updateVisibility()
{
    setVisible(isItemVisible());
}

bool VEInteractiveObject::isItemVisible() const
{
    return true;
}

QList<QPair<shared::VEObject *, QVector<QPointF>>> VEInteractiveObject::prepareChangeCoordinatesCommandParams() const
{
    QList<QPair<shared::VEObject *, QVector<QPointF>>> params;
    auto children = childItems();
    std::stable_sort(children.begin(), children.end(),
            [](QGraphicsItem *item1, QGraphicsItem *item2) { return item1->type() < item2->type(); });

    for (auto item : qAsConst(children)) {
        if (auto iObj = qobject_cast<VEInteractiveObject *>(item->toGraphicsObject()))
            params.append(iObj->prepareChangeCoordinatesCommandParams());
    }
    params.erase(std::unique(params.begin(), params.end()), params.end());
    return params;
}

void VEInteractiveObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    s_mouseReleased = false;
    QGraphicsObject::mousePressEvent(event);
    onManualMoveStart(gripPointItem(shared::ui::GripPoint::Center), event->scenePos());

    if (SettingsManager::load<bool>(SettingsManager::Common::ShowHelpLines, true)) {
        showHelperLines(true);
    }
}

void VEInteractiveObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // We keep track of whether the mouse has been released already or not. If
    // the release happens at an edge, and it causes resizing the canvas,
    // sometimes it makes the scrollbar of the canvas resize and move, which
    // makes the graphics scene send an additional mouse move even that we want
    // to discard, or the element ends out of the screen.
    if (s_mouseReleased) {
        return;
    }
    QGraphicsObject::mouseMoveEvent(event);
    onManualMoveProgress(gripPointItem(shared::ui::GripPoint::Center), event->lastScenePos(), event->scenePos());
}

void VEInteractiveObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    showHelperLines(false);
    s_mouseReleased = true;
    onManualMoveFinish(gripPointItem(shared::ui::GripPoint::Center), event->buttonDownScenePos(event->button()),
            event->scenePos());

    const qreal distance =
            graphicsviewutils::distanceLine(event->buttonDownScenePos(event->button()), event->scenePos());
    if (distance <= kClickTreshold)
        Q_EMIT clicked(event->scenePos());
    QGraphicsObject::mouseReleaseEvent(event);
}

void VEInteractiveObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseDoubleClickEvent(event);
    showHelperLines(false);
    Q_EMIT doubleClicked();
}

void VEInteractiveObject::setCommandsStack(cmd::CommandsStackBase *commandsStack)
{
    m_commandsStack = commandsStack;
}

QString VEInteractiveObject::prepareTooltip() const
{
    return entity() ? entity()->titleUI() : QString();
}

QFont VEInteractiveObject::font() const
{
    return m_font;
}

void VEInteractiveObject::setFont(const QFont &font)
{
    m_font = font;
}

QBrush VEInteractiveObject::brush() const
{
    return m_brush;
}

void VEInteractiveObject::setBrush(const QBrush &brush)
{
    m_brush = brush;
}

QPen VEInteractiveObject::pen() const
{
    return m_pen;
}

void VEInteractiveObject::setPen(const QPen &pen)
{
    m_pen = pen;
}

QString VEInteractiveObject::toString() const
{
    QString typeName = QString("VEInteractiveObject: ");
    if (m_textItem == nullptr) {
        return typeName;
    }

    QString name;
    if (m_textItem->textIsValid()) {
        name = m_textItem->toPlainText();
    } else {
        name = "NoName";
    }

    auto br = sceneBoundingRect();
    auto x = br.x();
    auto y = br.y();
    auto w = br.width();
    auto h = br.height();
    auto result = QString(typeName + name + " rect: %1,%2, (%3,%4)").arg(x).arg(y).arg(w).arg(h);
    return result;
}

QDebug operator<<(QDebug debug, const shared::ui::VEInteractiveObject &veobj)
{
    debug << veobj.toString();
    return debug;
}
} // namespace ui
} // namespace shared
