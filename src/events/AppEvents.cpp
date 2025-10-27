#include "AppEvents.h"

AppEvents& AppEvents::instance()
{
    static AppEvents instance;
    return instance;
}

void AppEvents::notifyNavigateToSettings()
{
    emit navigateToSettings();
}

void AppEvents::notifyNavigateToSnippets()
{
    emit navigateToSnippets();
}
