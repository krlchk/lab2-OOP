#include "createeditewindow.h"
#include "ui_creationwindow.h"

#include <QMessageBox>

#include "notewindow.h"

CreateEditeWindow::CreateEditeWindow(QWidget *parent, CreateEditeMode mode, Note* current) :
    QDialog(parent),
    ui(new Ui::CreateEditeWindow)
{
    ui->setupUi(this);

    this->mode = mode;
    this->current = current;

    if  (this->mode == CreateEditeMode::EDITE) {
        ui->typeComboBox->setCurrentText((current->containCategory("Нотатки")) ? "Нотатка": "Завдання");
        ui->typeComboBox->setEnabled(false);
        ui->titleLineEdit->setText(current->title.c_str());
        ui->contentTextEdit->setText(current->text.c_str());
        selectedCategories = current->categories;
    }

    configureListWidget();
}

CreateEditeWindow::~CreateEditeWindow()
{
    delete ui;
}

void CreateEditeWindow::configureListWidget(){
    QStringList strList;
    std::vector<std::string> categories = NoteWindow::loadCategories();
    for (auto& item : categories)
        if (item != "Нотатки" && item != "Завдання")
            strList << item.c_str();

    ui->categoriesListWidget->addItems(strList);

    QListWidgetItem* item = 0;
    for(int i = 0; i < ui->categoriesListWidget->count(); ++i){
        item = ui->categoriesListWidget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        if (current && current->containCategory(item->text().toStdString()))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
    }
    QObject::connect(ui->categoriesListWidget, SIGNAL(itemChanged(QListWidgetItem*)),
                     this, SLOT(switchCategory(QListWidgetItem*)));
}

void CreateEditeWindow::switchCategory(QListWidgetItem *item){
    if(item->checkState() == Qt::Checked) {
//        item->setBackground(QColor("#ffffb2"));
        if (std::find(selectedCategories.begin(), selectedCategories.end(), item->text().toStdString()) == selectedCategories.end())
            selectedCategories.push_back(item->text().toStdString());
    } else {
//        item->setBackground(QColor("#ffffff"));
        selectedCategories.erase(
                    std::remove(selectedCategories.begin(),
                                selectedCategories.end(),
                                item->text().toStdString()),
                    selectedCategories.end());
    }
}

void CreateEditeWindow::saveNote() {
    if (mode == CreateEditeMode::CREATE) {
        if (ui->typeComboBox->currentText() == "Нотатка") {
            selectedCategories.push_back("Нотатки");
            Note note(ui->titleLineEdit->text().toStdString(), ui->contentTextEdit->toPlainText().toStdString(), selectedCategories);
            note.writeToFile("..\\lab2-OOP\\notes.bin");
            NoteWindow::notes.push_back(new Note(note));
        } else {
            selectedCategories.push_back("Завдання");
            Task task(ui->titleLineEdit->text().toStdString(), ui->contentTextEdit->toPlainText().toStdString(), selectedCategories);
            task.writeToFile("..\\lab2-OOP\\notes.bin");
            NoteWindow::notes.push_back(new Task(task));
        }
    } else {
        current->setTitle(ui->titleLineEdit->text().toStdString());
        current->setText(ui->contentTextEdit->toPlainText().toStdString());
        current->setCategories(selectedCategories);
    }
//    qDebug() << "Note saved!";
}

void CreateEditeWindow::on_buttonBox_accepted()
{
    if (ui->titleLineEdit->text() != "") {
        saveNote();
    } else {
        auto res = QMessageBox::question(this, "Збереження", "Зберегти нотатку без заголовка?");
        if (res == QMessageBox::Yes)
            saveNote();
    }
//    qDebug() << "Creation window closed 'accepted'!";
}


void CreateEditeWindow::on_buttonBox_rejected()
{
    if (ui->titleLineEdit->text() != "") {
        auto res = QMessageBox::question(this, "Збереження", "Зберегти нотатку?");
        if (res == QMessageBox::Yes)
            saveNote();
    }
//    qDebug() << "Creation window closed 'rejected'!";
}

