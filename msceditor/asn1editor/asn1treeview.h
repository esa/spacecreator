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
    void onSeqOf(const QModelIndex &index, QVariant value, QVariant maxRange);
    void onChoice(const QModelIndex &index, QVariant lenght, QVariant currentIndex);

private:
    using ItemMap = QMap<QString, QStandardItem*>;

    // hide/show the columns containing the type and constraint
    void hideExtraColumns(bool hide);

    ItemMap addItem(QVariantMap asn1Item);

    void addNumber(QVariantMap asn1Item, ItemMap &itemMap);
    void addBool(QVariantMap asn1Item, ItemMap &itemMap);
    void addSequence(QVariantMap asn1Item, ItemMap &itemMap);
    void addSeqOf(QVariantMap asn1Item, ItemMap &itemMap);
    void addEnum(QVariantMap asn1Item, ItemMap &itemMap);
    void addChoice(QVariantMap asn1Item, ItemMap &itemMap);
    void addString(QVariantMap asn1Item, ItemMap &itemMap);

    void addPresentItem(QVariantMap asn1Item, ItemMap &itemMap);

    void hideExtraFields(const QStandardItem *item, bool hide = false, int row = 0);

private:
    Asn1ItemDelegate *m_itemDelegate;
    QStandardItemModel *m_model;
    QStandardItem *m_rootTreeItem;
};

#endif // ASN1TREEVIEW_H
