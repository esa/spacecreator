#pragma once

#include "xmlreader.h"

namespace shared {

class UIReader : public XmlReader
{
    Q_OBJECT

public:
    explicit UIReader(QObject *parent = nullptr);
    ~UIReader() override = default;

    EntityAttributes entityData(const Id &id) const;
    QHash<Id, EntityAttributes> data() const;

    QString rootElementName() const override;
    static QString filePathAttributeName();

private:
    QHash<Id, EntityAttributes> m_data;
    Id m_currentEntityID;
    EntityAttributes m_currentEntityAttributes;
    QStringList m_attrsPath;

protected:
    bool processTagOpen(QXmlStreamReader &xml) override;
    bool processTagClose(QXmlStreamReader &xml) override;
};

} // namespace shared
