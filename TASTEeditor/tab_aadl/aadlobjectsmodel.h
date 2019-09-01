/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlobjectcontainer.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"
#include "aadlobjectconnection.h"
#include "aadlobjectcomment.h"

#include <QObject>
#include <QVector>
#include <memory>

namespace taste3 {
namespace aadl {

struct AADLObjectsModelPrivate;
class AADLObjectsModel : public QObject
{
    Q_OBJECT
public:
    explicit AADLObjectsModel(QObject *parent = nullptr);
    ~AADLObjectsModel() override;

    bool addObject(AADLObject* obj);
    bool removeObject(AADLObject* obj);

    AADLObject* getObject( const common::Id& id ) const;
    AADLObjectFunction* getFunction(const common::Id& id) const;
    AADLObjectContainer* getContainer(const common::Id& id) const;
    AADLObjectIfaceRequired* getRequiredInterface(const common::Id& id) const;
    AADLObjectIfaceProvided* getProvidedInterface(const common::Id& id) const;
    AADLObjectConnection* getConnection(const common::Id& id) const;
    AADLObjectComment* getCommentById(const common::Id& id) const;

signals:
    void aadlObjectAdded(AADLObject *object);
    void aadlObjectRemoved(AADLObject *object);

public slots:
    bool initFromObjects(const QVector<AADLObject *>& objects);

private:
    const std::unique_ptr<AADLObjectsModelPrivate> d;
};

} // ns aadl
} // ns taste3
