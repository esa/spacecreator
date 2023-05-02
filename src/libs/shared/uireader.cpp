#include "uireader.h"

#include "common.h"

namespace shared {

static const QString kEntityTagName { "Entity" };

UIReader::UIReader(QObject *parent)
    : XmlReader { parent }
{
}

EntityAttributes UIReader::entityData(const Id &id) const
{
    return m_data.value(id);
}

QHash<shared::Id, EntityAttributes> UIReader::data() const
{
    return m_data;
}

bool UIReader::processTagOpen(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    EntityAttributes attrs = attributes(xml.attributes());
    if (tagName == kEntityTagName) {
        m_currentEntityID = attrs.take(QLatin1String("id")).value<QUuid>();
        m_currentEntityAttributes = attrs;
        return true;
    }

    m_attrsPath << tagName;
    const QString key = m_attrsPath.join(shared::kStringDelemiter);
    for (EntityAttribute entityAttr : qAsConst(attrs)) {
        entityAttr.setName(key + shared::kStringDelemiter + entityAttr.name());
        entityAttr.setType(EntityAttribute::Type::Property);
        m_currentEntityAttributes.insert(entityAttr.name(), entityAttr);
    }
    return true;
}

bool UIReader::processTagClose(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    if (tagName == kEntityTagName) {
        m_data.insert(m_currentEntityID, m_currentEntityAttributes);
        m_currentEntityID = shared::InvalidId;
        m_currentEntityAttributes.clear();
        m_attrsPath.clear();
    } else if (!m_attrsPath.isEmpty() && m_attrsPath.last() == tagName) {
        m_attrsPath.removeLast();
    } else if (tagName != rootElementName()) {
        Q_ASSERT(m_attrsPath.isEmpty());
        return false;
    }
    return true;
}

QString UIReader::rootElementName() const
{
    return QLatin1String("UI");
}

QString UIReader::filePathAttributeName()
{
    return QLatin1String("UiFile");
}

} // namespace shared
