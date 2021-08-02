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

        bool operator==(const Node &n)
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
    };

    explicit XmlData(const QByteArray &data)
    {
        QXmlStreamReader xml(data);
        processXml(xml);
    }
    explicit XmlData(const QString &path)
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            qFatal("%s", qUtf8Printable(file.errorString()));
            return;
        }

        QXmlStreamReader xml(&file);
        processXml(xml);
    }

    bool operator==(const XmlData &other) { return other.nodes == nodes; }

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

private:
    QList<Node> nodes;
};
