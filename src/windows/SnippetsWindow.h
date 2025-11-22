#ifndef SNIPPETSWINDOW_H
#define SNIPPETSWINDOW_H

#include "../repositories/RepositoryProvider.h"
#include "../repositories/interfaces/ISnippetRepository.h"
#include "BaseWindow.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QSplitter>

class SnippetsWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit SnippetsWindow(RepositoryProvider& repositoryProvider, QWidget* parent = nullptr);

  private:
    void setupUI() override;
    void setupConnections() override;
    void applyTheme() override;
    void setupSidebar();
    void setupSnippetForm();

    void loadSnippets();
    void clearForm();
    void populateForm(int snippetId);
    void saveSnippet();
    void deleteSnippet();
    void filterSnippets(const QString& text);
    void onSnippetSelected(QListWidgetItem* item);

  private:
    ISnippetRepository& snippetRepository;

    QSplitter* splitter;
    QWidget* rightPanel;

    // Left panel - Snippet list
    QLineEdit* searchInput;
    QListWidget* snippetList;
    QPushButton* addSnippetButton;

    // Right panel - Snippet editor
    QLineEdit* titleInput;
    QComboBox* languageComboBox;
    QTextEdit* codeEditor;
    QTextEdit* descriptionInput;
    QPushButton* saveButton;
    QPushButton* deleteButton;
    QPushButton* copyButton;

    // Info labels
    QLabel* charCountLabel;
    QLabel* lineCountLabel;

    // Current state
    int currentSnippetId = -1;
    QList<Snippet> allSnippets;
};

#endif // SNIPPETSWINDOW_H
