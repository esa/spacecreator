#include <QStandardItem>
#include <QStandardItemModel>

#include "asn1itemdelegate.h"
#include "asn1treeview.h"

Asn1TreeView::Asn1TreeView(QWidget *parent) : QTreeView(parent)
{
    m_model = new QStandardItemModel(1, 4);
    QStringList headers = QStringList() << tr("Field")
                                        << tr("Type")
                                        << tr("Value")
                                        << tr("Present");
    m_model->setHorizontalHeaderLabels(headers);
    setModel(m_model);

    m_itemDelegate = new Asn1ItemDelegate(this);
    setItemDelegate(m_itemDelegate);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors(true);
    setDragEnabled(false);

    connect(m_itemDelegate, &Asn1ItemDelegate::sequenceOfSizeChanged, this, &Asn1TreeView::onSequenceOfSizeChanged);
    connect(m_itemDelegate, &Asn1ItemDelegate::choiceFieldChanged, this, &Asn1TreeView::onChoiceFieldChanged);
}

void Asn1TreeView::setAsn1Model(QVariantMap asn1Item, int row)
{
    ItemMap itemMap = createModelItems(asn1Item);

    m_rootTreeItem = itemMap["item"];

    qobject_cast<QStandardItemModel *>(model())->setItem(row, 0, m_rootTreeItem);
    qobject_cast<QStandardItemModel *>(model())->setItem(row, 1, itemMap["type"]);
    qobject_cast<QStandardItemModel *>(model())->setItem(row, 2, itemMap["value"]);

    hideExtraFields(m_rootTreeItem, true, row);

    expandAll();
}

void Asn1TreeView::onSequenceOfSizeChanged(const QModelIndex &index, QVariant value, QVariant maxRange)
{
    for (int x = 0;  x < maxRange.toInt(); ++x)
        setRowHidden(x, index, x  < value.toInt() ? false : true);

    expand(index);
}

void Asn1TreeView::onChoiceFieldChanged(const QModelIndex &index, QVariant length, QVariant currentIndex)
{
    for (int x = 0;  x < length.toInt(); ++x)
        setRowHidden(x, index, x  == currentIndex.toInt() ? false : true);

    expand(index);
}

Asn1TreeView::ItemMap Asn1TreeView::createModelItems(QVariantMap asn1Item)
{
    ItemMap itemMap;
    QString typeLimit;

    itemMap["item"] = new QStandardItem(asn1Item["id"].toString());
    itemMap["item"]->setEditable(false);

    if (asn1Item["type"] == "integer" || asn1Item["type"] == "double") {
        addNumberItem(asn1Item, itemMap);

        typeLimit = QString(" (%1..%2)").arg(asn1Item["min"].toString(), asn1Item["max"].toString());
    }
    else if (asn1Item["type"] == "bool")
        addBoolItem(asn1Item, itemMap);
    else if (asn1Item["type"] == "sequence")
        addSequenceItem(asn1Item, itemMap);
    else if (asn1Item["type"] == "sequenceOf") {
        addSequenceOfItem(asn1Item, itemMap);

        if (asn1Item["min"] == asn1Item["max"])
            typeLimit = QString(tr(" Size(%1)")).arg(asn1Item["min"].toString());
        else
            typeLimit = QString(tr(" Size(%1..%2)")).arg(asn1Item["min"].toString(), asn1Item["max"].toString());
    }
    else if (asn1Item["type"] == "enumerated")
        addEnumeratedItem(asn1Item, itemMap);
    else if (asn1Item["type"] == "choice")
        addChoiceItem(asn1Item, itemMap);
    else if (asn1Item["type"] == "string") {
        addStringItem(asn1Item, itemMap);

        if (asn1Item["min"] == asn1Item["max"])
            typeLimit = QString(tr(" Length(%1)")).arg(asn1Item["min"].toString());
        else
            typeLimit = QString(tr(" Length(%1..%2)")).arg(asn1Item["min"].toString(), asn1Item["max"].toString());
    }

    itemMap["type"] = new QStandardItem(asn1Item["type"].toString() + typeLimit);
    itemMap["type"]->setData(QBrush(QColor("gray")), Qt::ForegroundRole);
    itemMap["type"]->setEditable(false);

    addPresentItem(asn1Item, itemMap);

    return itemMap;
}

void Asn1TreeView::addNumberItem(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    // set default value (min range):
    itemMap["value"] = new QStandardItem(asn1Item["min"].toString());
    itemMap["value"]->setData(asn1Item["type"], ASN1TYPE);
    itemMap["value"]->setData(asn1Item["min"],  MIN_RANGE);
    itemMap["value"]->setData(asn1Item["max"],  MAX_RANGE);
}

void Asn1TreeView::addBoolItem(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    itemMap["value"] = new QStandardItem(asn1Item["default"].toString());
    itemMap["value"]->setData(asn1Item["type"], ASN1TYPE);

    QVariantList choices;
    choices.append("true");
    choices.append("false");
    itemMap["value"]->setData(choices, CHOICE_LIST);
}

