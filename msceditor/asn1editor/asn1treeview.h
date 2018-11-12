#ifndef ASN1TREEVIEW_H
#define ASN1TREEVIEW_H

#include <QMap>
#include <QTreeView>

class Asn1ItemDelegate;
class QStandardItemModel;
class QStandardItem;

class Asn1TreeView : public QTreeView
{
    Q_OBJECT

public:
    Asn1TreeView(QWidget *parent = nullptr);

    void setAsn1Model(QVariantMap asn1Item, int row = 0);

private Q_SLOTS:
    void onSequenceOfSizeChanged(const QModelIndex &index, QVariant value, QVariant maxRange);
    void onChoiceFieldChanged(const QModelIndex &index, QVariant length, QVariant currentIndex);

private:
    using ItemMap = QMap<QString, QStandardItem*>;

    ItemMap createModelItems(QVariantMap asn1Item);

    void addNumberItem(QVariantMap asn1Item, ItemMap &itemMap);
    void addBoolItem(QVariantMap asn1Item, ItemMap &itemMap);
    void addSequenceItem(QVariantMap asn1Item, ItemMap &itemMap);
    void addSequenceOfItem(QVariantMap asn1Item, ItemMap &itemMap);
    void addEnumeratedItem(QVariantMap asn1Item, ItemMap &itemMap);
    void addChoiceItem(QVariantMap asn1Item, ItemMap &itemMap);
    void addStringItem(QVariantMap asn1Item, ItemMap &itemMap);

    void addPresentItem(QVariantMap asn1Item, ItemMap &itemMap);

    void hideExtraFields(const QStandardItem *item, bool hide = false, int row = 0);

private:
    Asn1ItemDelegate *m_itemDelegate;
    QStandardItemModel *m_model;
    QStandardItem *m_rootTreeItem;
};

#endif // ASN1TREEVIEW_H
