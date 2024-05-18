#ifndef SETTINGSTABLE_H
#define SETTINGSTABLE_H

#include "abstracttable.h"

#include "Settings/settingsmodel.h"

class SettingsTable : public AbstractTable
{
public:
    bool create() override;
    bool removeByExpression(const QString&) override;
    SettingsModel select();
    bool insert(const SettingsModel&);
    bool update(const SettingsModel&);
    bool insertOrUpdate(const SettingsModel&);
    bool exist();
};

#endif // SETTINGSTABLE_H
