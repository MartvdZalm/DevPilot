#ifndef PROJECTLAUNCHER_H
#define PROJECTLAUNCHER_H

#include <QString>
#include <QList>
#include "../models/Editor.h"

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
