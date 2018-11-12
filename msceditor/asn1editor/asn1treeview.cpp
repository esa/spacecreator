#include <QStandardItem>
#include <QStandardItemModel>

#include "asn1itemdelegate.h"
#include "asn1treeview.h"

Asn1TreeView::Asn1TreeView(QWidget *parent) : QTreeView(parent)
{
    m_model = new QStandardItemModel(1, 5);
    QStringList headers = QStringList() << "Field"
                                        << "Type"
                                        << "Constraints"
                                        << "Value"
                                        << "Present";
    m_model->setHorizontalHeaderLabels(headers);
    setModel(m_model);

    m_itemDelegate = new Asn1ItemDelegate(this);
    setItemDelegate(m_itemDelegate);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors(true);
    setDragEnabled(false);

    connect(m_itemDelegate, &Asn1ItemDelegate::seqof, this, &Asn1TreeView::onSeqOf);
    connect(m_itemDelegate, &Asn1ItemDelegate::choice, this, &Asn1TreeView::onChoice);
}

void Asn1TreeView::setAsn1Model(QVariantMap asn1Item, int row)
{
    ItemMap itemMap = addItem(asn1Item);

    m_rootTreeItem = itemMap["item"];

    qobject_cast<QStandardItemModel *>(model())->setItem(row, 0, m_rootTreeItem);
    qobject_cast<QStandardItemModel *>(model())->setItem(row, 1, itemMap["type"]);
    qobject_cast<QStandardItemModel *>(model())->setItem(row, 2, itemMap["constraint"]);
    qobject_cast<QStandardItemModel *>(model())->setItem(row, 3, itemMap["value"]);

    hideExtraFields(m_rootTreeItem, true, row);
    hideExtraColumns(true);

    expandAll();
}

void Asn1TreeView::onSeqOf(const QModelIndex &index, QVariant value, QVariant maxRange)
{

}

void Asn1TreeView::onChoice(const QModelIndex &index, QVariant lenght, QVariant currentIndex)
{

}

void Asn1TreeView::hideExtraColumns(bool hide)
{
    setColumnHidden(1, hide);
    setColumnHidden(2, hide);
}

Asn1TreeView::ItemMap Asn1TreeView::addItem(QVariantMap asn1Item)
{
    ItemMap itemMap;

    itemMap["item"] = new QStandardItem(asn1Item["id"].toString());
    itemMap["item"]->setEditable(false);

    itemMap["type"] = new QStandardItem(asn1Item["type"].toString());
    itemMap["type"]->setData(QBrush(QColor("gray")), Qt::ForegroundRole);
    itemMap["type"]->setEditable(false);

    // get the Value and Contraints fields for the different types:
    if (asn1Item["type"] == "integer" || asn1Item["type"] == "double")
        addNumber(asn1Item, itemMap);
    else if (asn1Item["type"] == "bool")
        addBool(asn1Item, itemMap);
    else if (asn1Item["type"] == "sequence")
        addSequence(asn1Item, itemMap);
    else if (asn1Item["type"] == "seqof")
        addSeqOf(asn1Item, itemMap);
    else if (asn1Item["type"] == "enumerated")
        addEnum(asn1Item, itemMap);
    else if (asn1Item["type"] == "choice")
        addChoice(asn1Item, itemMap);
    else if (asn1Item["type"] == "string")
        addString(asn1Item, itemMap);

    return  itemMap;
}

void Asn1TreeView::addNumber(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    // set default value (min range):
    itemMap["value"] = new QStandardItem(asn1Item["min"].toString());
    itemMap["value"]->setData(asn1Item["type"], ASN1TYPE);
    itemMap["value"]->setData(asn1Item["minR"], MIN_RANGE);
    itemMap["value"]->setData(asn1Item["maxR"], MAX_RANGE);

    itemMap["constraint"] = new QStandardItem(QString("(%1..%2)").arg(asn1Item["min"].toString(),
                                                                      asn1Item["max"].toString()));
    itemMap["constraint"]->setData(QBrush(QColor("gray")), Qt::ForegroundRole);
    itemMap["constraint"]->setEditable(false);

    addPresentItem(asn1Item, itemMap);
}

void Asn1TreeView::addBool(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    itemMap["value"] = new QStandardItem(asn1Item["default"].toString());
    itemMap["value"]->setData(asn1Item["type"], ASN1TYPE);

    QVariantList choices;
    choices.append("true");
    choices.append("false");
    itemMap["value"]->setData(choices, CHOICE_LIST);

    itemMap["constraint"] = new QStandardItem();
    itemMap["constraint"]->setEditable(false);

    addPresentItem(asn1Item, itemMap);
}