void Asn1TreeView::addSequenceItem(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    QStandardItem *parent = itemMap["item"];
    QVariantList children = asn1Item["children"].toList();

    for (int x = 0; x < children.size(); ++x) {
        ItemMap chilItem = createModelItems(children[x].toMap());

        parent->appendRow(chilItem["item"]);

        typeItems.append(chilItem["type"]);
        valueItems.append(chilItem["value"]);
        presentItems.append(chilItem["present"]);
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    itemMap["value"] = new QStandardItem();
}

void Asn1TreeView::addSequenceOfItem(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    QStandardItem *parent = itemMap["item"];

    for (int x = 0; x < asn1Item["max"].toInt(); ++x) {
        ItemMap chilItem = createModelItems(asn1Item["seqoftype"].toMap());

        chilItem["item"]->setText(QString(tr("elem%1")).arg(x + 1));
        parent->appendRow(chilItem["item"]);

        typeItems.append(chilItem["type"]);
        valueItems.append(chilItem["value"]);
        presentItems.append(chilItem["present"]);
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    itemMap["value"] = new QStandardItem(asn1Item["min"].toString());
    itemMap["value"]->setData(asn1Item["type"], ASN1TYPE);
    itemMap["value"]->setData(asn1Item["min"],  MIN_RANGE);
    itemMap["value"]->setData(asn1Item["max"],  MAX_RANGE);
}

void Asn1TreeView::addEnumeratedItem(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    itemMap["value"] = new QStandardItem(asn1Item["values"].toList()[0].toString());
    itemMap["value"]->setData(asn1Item["type"],      ASN1TYPE);
    itemMap["value"]->setData(asn1Item["values"],    CHOICE_LIST);
    itemMap["value"]->setData(asn1Item["valuesInt"], INTMAP);
}

void Asn1TreeView::addChoiceItem(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;
    QVariantList idList;

    QStandardItem *parent = itemMap["item"];

    QVariantList choices = asn1Item["choices"].toList();
    for (int x = 0; x < choices.size(); ++x) {
        ItemMap choiceItem = createModelItems(choices[x].toMap());

        parent->appendRow(choiceItem["item"]);

        typeItems.append(choiceItem["type"]);
        valueItems.append(choiceItem["value"]);
        presentItems.append(choiceItem["present"]);

        idList.append(choices[x].toMap()["id"]);
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    itemMap["value"] = new QStandardItem(idList[0].toString());
    itemMap["value"]->setData(asn1Item["type"],      ASN1TYPE);
    itemMap["value"]->setData(idList,                CHOICE_LIST);
    itemMap["value"]->setData(asn1Item["choiceIdx"], INTMAP);
}

void Asn1TreeView::addStringItem(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    itemMap["value"] = new QStandardItem();
    itemMap["value"]->setData(asn1Item["type"], ASN1TYPE);
    itemMap["value"]->setData(asn1Item["min"],  MIN_RANGE);
    itemMap["value"]->setData(asn1Item["max"],  MAX_RANGE);
}

void Asn1TreeView::addPresentItem(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    itemMap["present"] = new QStandardItem();

    if (asn1Item["isOptional"].toBool() == true &&
        asn1Item["alwaysPresent"].toBool() == false &&
        asn1Item["alwaysAbsent"].toBool() == false)
    {
        itemMap["present"]->setCheckState(Qt::Checked);
        itemMap["present"]->setCheckable(true);
    }

    itemMap["present"]->setData(asn1Item["isOptional"],    OPTIONAL);
    itemMap["present"]->setData(asn1Item["alwaysAbsent"],  ALWAYS_ABSENT);
    itemMap["present"]->setData(asn1Item["alwaysPresent"], ALWAYS_PRESENT);
}

void Asn1TreeView::hideExtraFields(const QStandardItem *item, bool hide, int row)
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(this->model());
    QString asnType = model->item(row, 1)->text();

    if (asnType == "choice" && hide) {
        QVariantList choices = model->item(row, 2)->data(CHOICE_LIST).toList();
        onChoiceFieldChanged(model->item(row)->index(),
                             choices.size(),
                             choices.indexOf(model->item(row, 2)->text()));
    }
    else if (asnType.indexOf("sequenceOf") >= 0 && hide) {
        onSequenceOfSizeChanged(model->item(row)->index(),
                                model->item(row, 2)->text(),
                                model->item(row, 2)->data(MAX_RANGE));
    }

    for (int x = 0; x < item->rowCount(); ++x) {
        hideExtraFields(item->child(x));

        asnType = item->child(x, 1)->text();
        if (asnType.indexOf("sequenceOf") >= 0)
            onSequenceOfSizeChanged(item->child(x)->index(),
                                    item->child(x, 2)->text(),
                                    item->child(x, 2)->data(MAX_RANGE));
        else if (asnType == "choice") {
            QVariantList choices = item->child(x, 2)->data(CHOICE_LIST).toList();
            onChoiceFieldChanged(item->child(x)->index(),
                                 choices.size(),
                                 choices.indexOf(item->child(x, 2)->text()));
        }
    }
}
