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

#include "implementationsmodel.h"

#include "commands/cmdfunctionattrchange.h"
#include "commands/cmdfunctionlanguageinsert.h"
#include "commands/cmdfunctionlanguageremove.h"
#include "commands/cmdfunctionlanguageupdate.h"
#include "ivcore/abstractsystemchecks.h"
#include "ivfunction.h"
#include "ivmodel.h"

#include <QDebug>
#include <QFont>
#include <QMetaEnum>

namespace ive {

ImplementationsModel::ImplementationsModel(
        ivm::AbstractSystemChecks *checks, cmd::CommandsStack::Macro *macro, QObject *parent)
    : QAbstractItemModel(parent)
    , m_checks(checks)
    , m_cmdMacro(macro)
{
}

void ImplementationsModel::setFunction(ivm::IVFunction *fn)
{
    beginResetModel();
    m_function = fn;
    endResetModel();
}

QVariant ImplementationsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case Column::Name:
            return tr("Name");
        case Column::Language:
            return tr("Language");
        case Column::Default:
            return tr("Default");
        }
    }
    return QVariant();
}

QModelIndex ImplementationsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex ImplementationsModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    // no nesting levels -> no parents
    return QModelIndex();
}

int ImplementationsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (!m_function) {
        return 0;
    }

    return m_function->implementations().count();
}

int ImplementationsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    static const QMetaEnum &me = QMetaEnum::fromType<Column>();
    return me.keyCount();
}

QVariant ImplementationsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_function || m_function->implementations().size() <= index.row()) {
        return QVariant();
    }

    EntityAttribute attribute = m_function->implementations().at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case Column::Name:
            return QVariant(attribute.name());
        case Column::Language:
            return QVariant(attribute.value());
        }
        return QVariant();
    }

    if (role == Qt::CheckStateRole && index.column() == Column::Default) {
        return m_function->defaultImplementation() == attribute.name() ? Qt::CheckState::Checked
                                                                       : Qt::CheckState::Unchecked;
    }

    if (role == Qt::FontRole && isImplementationUsed(index.row())) {
        QFont font;
        font.setItalic(true);
        return QVariant::fromValue(font);
    }

    return QVariant();
}

bool ImplementationsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || !m_function || m_function->implementations().size() <= index.row()) {
        return false;
    }

    auto setDefaultLanguage = [this](const QString &name, const QString &language) {
        QVariantHash attrs = { { ivm::meta::Props::token(ivm::meta::Props::Token::default_implementation), name },
            { ivm::meta::Props::token(ivm::meta::Props::Token::language), language } };
        auto cmd = new cmd::CmdFunctionAttrChange(m_function, attrs);
        m_cmdMacro->push(cmd);
    };

    if (index.column() == Column::Default) {
        if (role == Qt::CheckStateRole) {
            QModelIndex oldDefault = defaultIndex();
            if (index == oldDefault) {
                return false;
            }
            setDefaultLanguage(data(createIndex(index.row(), Column::Name)).toString(),
                    data(createIndex(index.row(), Column::Language)).toString());
            Q_EMIT dataChanged(oldDefault, oldDefault, QVector<int>() << role);
            Q_EMIT dataChanged(index, index, QVector<int>() << role);
            return true;
        }
    }
    if ((role == Qt::DisplayRole || role == Qt::EditRole)
            && (index.column() == Column::Name || index.column() == Column::Language)) {
        QModelIndex defaultIdx = createIndex(index.row(), Column::Default);
        const bool isDefault = data(defaultIdx, Qt::CheckStateRole) == Qt::CheckState::Checked;
        EntityAttribute language = m_function->implementations().at(index.row());
        if (index.column() == Column::Name) {
            QString name = uniqueName(value.toString());
            language.setName(name);
        }
        if (index.column() == Column::Language) {
            language.setValue(value.toString());
        }
        auto cmd = new cmd::CmdFunctionLanguageUpdate(m_function, index.row(), language);
        m_cmdMacro->push(cmd);
        Q_EMIT dataChanged(index, index, QVector<int>() << role);

        if (isDefault) {
            setDefaultLanguage(language.name(), language.value().toString());
            Q_EMIT dataChanged(defaultIdx, defaultIdx, QVector<int>() << Qt::CheckStateRole);
        }

        return true;
    }

    return false;
}

Qt::ItemFlags ImplementationsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    if (index.column() == Column::Default) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool ImplementationsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (!m_function) {
        return false;
    }
    beginInsertRows(parent, row, row + count - 1);
    const QString name("new_name");
    const QString defaultLanguage = m_function->model()->defaultFunctionLanguage();
    for (int i = 0; i < count; ++i) {
        EntityAttribute language(
                uniqueName(name), QVariant::fromValue(defaultLanguage), EntityAttribute::Type::Attribute);
        auto cmd = new cmd::CmdFunctionLanguageInsert(m_function, row + i, language);
        m_cmdMacro->push(cmd);
    }
    endInsertRows();
    return true;
}

bool ImplementationsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!m_function) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        auto cmd = new cmd::CmdFunctionLanguageRemove(m_function, row + i);
        m_cmdMacro->push(cmd);
    }
    endRemoveRows();
    return true;
}

QModelIndex ImplementationsModel::defaultIndex() const
{
    if (!m_function) {
        return QModelIndex();
    }

    int row = 0;
    for (const EntityAttribute &attribute : m_function->implementations()) {
        if (attribute.name() == m_function->defaultImplementation()) {
            return index(row, Column::Default);
        }
        ++row;
    }

    return QModelIndex();
}

bool ImplementationsModel::isImplementationUsed(int row) const
{
    if (!m_checks || row < 0 || row >= m_function->implementations().size()) {
        return false;
    }

    const QString name = m_function->implementations().at(row).name();
    return m_checks->isImplementationUsed(m_function, name);
}

QString ImplementationsModel::uniqueName(const QString &name)
{
    if (!m_function) {
        return name;
    }

    QString checkedName = name;
    int count = 0;
    while (m_function->hasImplementationName(checkedName)) {
        checkedName = name + QString("_%1").arg(++count);
    }

    return checkedName;
}

} // namespace ive
