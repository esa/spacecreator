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
public:
    explicit IVFunction(const QString &title = QString(), QObject *parent = nullptr);
    ~IVFunction() override;

    bool postInit() override;

    void setInstanceOf(IVFunctionType *fnType);
    const IVFunctionType *instanceOf() const;
    bool inheritsFunctionType() const;

protected Q_SLOTS:
    void reflectAttr(ivm::meta::Props::Token attr);
    void reflectProp(ivm::meta::Props::Token prop);
    void reflectContextParam();

private:
    const std::unique_ptr<IVFunctionPrivate> d;

    struct OriginalPropsHolder {
        // TODO: unite with IVInterface::OriginalPropsHolder

        QString name() const { return attrs.value(meta::Props::token(meta::Props::Token::name)).toString(); }
        QHash<QString, QVariant> attrs;
        QHash<QString, QVariant> props;
        QVector<ContextParameter> params;

        inline bool collected() const { return m_collected; }

        inline void collect(const IVFunction *src)
        {
            if (m_collected || !src)
                return;

            attrs = src->attrs();
            props = src->props();
            params = src->contextParams();

            m_collected = true;
        }

    private:
        bool m_collected { false };
    } m_originalFields;

    void cloneInternals();
    void restoreInternals();

    void reflectAttrs(const QHash<QString, QVariant> &attrs);
    void reflectProps(const QHash<QString, QVariant> &props);
    void reflectContextParams(const QVector<ContextParameter> &params);
};

}
