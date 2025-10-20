#include "ProjectLauncher.h"
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QDir>
#include <QMessageBox>

void ProjectLauncher::openFolder(const QString& path)
{
    if (path.isEmpty() || !QDir(path).exists())
        return;

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void ProjectLauncher::openTerminal(const QString& path)
{
    if (path.isEmpty() || !QDir(path).exists())
        return;

#if defined(Q_OS_WIN)
    if (path.isEmpty())
    {
        return;
    }

    // Handle WSL paths differently from regular Windows paths
    if (path.contains("wsl", Qt::CaseInsensitive))
    {
        QRegularExpression regex(R"(\\\\wsl\.localhost\\[^\\]+\\home\\[^\\]+)");
        QString dirPath = path;
        QString cleanedPath = dirPath.replace(regex, "~");
        cleanedPath.replace("\\", "/");

        QStringList arguments;
        arguments << "wsl"
                  << "--cd" << cleanedPath;

        QProcess::startDetached("wt.exe", arguments);
    }
    else
    {
        QString windowsPath = QDir::toNativeSeparators(path).trimmed();
        if (windowsPath.endsWith('\\'))
        {
            windowsPath.chop(1);
        }

        QString command = QString("Start-Process cmd -ArgumentList '/K cd /d \"\"%1\"\"'").arg(windowsPath);
        QProcess::startDetached("powershell.exe", QStringList() << "-Command" << command);
    }
#endif
}

void ProjectLauncher::openInEditor(const QString& path, const Editor& editor)
{
    QString command = editor.getPath();
    QString arguments = editor.getArguments();

    arguments.replace("%PATH%", path);

    QProcess::startDetached(command, QStringList() << arguments);
}
