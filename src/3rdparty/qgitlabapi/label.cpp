#include "label.h"

Label::Label()
{
}

Label::Label(const QJsonObject &label)
{
    mLabelId = label["id"].toInt();
    mName = label["name"].toString();
    mDescription = label["description"].toString();
    mColor = QColor::fromString(label["color"].toString());
}