void Asn1TreeView::addSequence(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> constraintItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    QStandardItem *parent = itemMap["item"];
    QVariantList children = asn1Item["children"].toList();

    for (int x = 0; x < children.size(); ++x) {
        ItemMap chilItem = addItem(children[x].toMap());

        parent->appendRow(chilItem["item"]);

        typeItems.append(chilItem["type"]);
        constraintItems.append(chilItem["value"]);
        valueItems.append(chilItem["constraint"]);
        presentItems.append(chilItem["present"]);
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(constraintItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    itemMap["value"] = new QStandardItem();
    itemMap["constraint"] = new QStandardItem();
    itemMap["constraint"]->setEditable(false);

    addPresentItem(asn1Item, itemMap);
}

void Asn1TreeView::addSeqOf(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> constraintItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    QStandardItem *parent = itemMap["item"];

    for (int x = 0; x < asn1Item["max"].toInt(); ++x) {
        asn1Item["seqoftype"].toMap()["id"] = QString("elem_%1").arg(x);

        ItemMap chilItem = addItem(asn1Item["seqoftype"].toMap());

        parent->appendRow(chilItem["item"]);

        typeItems.append(chilItem["type"]);
        constraintItems.append(chilItem["value"]);
        valueItems.append(chilItem["constraint"]);
        presentItems.append(chilItem["present"]);
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(constraintItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    QString constraintText;
    if (asn1Item["min"] == asn1Item["max"])
        constraintText = QString("Size(%1)").arg(asn1Item["min"].toInt());
    else
        constraintText = QString("Size(%1..%2)").arg(asn1Item["min"].toInt())
                                                .arg(asn1Item["max"].toInt());
    itemMap["constraint"] = new QStandardItem(constraintText);
    itemMap["constraint"]->setData(QBrush(QColor("gray")), Qt::ForegroundRole);
    itemMap["constraint"]->setEditable(false);

    itemMap["value"] = new QStandardItem(asn1Item["min"].toString());
    itemMap["value"]->setData(asn1Item["type"], ASN1TYPE);
    itemMap["value"]->setData(asn1Item["min"], MIN_RANGE);
    itemMap["value"]->setData(asn1Item["max"], MAX_RANGE);

    addPresentItem(asn1Item, itemMap);
}

void Asn1TreeView::addEnum(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    itemMap["value"] = new QStandardItem(asn1Item["values"].toList()[0].toString());
    itemMap["value"]->setData(asn1Item["type"], ASN1TYPE);
    itemMap["value"]->setData(asn1Item["values"], CHOICE_LIST);
    itemMap["value"]->setData(asn1Item["valuesInt"], INTMAP);

    itemMap["constraint"] = new QStandardItem();
    itemMap["constraint"]->setEditable(false);

    addPresentItem(asn1Item, itemMap);
}

void Asn1TreeView::addChoice(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> constraintItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;
    QVariantList idList;

    QStandardItem *parent = itemMap["item"];

    QVariantList choices = asn1Item["choices"].toList();
    for (int x = 0; x < choices.size(); ++x) {
        ItemMap choiceItem = addItem(choices[x].toMap());

        parent->appendRow(choiceItem["item"]);

        typeItems.append(choiceItem["type"]);
        constraintItems.append(choiceItem["value"]);
        valueItems.append(choiceItem["constraint"]);
        presentItems.append(choiceItem["present"]);

        idList.append(choices[x].toMap()["id"]);
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(constraintItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    itemMap["value"] = new QStandardItem(idList[0].toString());
    itemMap["value"]->setData(asn1Item["type"], ASN1TYPE);
    itemMap["value"]->setData(idList, CHOICE_LIST);
    itemMap["value"]->setData(asn1Item["choiceIdx"], INTMAP);

    itemMap["constraint"] = new QStandardItem();
    itemMap["constraint"]->setEditable(false);

    addPresentItem(asn1Item, itemMap);
}

void Asn1TreeView::addString(QVariantMap asn1Item, Asn1TreeView::ItemMap &itemMap)
{
    itemMap["value"] = new QStandardItem();
    itemMap["value"]->setData(asn1Item["type"], ASN1TYPE);
    itemMap["value"]->setData(asn1Item["min"], MIN_RANGE);
    itemMap["value"]->setData(asn1Item["max"], MAX_RANGE);

    QString constraintText;
    if (asn1Item["min"] == asn1Item["max"])
        constraintText = QString("Size(%1)").arg(asn1Item["min"].toInt());
    else
        constraintText = QString("Size(%1..%1)").arg(asn1Item["min"].toInt())
                                                .arg(asn1Item["max"].toInt());
    itemMap["constraint"] = new QStandardItem(constraintText);
    itemMap["constraint"]->setData(QBrush(QColor("gray")), Qt::ForegroundRole);
    itemMap["constraint"]->setEditable(false);

    addPresentItem(asn1Item, itemMap);
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
    // hide CHOICE unselected fields and SEQUENCE OF out-of-range fields
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(this->model());
    QString asnType = model->item(row, 1)->text();

    if (asnType == "choice" && hide) {
        QVariantList choices = model->item(row, 3)->data(CHOICE_LIST).toList();
        onChoice(model->item(row)->index(),
                 choices.size(),
                 choices.indexOf(model->item(row, 3)->text()));
    }
    else if (asnType == "seqof" && hide) {
        onSeqOf(model->item(row)->index(),
                model->item(row, 3)->text(),
                model->item(row, 3)->data(MAX_RANGE));
    }

    for (int x = 0; x < item->rowCount(); ++x) {
        hideExtraFields(item->child(x));

        asnType = item->child(x, 1)->text();
        if (asnType == "seqof")
            onSeqOf(item->child(x)->index(),
                    item->child(x, 3)->text(),
                    item->child(x, 3)->data(MAX_RANGE));
        else if (asnType == "choice") {
            QVariantList choices = item->child(x, 3)->data(CHOICE_LIST).toList();
            onChoice(item->child(x)->index(),
                     choices.size(),
                     choices.indexOf(m_rootTreeItem->child(x, 3)->text()));
        }
    }
}
