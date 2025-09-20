#include "SettingsWindow.h"

#include "../styles/ButtonStyle.h"
#include "../styles/ListStyle.h"
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QFormLayout>
#include <QCheckBox>

SettingsWindow::SettingsWindow(QWidget* parent) : BaseWindow(parent)
{
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
    sidebar->addItem("Audio");
    sidebar->addItem("Library");
    sidebar->addItem("Playback");
    sidebar->addItem("Interface");
    sidebar->addItem("Hotkeys");
    sidebar->addItem("Advanced");
    sidebar->addItem("About");

    sidebarLayout->addWidget(sidebar);

    connect(sidebar, &QListWidget::currentRowChanged, this, &SettingsWindow::onSidebarItemChanged);
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
    audioPage = createAudioPage();
    libraryPage = createLibraryPage();
    playbackPage = createPlaybackPage();
    interfacePage = createInterfacePage();
    hotkeysPage = createHotkeysPage();
    advancedPage = createAdvancedPage();
    aboutPage = createAboutPage();

    contentStack->addWidget(generalPage);
    contentStack->addWidget(audioPage);
    contentStack->addWidget(libraryPage);
    contentStack->addWidget(playbackPage);
    contentStack->addWidget(interfacePage);
    contentStack->addWidget(hotkeysPage);
    contentStack->addWidget(advancedPage);
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

    connect(cancelButton, &QPushButton::clicked, this, &SettingsWindow::onCancelClicked);
    connect(applyButton, &QPushButton::clicked, this, &SettingsWindow::onApplyClicked);
}

QWidget* SettingsWindow::createGeneralPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QGroupBox* languageGroup = new QGroupBox("Language");
    QFormLayout* languageLayout = new QFormLayout(languageGroup);

    QComboBox* languageCombo = new QComboBox;
    //languageCombo->addItems(LanguageManager::instance().getAvailableLanguages());
    languageLayout->addRow("Language:", languageCombo);

    layout->addWidget(languageGroup);
    layout->addStretch();

    return page;
}

QWidget* SettingsWindow::createAudioPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QGroupBox* outputGroup = new QGroupBox("Audio Output");
    QFormLayout* outputLayout = new QFormLayout(outputGroup);

    QComboBox* deviceCombo = new QComboBox;
    deviceCombo->addItems({"Default", "Speakers", "Headphones"});
    outputLayout->addRow("Output Device:", deviceCombo);

    QComboBox* qualityCombo = new QComboBox;
    qualityCombo->addItems({"16-bit 44.1kHz", "24-bit 48kHz", "24-bit 96kHz"});
    outputLayout->addRow("Audio Quality:", qualityCombo);

    QGroupBox* processingGroup = new QGroupBox("Audio Processing");
    QVBoxLayout* processingLayout = new QVBoxLayout(processingGroup);

    QCheckBox* normalization = new QCheckBox("Volume normalization");
    QCheckBox* gapless = new QCheckBox("Gapless playback");

    processingLayout->addWidget(normalization);
    processingLayout->addWidget(gapless);

    QHBoxLayout* crossfadeLayout = new QHBoxLayout;
    crossfadeLayout->addWidget(new QLabel("Crossfade:"));
    QSlider* crossfadeSlider = new QSlider(Qt::Horizontal);
    crossfadeSlider->setRange(0, 10);
    crossfadeSlider->setValue(2);
    QLabel* crossfadeValue = new QLabel("2s");
    crossfadeLayout->addWidget(crossfadeSlider);
    crossfadeLayout->addWidget(crossfadeValue);
    processingLayout->addLayout(crossfadeLayout);

    layout->addWidget(outputGroup);
    layout->addWidget(processingGroup);
    layout->addStretch();

    return page;
}

QWidget* SettingsWindow::createLibraryPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* placeholder = new QLabel("Library settings will go here...");
    layout->addWidget(placeholder);
    layout->addStretch();

    return page;
}

QWidget* SettingsWindow::createPlaybackPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* placeholder = new QLabel("Playback settings will go here...");
    layout->addWidget(placeholder);
    layout->addStretch();

    return page;
}

QWidget* SettingsWindow::createInterfacePage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* placeholder = new QLabel("Interface settings will go here...");
    layout->addWidget(placeholder);
    layout->addStretch();

    return page;
}

QWidget* SettingsWindow::createHotkeysPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* placeholder = new QLabel("Hotkey settings will go here...");
    layout->addWidget(placeholder);
    layout->addStretch();

    return page;
}

QWidget* SettingsWindow::createAdvancedPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* placeholder = new QLabel("Advanced settings will go here...");
    layout->addWidget(placeholder);
    layout->addStretch();

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
    //AppEvents::instance().notifyNavigateToHome();
}

void SettingsWindow::onApplyClicked()
{
    saveSettings();
}

void SettingsWindow::loadSettings()
{
    //std::optional<Setting> settingModel = Container::instance().getSettingRepository()->findByKey("language");

    //if (settingModel)
    //{
    //    int index = languageCombo->findText(settingModel->getValue());
    //    if (index >= 0)
    //    {
    //        languageCombo->setCurrentIndex(index);
    //    }
    //}
    //else
    //{
    //    QString currentLang = LanguageManager::instance().getCurrentLanguage();
    //    int index = languageCombo->findText(currentLang);
    //    if (index >= 0)
    //    {
    //        languageCombo->setCurrentIndex(index);
    //    }
    //}
}

void SettingsWindow::saveSettings()
{

}
