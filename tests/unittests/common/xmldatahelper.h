#include <QDebug>
#include <QFile>
#include <QVariant>
#include <QXmlStreamReader>

class XmlData
{
public:
    struct Node {
        QString type;
        QVariant data;
        QVariantHash attrs;
        QList<Node> children;

        bool operator==(const Node &n) const
        {
            const bool ok = n.type == type && n.data == data && n.attrs == attrs && n.children == children;
            if (!ok) {
                if (n.type != type)
                    qDebug() << n.type << "!=" << type;
                if (n.data != data)
                    qDebug() << n.data << "!=" << data;
                if (n.attrs != attrs)
                    qDebug() << n.attrs << "!=" << attrs;
                if (n.children != children)
                    qDebug() << "children of type '" << type << "' differ";
            }
            return ok;
        }

        bool compare(const Node &n, bool strict, const QStringList &excludedAttrs, const QStringList &excludedProperties) const
        {
            if (strict)
                return *this == n;

            if (type != n.type)
                return false;

            if (data != n.data)
                return false;

            for (auto it = attrs.cbegin(); it != attrs.cend(); ++it) {
                if (excludedAttrs.contains(it.key()))
                    continue;

                auto nIt = n.attrs.constFind(it.key());
                if (nIt == n.attrs.cend())
                    return false;

                if (nIt.value() != it.value())
                    return false;
            }

            for (auto it = children.cbegin(); it != children.cend(); ++it) {
                if (excludedProperties.contains(it->type))
                    continue;

                auto nIt = std::find_if(n.children.cbegin(), n.children.cend(), [&](const Node& node){
                    return it->compare(node, strict, excludedAttrs, excludedProperties);
                });

                if (nIt == n.children.cend())
                    return false;
            }

            return true;
        }
    };

    explicit XmlData(const QByteArray &data, const QStringList &excludedAttrs = {}, const QStringList &excludedProperties = {})
//        : excludedAttrs(excludedAttrs), excludedProperties(excludedProperties)
    {
        QXmlStreamReader xml(data);
        processXml(xml);
    }
    explicit XmlData(const QString &path, const QStringList &excludedAttrs = {}, const QStringList &excludedProperties = {})
//        : excludedAttrs(excludedAttrs), excludedProperties(excludedProperties)
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            qFatal("%s", qUtf8Printable(file.errorString()));
            return;
        }

        QXmlStreamReader xml(&file);
        processXml(xml);
    }

    bool isEqual(const XmlData &xmlData, bool strictComparision, const QStringList &excludedAttrs, const QStringList &excludedProperties) const
    {
        for (const Node &n: qAsConst(nodes)) {
            if (excludedProperties.contains(n.type))
                continue;

            auto it = std::find_if(xmlData.nodes.cbegin(), xmlData.nodes.cend(), [&](const Node& node){
                return n.compare(node, strictComparision, excludedAttrs, excludedProperties);
            });

            if (it == xmlData.nodes.cend()) {
                qDebug() << "Matching node '" << n.type << "' isn't found";
                return false;
            }
        }

        return true;
    }

private:
    Node parseNode(QXmlStreamReader &xml)
    {
        Node node;
        node.type = xml.name().toString();
        node.data = xml.text().toString();
        for (const QXmlStreamAttribute &attr : xml.attributes()) {
            node.attrs.insert(attr.name().toString(), attr.value().toString());
        }
        while (xml.readNextStartElement()) {
            node.children.append(parseNode(xml));
        }
        return node;
    }

    void processXml(QXmlStreamReader &xml)
    {
        while (!xml.atEnd() && !xml.hasError()) {
            const QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartDocument) {
                continue;
            }
            while (xml.readNextStartElement()) {
                nodes.append(parseNode(xml));
            }
        }
    }

    friend bool operator==(const XmlData &lhs, const XmlData &rhs);

private:
    QList<Node> nodes;
//    const QStringList excludedAttrs;
//    const QStringList excludedProperties;
};

bool operator==(const XmlData &lhs, const XmlData &rhs)
{
    return lhs.nodes == rhs.nodes;
}
