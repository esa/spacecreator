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

#include "ivfunctiontype.h"

#include <QHash>
#include <QObject>
#include <QVector>

namespace ivm {

struct IVFunctionPrivate;
class IVFunction : public IVFunctionType
{
    Q_OBJECT
    Q_PROPERTY(QList<EntityAttribute> implementations READ implementations)
    Q_PROPERTY(QString defaultImplementation READ defaultImplementation WRITE setDefaultImplementation)

public:
    explicit IVFunction(QObject *parent = nullptr, const shared::Id &id = shared::InvalidId);
    ~IVFunction() override;

    bool postInit() override;

    void setInstanceOf(IVFunctionType *fnType);
    const IVFunctionType *instanceOf() const;
    bool inheritsFunctionType() const;

    const QList<EntityAttribute> &implementations() const;
    void setImplementation(int idx, const EntityAttribute &value);
    bool hasImplementationName(const QString &name) const;
    void addImplementation(const QString &name, const QString &language);
    void insertImplementation(int idx, const EntityAttribute &value);
    void removeImplementation(int idx);

    void setDefaultImplementation(const QString &name);
    QString defaultImplementation() const;

    bool isPseudoFunction() const;

Q_SIGNALS:
    void implemntationListChanged();

protected Q_SLOTS:
    void reflectAttr(const QString &attrName);
    void reflectContextParam();

private:
    const std::unique_ptr<IVFunctionPrivate> d;

    struct OriginalPropsHolder {
        // TODO: unite with IVInterface::OriginalPropsHolder
        EntityAttributes attrs;
        QList<EntityAttribute> implementations;
        QVector<shared::ContextParameter> params;
        QString defaultImplementation;

        QString name() const
        {
            const QString attrName = meta::Props::token(meta::Props::Token::name);
            return attrs.value(attrName).value<QString>();
        }
        inline bool collected() const { return m_collected; }
        inline void collect(const IVFunction *src)
        {
            if (m_collected || !src)
                return;

            attrs = src->entityAttributes();
            params = src->contextParams();
            implementations = src->implementations();
            defaultImplementation = src->defaultImplementation();

            m_collected = true;
        }

    private:
        bool m_collected { false };
    } m_originalFields;

    void cloneInternals();
    void restoreInternals();

    void reflectAttrs(const EntityAttributes &attrs);
    void reflectContextParams(const QVector<shared::ContextParameter> &params);
    void checkDefaultFunctionImplementation();
};

QDebug operator<<(QDebug debug, const IVFunction &function);

}
