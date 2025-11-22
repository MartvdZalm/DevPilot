#include "SnippetsWindow.h"
#include "../styles/AppStyle.h"
#include "../styles/ButtonStyle.h"
#include "../styles/FontStyle.h"
#include "../styles/GroupBoxStyle.h"
#include "../styles/InputStyle.h"
#include "../styles/ListStyle.h"
#include "../styles/ThemeManager.h"
#include "../core/IconManager.h"
#include <QApplication>
#include <QClipboard>
#include <QFont>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>

SnippetsWindow::SnippetsWindow(RepositoryProvider& repositoryProvider, QWidget* parent)
    : BaseWindow(parent), snippetRepository(repositoryProvider.getSnippetRepository())
{
    setupUI();
    setupConnections();
    loadSnippets();
    applyTheme();
}

void SnippetsWindow::setupUI()
{
    setWindowTitle("Snippets");
    setMinimumSize(1000, 700);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    splitter = new QSplitter(Qt::Horizontal);
    splitter->setStretchFactor(0, 1); // Left panel: 1 part
    splitter->setStretchFactor(1, 2); // Right panel: 2 parts

    setupSidebar();
    setupSnippetForm();

    mainLayout->addWidget(splitter);

    rightPanel->setVisible(false); // Initially hide the right panel
}

void SnippetsWindow::setupConnections()
{
    connect(searchInput, &QLineEdit::textChanged, this, &SnippetsWindow::filterSnippets);
    connect(snippetList, &QListWidget::itemClicked, this, &SnippetsWindow::onSnippetSelected);

    connect(addSnippetButton, &QPushButton::clicked, this,
            [this]()
            {
                clearForm();
                rightPanel->setVisible(true);
                snippetList->clearSelection();
                titleInput->setEnabled(true);
                languageComboBox->setEnabled(true);
                codeEditor->setEnabled(true);
                descriptionInput->setEnabled(true);
                saveButton->setEnabled(true);
                deleteButton->setEnabled(false);
                copyButton->setEnabled(true);
                currentSnippetId = -1;
                titleInput->setFocus();
            });

    connect(saveButton, &QPushButton::clicked, this, &SnippetsWindow::saveSnippet);
    connect(deleteButton, &QPushButton::clicked, this, &SnippetsWindow::deleteSnippet);

    connect(copyButton, &QPushButton::clicked, this,
            [this]()
            {
                QApplication::clipboard()->setText(codeEditor->toPlainText());
                QMessageBox::information(this, "Copied", "Code copied to clipboard!");
            });

    connect(codeEditor, &QTextEdit::textChanged, this,
            [this]()
            {
                int chars = codeEditor->toPlainText().length();
                int lines = codeEditor->document()->lineCount();
                charCountLabel->setText("Characters: " + QString::number(chars));
                lineCountLabel->setText("Lines: " + QString::number(lines));
            });

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &SnippetsWindow::applyTheme);
}

void SnippetsWindow::setupSidebar()
{
    QWidget* leftPanel = new QWidget();
    leftPanel->setFixedWidth(250);

    QFrame* sidebarFrame = new QFrame();
    sidebarFrame->setProperty("role", "sidebar");
    auto* sidebarLayout = new QVBoxLayout(sidebarFrame);
    sidebarLayout->setContentsMargins(0, 0, 15, 0);

    QHBoxLayout* snippetsHeaderLayout = new QHBoxLayout();

    QLabel* snippetsLabel = new QLabel("Snippets");
    snippetsLabel->setProperty("role", "h2");
    snippetsLabel->setStyleSheet(FontStyle::h2());

    addSnippetButton = new QPushButton("", this);
    addSnippetButton->setIcon(IconManager::instance().add());
    addSnippetButton->setStyleSheet(ButtonStyle::icon());
    addSnippetButton->setIconSize(QSize(25, 25));

    snippetsHeaderLayout->addWidget(snippetsLabel);
    snippetsHeaderLayout->addStretch();
    snippetsHeaderLayout->addWidget(addSnippetButton);

    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Search snippets...");
    searchInput->setProperty("role", "input");
    searchInput->setStyleSheet(InputStyle::primary());

    snippetList = new QListWidget();
    snippetList->setProperty("role", "list");
    snippetList->setStyleSheet(ListStyle::primary());
    snippetList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    snippetList->setTextElideMode(Qt::ElideRight);

    sidebarLayout->addLayout(snippetsHeaderLayout);
    sidebarLayout->addWidget(searchInput);
    sidebarLayout->addWidget(snippetList);

    auto* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->addWidget(sidebarFrame);

    splitter->addWidget(leftPanel);
}

