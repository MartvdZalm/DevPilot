#include "ChooseEditorDialog.h"
#include "../../styles/ButtonStyle.h"
#include "../../styles/ListStyle.h"
#include <QListWidget>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>

ChooseEditorDialog::ChooseEditorDialog(const QList<Editor>& editors, QWidget* parent) : QDialog(parent),
    editors(editors)
{
    setupUI();
}

void ChooseEditorDialog::setupUI()
{
    setWindowTitle("Open Project in Editor");
    setFixedSize(400, 300);
    setStyleSheet("QDialog { background-color: #2b2b2b; }");

    QVBoxLayout* layout = new QVBoxLayout(this);
    editorList = new QListWidget();
    editorList->setStyleSheet(ListStyle::primary());
    editorList->setAlternatingRowColors(false);

    for (const Editor& editor : editors)
    {
        QListWidgetItem* item = new QListWidgetItem(editor.getName());
        item->setData(Qt::UserRole, QVariant::fromValue(editor));
        editorList->addItem(item);
    }

    if (editorList->count() > 0)
    {
        editorList->setCurrentRow(0);
    }

    layout->addWidget(editorList);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Open | QDialogButtonBox::Cancel);
    QPushButton* openButton = buttonBox->button(QDialogButtonBox::Open);
    QPushButton* cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    openButton->setStyleSheet(ButtonStyle::primary());
    cancelButton->setStyleSheet(ButtonStyle::primary());

    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

Editor ChooseEditorDialog::getSelectedEditor()
{
    return editorList->currentItem()->data(Qt::UserRole).value<Editor>();
}
