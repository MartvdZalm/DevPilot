#include "ThemedIcon.h"
#include <QFile>
#include <QPainter>
#include <QSvgRenderer>

ThemedIcon::ThemedIcon(const QString& svgPath)
    : svgPath(svgPath)
{
}

QIcon ThemedIcon::icon() const
{
    return icon(ThemeManager::instance().getCurrentTheme());
}

QIcon ThemedIcon::icon(Theme theme) const
{
    if (theme == Theme::Dark) {
        if (darkIcon.isNull()) {
            darkIcon = createIcon(Theme::Dark);
        }
        return darkIcon;
    } else {
        if (lightIcon.isNull()) {
            lightIcon = createIcon(Theme::Light);
        }
        return lightIcon;
    }
}

void ThemedIcon::updateTheme()
{
    // Clear cached icons to force regeneration
    darkIcon = QIcon();
    lightIcon = QIcon();
}

QIcon ThemedIcon::createIcon(Theme theme) const
{
    QSvgRenderer renderer(svgPath);
    if (!renderer.isValid())
        return QIcon();

           // Choose color
    QColor color = (theme == Theme::Dark) ? QColor("#ffffff")
                                          : QColor("#000000");

           // Determine default size of the SVG
    QSize size = renderer.defaultSize();
    if (!size.isValid())
        size = QSize(32, 32); // fallback

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

           // Render original SVG
    renderer.render(&painter);

           // Apply color tint
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), color);

    painter.end();

    return QIcon(pixmap);
}

QString ThemedIcon::loadAndColorSvg(Theme theme) const
{
    QFile file(svgPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }

    QString svgContent = file.readAll();
    file.close();

    // Define colors for themes
    QString iconColor = (theme == Theme::Dark) ? "#ffffff" : "#000000";
    QString backgroundColor = (theme == Theme::Dark) ? "transparent" : "transparent";

    // Replace colors in SVG
    // This assumes your SVGs use currentColor for the icon color
    svgContent.replace("currentColor", iconColor);
    svgContent.replace("fill=\"#000000\"", QString("fill=\"%1\"").arg(iconColor));
    svgContent.replace("stroke=\"#000000\"", QString("stroke=\"%1\"").arg(iconColor));
    svgContent.replace("fill=\"#ffffff\"", QString("fill=\"%1\"").arg(iconColor));
    svgContent.replace("stroke=\"#ffffff\"", QString("stroke=\"%1\"").arg(iconColor));

    return svgContent;
}