void SnippetsWindow::setupSnippetForm()
{
    rightPanel = new QWidget();
    auto* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    QGroupBox* editorGroup = new QGroupBox("Snippet Details");
    editorGroup->setProperty("role", "group");
    editorGroup->setStyleSheet(GroupBoxStyle::primary());
    auto* editorLayout = new QVBoxLayout(editorGroup);

    auto* titleLayout = new QHBoxLayout();

    QLabel* titleLabel = new QLabel("Title:");
    titleLabel->setProperty("role", "label");
    titleLabel->setStyleSheet(FontStyle::text() + " font-weight: bold; min-width: 80px;");

    titleInput = new QLineEdit();
    titleInput->setProperty("role", "input");
    titleInput->setPlaceholderText("Enter snippet title...");
    titleInput->setStyleSheet(InputStyle::primary());

    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(titleInput);

    auto* languageLayout = new QHBoxLayout();

    QLabel* languageLabel = new QLabel("Language:");
    languageLabel->setProperty("role", "label");
    languageLabel->setStyleSheet(FontStyle::text() + " font-weight: bold; min-width: 80px;");

    languageComboBox = new QComboBox();
    languageComboBox->setProperty("role", "input");
    languageComboBox->setStyleSheet(InputStyle::primary());
    languageComboBox->addItems({"C++",    "C",    "Python", "JavaScript", "TypeScript", "Java",
                                "C#",     "Go",   "Rust",   "PHP",        "Ruby",       "Swift",
                                "Kotlin", "HTML", "CSS",    "SQL",        "Bash",       "PowerShell",
                                "JSON",   "XML",  "YAML",   "Markdown",   "Plain Text", "Other"});

    languageLayout->addWidget(languageLabel);
    languageLayout->addWidget(languageComboBox);

    QLabel* codeLabel = new QLabel("Code:");
    codeLabel->setProperty("role", "label");
    codeLabel->setStyleSheet(FontStyle::text() + " font-weight: bold; margin-top: 10px;");

    codeEditor = new QTextEdit();
    codeEditor->setProperty("role", "code-editor");
    codeEditor->setLineWrapMode(QTextEdit::NoWrap);
    codeEditor->setFont(QFont("Consolas", 10));
    codeEditor->setPlaceholderText("Paste your code here...");
    codeEditor->setMinimumHeight(250);

    auto* statsLayout = new QHBoxLayout();

    charCountLabel = new QLabel("Characters: 0");
    charCountLabel->setProperty("role", "text-muted");
    charCountLabel->setStyleSheet(FontStyle::textMuted());

    lineCountLabel = new QLabel("Lines: 0");
    lineCountLabel->setProperty("role", "text-muted");
    lineCountLabel->setStyleSheet(FontStyle::textMuted());

    statsLayout->addWidget(charCountLabel);
    statsLayout->addWidget(lineCountLabel);
    statsLayout->addStretch();

    QLabel* descLabel = new QLabel("Description:");
    descLabel->setProperty("role", "label");
    descLabel->setStyleSheet(FontStyle::text() + " font-weight: bold; margin-top: 10px;");

    descriptionInput = new QTextEdit();
    descriptionInput->setProperty("role", "input");
    descriptionInput->setStyleSheet(InputStyle::primary());
    descriptionInput->setPlaceholderText("Optional description or notes...");
    descriptionInput->setMaximumHeight(80);

    auto* buttonLayout = new QHBoxLayout();

    saveButton = new QPushButton(QIcon(":/Images/Save"), "Save");
    saveButton->setProperty("role", "button-success");
    saveButton->setStyleSheet(ButtonStyle::success());

    deleteButton = new QPushButton(QIcon(":/Images/Trash"), "Delete");
    deleteButton->setProperty("role", "button-danger");
    deleteButton->setStyleSheet(ButtonStyle::danger());

    copyButton = new QPushButton(QIcon(":/Images/Copy"), "Copy Code");
    copyButton->setProperty("role", "button-primary");
    copyButton->setStyleSheet(ButtonStyle::primary());

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(copyButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();

    editorLayout->addLayout(titleLayout);
    editorLayout->addLayout(languageLayout);
    editorLayout->addWidget(codeLabel);
    editorLayout->addWidget(codeEditor);
    editorLayout->addLayout(statsLayout);
    editorLayout->addWidget(descLabel);
    editorLayout->addWidget(descriptionInput);
    editorLayout->addLayout(buttonLayout);

    rightLayout->addWidget(editorGroup);

    splitter->addWidget(rightPanel);
}

void SnippetsWindow::loadSnippets()
{
    snippetList->clear();
    allSnippets.clear();
    allSnippets = snippetRepository.findAll();

    for (const auto& snippet : allSnippets)
    {
        QListWidgetItem* item = new QListWidgetItem(QIcon(":/Images/Snippet"), snippet.getTitle());
        item->setData(Qt::UserRole, snippet.getId());
        snippetList->addItem(item);
    }
}

void SnippetsWindow::clearForm()
{
    titleInput->clear();
    languageComboBox->setCurrentIndex(0);
    codeEditor->clear();
    descriptionInput->clear();
    currentSnippetId = -1;
}

void SnippetsWindow::populateForm(int snippetId)
{
    auto snippetOpt = snippetRepository.findById(snippetId);

    if (!snippetOpt.has_value())
    {
        QMessageBox::warning(this, "Error", "Failed to load snippet.");
        return;
    }

    Snippet snippet = snippetOpt.value();

    titleInput->setText(snippet.getTitle());
    languageComboBox->setCurrentText(snippet.getLanguage());
    codeEditor->setPlainText(snippet.getCode());
    descriptionInput->setPlainText(snippet.getDescription());

    currentSnippetId = snippetId;

    rightPanel->setVisible(true);
    titleInput->setEnabled(true);
    languageComboBox->setEnabled(true);
    codeEditor->setEnabled(true);
    descriptionInput->setEnabled(true);
    saveButton->setEnabled(true);
    deleteButton->setEnabled(true);
    copyButton->setEnabled(true);
}

void SnippetsWindow::saveSnippet()
{
    QString title = titleInput->text().trimmed();
    QString language = languageComboBox->currentText();
    QString code = codeEditor->toPlainText();
    QString description = descriptionInput->toPlainText();

    if (title.isEmpty())
    {
        QMessageBox::warning(this, "Validation Error", "Please enter a title for the snippet.");
        titleInput->setFocus();
        return;
    }

    if (code.isEmpty())
    {
        QMessageBox::warning(this, "Validation Error", "Please enter some code for the snippet.");
        codeEditor->setFocus();
        return;
    }

    Snippet snippet;
    snippet.setTitle(title);
    snippet.setLanguage(language);
    snippet.setCode(code);
    snippet.setDescription(description);

    if (currentSnippetId == -1)
    {
        auto result = snippetRepository.save(snippet);

        if (result.has_value())
        {
            currentSnippetId = result.value().getId();
        }
        else
        {
            QMessageBox::critical(this, "Error", "Failed to create snippet.");
            return;
        }
    }
    else
    {
        snippet.setId(currentSnippetId);
        auto result = snippetRepository.save(snippet);

        if (!result.has_value())
        {
            QMessageBox::critical(this, "Error", "Failed to update snippet.");
            return;
        }
    }

    loadSnippets();
}

void SnippetsWindow::deleteSnippet()
{
    if (currentSnippetId == -1)
        return;

    int ret = QMessageBox::question(this, "Delete Snippet", "Are you sure you want to delete this snippet?",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {
        bool success = snippetRepository.deleteById(currentSnippetId);

        if (success)
        {
            clearForm();
            loadSnippets();

            titleInput->setEnabled(false);
            languageComboBox->setEnabled(false);
            codeEditor->setEnabled(false);
            descriptionInput->setEnabled(false);
            saveButton->setEnabled(false);
            deleteButton->setEnabled(false);
            copyButton->setEnabled(false);
        }
        else
        {
            QMessageBox::critical(this, "Error", "Failed to delete snippet.");
        }
    }
}

void SnippetsWindow::filterSnippets(const QString& text)
{
    if (text.isEmpty())
    {
        for (int i = 0; i < snippetList->count(); ++i)
        {
            snippetList->item(i)->setHidden(false);
        }
        return;
    }

    for (int i = 0; i < snippetList->count(); ++i)
    {
        QListWidgetItem* item = snippetList->item(i);
        int snippetId = item->data(Qt::UserRole).toInt();

        auto it = std::find_if(allSnippets.begin(), allSnippets.end(),
                               [snippetId](const Snippet& s) { return s.getId() == snippetId; });

        if (it != allSnippets.end())
        {
            const Snippet& snippet = *it;

            bool matches = snippet.getTitle().contains(text, Qt::CaseInsensitive) ||
                           snippet.getLanguage().contains(text, Qt::CaseInsensitive) ||
                           snippet.getCode().contains(text, Qt::CaseInsensitive) ||
                           snippet.getDescription().contains(text, Qt::CaseInsensitive);

            item->setHidden(!matches);
        }
    }
}

void SnippetsWindow::onSnippetSelected(QListWidgetItem* item)
{
    if (!item)
        return;

    int snippetId = item->data(Qt::UserRole).toInt();

    if (currentSnippetId == snippetId && rightPanel->isVisible())
    {
        rightPanel->setVisible(false);
        snippetList->clearSelection();
        currentSnippetId = -1;
        return;
    }

    populateForm(snippetId);
}

void SnippetsWindow::applyTheme()
{
    if (snippetList)
    {
        snippetList->setStyleSheet(ListStyle::primary());
    }

    if (searchInput)
    {
        searchInput->setStyleSheet(InputStyle::primary());
    }

    if (titleInput)
    {
        titleInput->setStyleSheet(InputStyle::primary());
    }

    if (languageComboBox)
    {
        languageComboBox->setStyleSheet(InputStyle::primary());
    }

    if (descriptionInput)
    {
        descriptionInput->setStyleSheet(InputStyle::primary());
    }

    if (saveButton)
    {
        saveButton->setStyleSheet(ButtonStyle::success());
    }

    if (deleteButton)
    {
        deleteButton->setStyleSheet(ButtonStyle::danger());
    }

    if (copyButton)
    {
        copyButton->setStyleSheet(ButtonStyle::primary());
    }

    if (addSnippetButton)
    {
        addSnippetButton->setStyleSheet(ButtonStyle::icon());
    }

    if (codeEditor)
    {
        Theme currentTheme = ThemeManager::instance().getCurrentTheme();
        if (currentTheme == Theme::Dark)
        {
            codeEditor->setStyleSheet(R"(
                QTextEdit {
                    background-color: #1e1e1e;
                    color: #d4d4d4;
                    border: 1px solid #3e3e3e;
                    border-radius: 4px;
                    padding: 8px;
                    font-family: 'Consolas', 'Courier New', monospace;
                    font-size: 10pt;
                    line-height: 1.5;
                }
            )");
        }
        else
        {
            codeEditor->setStyleSheet(R"(
                QTextEdit {
                    background-color: #ffffff;
                    color: #333333;
                    border: 1px solid #cccccc;
                    border-radius: 4px;
                    padding: 8px;
                    font-family: 'Consolas', 'Courier New', monospace;
                    font-size: 10pt;
                    line-height: 1.5;
                }
            )");
        }
    }

    const auto labels = findChildren<QLabel*>();
    for (QLabel* label : labels)
    {
        const QVariant roleVar = label->property("role");
        if (!roleVar.isValid())
        {
            continue;
        }

        const QString role = roleVar.toString();
        if (role == "h1")
        {
            label->setStyleSheet(FontStyle::h1());
        }
        else if (role == "h2")
        {
            label->setStyleSheet(FontStyle::h2());
        }
        else if (role == "h3")
        {
            label->setStyleSheet(FontStyle::h3());
        }
        else if (role == "text")
        {
            label->setStyleSheet(FontStyle::text());
        }
        else if (role == "text-muted")
        {
            label->setStyleSheet(FontStyle::textMuted());
        }
        else if (role == "label")
        {
            label->setStyleSheet(FontStyle::text());
        }
    }

    const auto groupBoxes = findChildren<QGroupBox*>();
    for (QGroupBox* groupBox : groupBoxes)
    {
        if (groupBox->property("role").toString() == "group")
        {
            groupBox->setStyleSheet(GroupBoxStyle::primary());
        }
    }

    const auto sidebars = findChildren<QFrame*>();
    for (QFrame* sidebar : sidebars)
    {
        if (sidebar->property("role").toString() == "sidebar")
        {
            Theme currentTheme = ThemeManager::instance().getCurrentTheme();
            if (currentTheme == Theme::Dark)
            {
                sidebar->setStyleSheet("QFrame { border-right: 1px solid #3a3f47; background-color: transparent; }");
            }
            else
            {
                sidebar->setStyleSheet("QFrame { border-right: 1px solid #cccccc; background-color: transparent; }");
            }
        }
    }

    addSnippetButton->setIcon(IconManager::instance().add());
}
