#ifndef SETTINGSTABLE_H
#define SETTINGSTABLE_H

#include "abstracttable.h"

#include "Settings/settingsmodel.h"

class SettingsTable : public AbstractTable
{
public:
    bool create() override;
    bool removeByExpression(const QString&) override;
    bool insert(const SettingsModel&);
    bool update(const SettingsModel&);
};

#endif // SETTINGSTABLE_H
