#ifndef ASN1ITEMDELEGATE_H
#define ASN1ITEMDELEGATE_H

#include <QStyledItemDelegate>

const int ASN1TYPE       = Qt::UserRole;
const int MIN_RANGE      = Qt::UserRole + 1;
const int MAX_RANGE      = Qt::UserRole + 2;
const int CHOICE_LIST    = Qt::UserRole + 3;
const int INTMAP         = Qt::UserRole + 4;
const int PLOTTERS       = Qt::UserRole + 5;
const int OPTIONAL       = Qt::UserRole + 6;
const int ALWAYS_PRESENT = Qt::UserRole + 7;
const int ALWAYS_ABSENT  = Qt::UserRole + 8;

class Asn1ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    Asn1ItemDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index);
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index);

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index);
 Q_SIGNALS:
    void seqof(const QModelIndex &index, QVariant value, QVariant maxRange);
    void choice(const QModelIndex &index, QVariant lenght, QVariant currentIndex);

};

#endif // ASN1ITEMDELEGATE_H
