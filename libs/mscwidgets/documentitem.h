#ifndef DOCUMENTITEM_H
#define DOCUMENTITEM_H

#include <QGraphicsObject>

#include <memory>

namespace msc {

class MscDocument;

class DocumentItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(QSizeF boxSize READ boxSize WRITE setBoxSize NOTIFY boxSizeChanged)

public:
    explicit DocumentItem(MscDocument* document, QGraphicsItem *parent = nullptr);
    ~DocumentItem() override;

    QRectF boundingRect() const override;

    void setBoundingRect(const QRectF& rect);

    // Get the size of the written text with this font
    QSizeF textSize(const QFont& font) const;

    QString documentName() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QSizeF boxSize() const;

    void addChildDocument(DocumentItem* item);
    const QVector<DocumentItem *> &childDocuments() const;

public Q_SLOTS:
    void setBoxSize(const QSizeF& size);

Q_SIGNALS:
    void boxSizeChanged(QSizeF size);

private:
    struct DocumentItemPrivate;
    const std::unique_ptr<DocumentItemPrivate> d;
};

}

#endif // DOCUMENTITEM_H
