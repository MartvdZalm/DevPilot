#include "SettingsWindow.h"

#include "../styles/ButtonStyle.h"
#include "../styles/ListStyle.h"
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QFormLayout>
#include <QCheckBox>

SettingsWindow::SettingsWindow(std::shared_ptr<RepositoryProvider> repoProvider, QWidget* parent)
    : repositoryRepository(std::move(repoProvider)), BaseWindow(parent)
{
    setFixedSize(QSize(1000, 600));
    setWindowTitle("Settings");
    setupUI();
    setupConnections();
}

void SettingsWindow::setupUI()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setupSidebar();
    setupContentArea();
    setupButtonArea();

    QWidget* rightContainer = new QWidget;
    rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    rightLayout->addWidget(contentFrame, 1);
    rightLayout->addWidget(buttonFrame, 0);

    mainLayout->addWidget(sidebarFrame, 0);
    mainLayout->addWidget(rightContainer, 1);

    sidebar->setCurrentRow(0);
    onSidebarItemChanged(0);
}

void SettingsWindow::setupConnections()
{
    connect(sidebar, &QListWidget::currentRowChanged, this, &SettingsWindow::onSidebarItemChanged);
    connect(cancelButton, &QPushButton::clicked, this, &SettingsWindow::onCancelClicked);
    connect(applyButton, &QPushButton::clicked, this, &SettingsWindow::onApplyClicked);
}

void SettingsWindow::setupSidebar()
{
    sidebarFrame = new QFrame;
    sidebarFrame->setFixedWidth(200);
    sidebarFrame->setFrameStyle(QFrame::StyledPanel);

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebarFrame);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);

    sidebar = new QListWidget;
    sidebar->setFrameStyle(QFrame::NoFrame);
    sidebar->setStyleSheet(ListStyle::primary());

    sidebar->addItem("General");
    sidebar->addItem("About");

    sidebarLayout->addWidget(sidebar);
}

void SettingsWindow::setupContentArea()
{
    contentFrame = new QFrame;
    contentFrame->setFrameStyle(QFrame::StyledPanel);

    QVBoxLayout* contentLayout = new QVBoxLayout(contentFrame);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    contentStack = new QStackedWidget;

    generalPage = createGeneralPage();
    aboutPage = createAboutPage();

    contentStack->addWidget(generalPage);
    contentStack->addWidget(aboutPage);

    scrollArea->setWidget(contentStack);
    contentLayout->addWidget(scrollArea);
}

void SettingsWindow::setupButtonArea()
{
    buttonFrame = new QFrame;
    buttonFrame->setFixedHeight(60);
    buttonFrame->setFrameStyle(QFrame::StyledPanel);

    buttonLayout = new QHBoxLayout(buttonFrame);
    buttonLayout->setContentsMargins(15, 10, 15, 10);
    buttonLayout->addStretch();

    cancelButton = new QPushButton("Cancel");
    cancelButton->setStyleSheet(ButtonStyle::primary());

    applyButton = new QPushButton("Apply");
    applyButton->setStyleSheet(ButtonStyle::primary());

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(applyButton);
}

QWidget* SettingsWindow::createGeneralPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    return page;
}

QWidget* SettingsWindow::createAboutPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* placeholder = new QLabel("About..");
    layout->addWidget(placeholder);
    layout->addStretch();

    return page;
}

void SettingsWindow::onSidebarItemChanged(int index)
{
    contentStack->setCurrentIndex(index);
}

void SettingsWindow::onCancelClicked()
{
    loadSettings();
}

void SettingsWindow::onApplyClicked()
{
    saveSettings();
}

void SettingsWindow::loadSettings()
{

}

void SettingsWindow::saveSettings()
{

}
