#include "Label.h"

Label::Label()
{

}
Label::Label(const QJsonObject &label)
{
    mName = label["name"].toString();
    mDescription = label["description"].toString();
}
