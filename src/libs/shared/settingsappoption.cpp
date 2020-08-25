#include "settingsappoption.h"

#include "settingsmanager.h"

namespace shared {

SettingsAppOption::SettingsAppOption(const QString &name, const QVariant &defaultValue)
    : Name(name)
    , DefaultValue(defaultValue)
{
}

QVariant SettingsAppOption::read() const
{
    return shared::SettingsManager::instance()->storage()->value(Name, DefaultValue);
}

void SettingsAppOption::write(const QVariant &val)
{
    shared::SettingsManager::instance()->storage()->setValue(Name, val);
    shared::SettingsManager::instance()->storage()->sync();
}

}
