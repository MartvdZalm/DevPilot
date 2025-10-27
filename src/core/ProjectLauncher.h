#ifndef PROJECTLAUNCHER_H
#define PROJECTLAUNCHER_H

#include "../models/Editor.h"
#include <QList>
#include <QString>

class ProjectLauncher
{
  public:
    // Opens the folder in the file explorer
    static void openFolder(const QString& path);

    // Opens the terminal in the specified directory
    static void openTerminal(const QString& path);

    // Opens the project in a specific editor
    static void openInEditor(const QString& path, const Editor& editor);
};

#endif // PROJECTLAUNCHER_H
