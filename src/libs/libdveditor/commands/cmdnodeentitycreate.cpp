/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdnodeentitycreate.h"

#include "commandids.h"
#include "dvboard.h"
#include "dvcommonprops.h"
#include "dvdevice.h"
#include "dvmodel.h"
#include "dvnamevalidator.h"
#include "dvnode.h"
#include "dvpartition.h"
#include "dvsystemfunction.h"
#include "dvsysteminterface.h"
#include "graphicsviewutils.h"

#include <QRectF>

namespace dve {
namespace cmd {

CmdNodeEntityCreate::CmdNodeEntityCreate(dvm::DVModel *model, const dvm::DVBoard *board, const QPointF &pos)
    : shared::cmd::CmdEntityGeometryChange({}, tr("Create Board"))
    , m_model(model)
    , m_board(board)
    , m_pos(pos)
    , m_node(new dvm::DVNode(*board))
    , m_partition(new dvm::DVPartition(m_node))
{
    m_node->setTitle(dvm::DVNameValidator::nameForObject(m_node, m_model, m_board->title()));
    const QRectF geometry { QRectF(pos, shared::graphicsviewutils::kDefaultNodeGraphicsItemSize) };
    prepareData({ qMakePair(m_node, QVector<QPointF> { geometry.topLeft(), geometry.bottomRight() }) });

    m_partition->setTitle(dvm::DVNameValidator::nameForObject(m_partition, m_model));
    m_node->addPartition(m_partition);

    for (const dvm::DVPort *port : m_board->ports()) {
        auto nodeDevice = new dvm::DVDevice(*port, m_node);
        m_node->addDevice(nodeDevice);
        m_devices.append(nodeDevice);
    }
    for (const dvm::DVSystemFunction *sfunc : m_board->systemFunctions()) {
        auto newFunc = new dvm::DVSystemFunction(*sfunc, m_node);
        for (const dvm::DVSystemInterface *sface : sfunc->interfaces()) {
            new dvm::DVSystemInterface(*sface, newFunc);
        }
    }
}

CmdNodeEntityCreate::~CmdNodeEntityCreate()
{
    if (m_node && !m_node->parent()) {
        delete m_node;
    }
}

void CmdNodeEntityCreate::redo()
{
    shared::cmd::CmdEntityGeometryChange::redo();

    m_node->setParent(m_model);

    if (m_model) {
        m_model->addObject(m_node);
        m_model->addObject(m_partition);
        for (dvm::DVDevice *dev : qAsConst(m_devices)) {
            m_model->addObject(dev);
        }
        for (dvm::DVSystemFunction *sfunc : m_node->systemFunctions()) {
            m_model->addObject(sfunc);
            for (dvm::DVSystemInterface *sface : sfunc->interfaces()) {
                m_model->addObject(sface);
            }
        }
    }
}

void CmdNodeEntityCreate::undo()
{
    if (m_model) {
        for (dvm::DVSystemFunction *sfunc : m_node->systemFunctions()) {
            for (dvm::DVSystemInterface *sface : sfunc->interfaces()) {
                m_model->removeObject(sface);
            }
            m_model->removeObject(sfunc);
        }
        for (dvm::DVDevice *dev : qAsConst(m_devices)) {
            m_model->removeObject(dev);
        }
        m_model->removeObject(m_partition);
        m_model->removeObject(m_node);
    }

    m_node->setParent(this);

    shared::cmd::CmdEntityGeometryChange::undo();
}

int CmdNodeEntityCreate::id() const
{
    return CreateBoardEntity;
}

} // namespace cmd
} // namespace dve
