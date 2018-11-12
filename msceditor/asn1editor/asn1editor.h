#ifndef ASN1EDITWIDGET_H
#define ASN1EDITWIDGET_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
    class Asn1Editor;
}

class Asn1TreeView;

class Asn1Editor : public QDialog
{
    Q_OBJECT

public:
    Asn1Editor(QWidget *parent = nullptr);
    ~Asn1Editor();

private Q_SLOTS:
    void openFile();
    void showParseError(const QString &error);
    void showAsn1Type(const QString &text);

private:
    void loadFile(const QString &file);

private:
    Ui::Asn1Editor *ui;
    Asn1TreeView *m_ans1TreeView;
    QVariantMap m_asn1Types;
};

#endif // ASN1EDITWIDGET_H
