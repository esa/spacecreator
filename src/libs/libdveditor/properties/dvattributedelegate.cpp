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

#include "dvattributedelegate.h"

#include "asn1/file.h"
#include "asn1editor.h"
#include "asn1modelstorage.h"
#include "asn1reader.h"
#include "asn1systemchecks.h"
#include "dvcommonprops.h"
#include "propertieslistmodel.h"

#include <QEvent>
#include <QLabel>
#include <QtDebug>

static const char *MODEL_INDEX_PROPERTY = "modelIndex";

namespace dve {

DVAttributeDelegate::DVAttributeDelegate(Asn1Acn::Asn1SystemChecks *asn1Checks, QObject *parent)
    : shared::AttributeDelegate(parent)
    , m_asn1Checks(asn1Checks)
{
}

QWidget *DVAttributeDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        const QString name = shared::PropertiesListModel::tokenNameFromIndex(index);
        if (name == dvm::meta::Props::token(dvm::meta::Props::Token::config)) {
            auto *proxy = new QLabel(parent);
            proxy->installEventFilter(const_cast<DVAttributeDelegate *>(this));
            return proxy;
        }
    }
    return shared::AttributeDelegate::createEditor(parent, option, index);
}

void DVAttributeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.isValid()) {
        const QString name = shared::PropertiesListModel::tokenNameFromIndex(index);
        if (name == dvm::meta::Props::token(dvm::meta::Props::Token::config)) {
            const QString dataText = index.data(shared::PropertiesListModel::DataRole).toString();
            auto *proxy = qobject_cast<QLabel *>(editor);
            Q_ASSERT(proxy);
            proxy->setText(dataText);
            proxy->setProperty(MODEL_INDEX_PROPERTY, QVariant::fromValue(QPersistentModelIndex(index)));
            return;
        }
    }
    return shared::AttributeDelegate::setEditorData(editor, index);
}

bool DVAttributeDelegate::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (auto label = qobject_cast<QLabel *>(object)) {
            auto modelIndex = object->property(MODEL_INDEX_PROPERTY).toPersistentModelIndex();
            if (!modelIndex.isValid()) {
                return shared::AttributeDelegate::eventFilter(object, event);
            }

            auto getAttrValue = [modelIndex](const QString &attrName) {
                if (auto model = modelIndex.model()) {
                    const QModelIndex startIndex = model->index(0, shared::PropertiesListModel::Column::Name);
                    const QModelIndexList indexList =
                            model->match(startIndex, shared::PropertiesListModel::DataRole, attrName);
                    if (!indexList.isEmpty()) {
                        const QModelIndex valueIndex =
                                indexList.first().siblingAtColumn(shared::PropertiesListModel::Column::Value);
                        const QString asn1file = valueIndex.data(shared::PropertiesListModel::DataRole).toString();
                        return asn1file;
                    }
                }
                return QString();
            };

            std::unique_ptr<Asn1Acn::File> asn1Data;
            const QString asn1fileAttrName = dvm::meta::Props::token(dvm::meta::Props::Token::asn1file);
            const QString asn1file = getAttrValue(asn1fileAttrName);
            if (!asn1file.isEmpty()) {
                QStringList errorMessages;
                Asn1Acn::Asn1Reader parser;
                asn1Data = parser.parseAsn1File(QFileInfo(asn1file), &errorMessages);
                if (!asn1Data) {
                    qWarning() << "Can't read file" << asn1file << ":" << errorMessages.join(", ");
                    return {};
                }
            }

            auto dialog = new asn1::Asn1Editor(m_asn1Checks, std::move(asn1Data), label->window());
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->setModal(true);

            const QString asn1typeAttrName = dvm::meta::Props::token(dvm::meta::Props::Token::asn1type);
            const QString asn1type = getAttrValue(asn1typeAttrName);
            if (!asn1type.isEmpty()) {
                dialog->showAsn1Type(asn1type);
            }
            dialog->setValue(label->text());
            connect(dialog, &asn1::Asn1Editor::accepted, this, [this, modelIndex]() {
                if (auto dialog = qobject_cast<asn1::Asn1Editor *>(sender())) {
                    auto model = const_cast<QAbstractItemModel *>(modelIndex.model());
                    const QString newValue = dialog->value();
                    if (model && newValue != model->data(modelIndex).toString()) {
                        model->setData(modelIndex, newValue);
                    }
                }
            });
            dialog->show();
            return true;
        }
    }
    return false;
}

} // namespace dve
