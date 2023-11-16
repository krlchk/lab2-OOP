#include "notewindow.h"
#include "ui_notewindow.h"

#include <QMessageBox>

#include "createeditewindow.h"

std::vector<Note*> NoteWindow::notes = std::vector<Note*>();

NoteWindow::NoteWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NoteWindow)
{
    ui->setupUi(this);

    configureCategoriesListWidget();
    QObject::connect(ui->categoriesListWidget, SIGNAL(itemChanged(QListWidgetItem*)),
                     this, SLOT(switchCategory(QListWidgetItem*)));
    loadNotes();
    configureNotesListWidget();
    QObject::connect(ui->notesListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this, SLOT(selectNote(QListWidgetItem*)));
}

NoteWindow::~NoteWindow()
{
    delete ui;
}

void NoteWindow::configureCategoriesListWidget(){
    QStringList strList;
    std::vector<std::string> categories = loadCategories();
    for (auto& item : categories)
        strList << item.c_str();

    ui->categoriesListWidget->addItems(strList);

    QListWidgetItem* item = 0;
    for(int i = 0; i < ui->categoriesListWidget->count(); ++i){
        item = ui->categoriesListWidget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

void NoteWindow::configureNotesListWidget(){
    ui->notesListWidget->clear();
    QStringList strList;
    for (size_t i = 0; i < notes.size(); i++)
        if (notes[i]->getTitle() != "") strList << (std::to_string(i + 1) + ": " + notes[i]->getTitle()).c_str();
        else strList << (std::to_string(i + 1) + ": " + notes[i]->getShortText()).c_str();

    ui->notesListWidget->addItems(strList);

//    QListWidgetItem* item = 0;
//    for(int i = 0; i < ui->categoriesListWidget->count(); ++i){
//        item = ui->notesListWidget->item(i);
//        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
//        item->setCheckState(Qt::Unchecked);
//    }
}

void NoteWindow::switchCategory(QListWidgetItem *item){
    // ...
}

void NoteWindow::selectNote(QListWidgetItem *item){
    int index = QString(item->text().toStdString().substr(0, item->text().toStdString().find_first_of(':')).c_str()).toInt();
    CreateEditeWindow(nullptr, CreateEditeMode::EDITE, notes[index - 1]).exec();
    configureNotesListWidget();
    std::ofstream out("..\\lab2-OOP\\notes.bin", std::ios::out | std::ios::binary);
    out.close();
    for (int i = 0; i < notes.size(); i++) {
        Task* task = dynamic_cast<Task*>(notes[i]);
        if (!task)
            notes[i]->writeToFile("..\\lab2-OOP\\notes.bin");
        else
            task->writeToFile("..\\lab2-OOP\\notes.bin");
    }
}

std::vector<std::string> NoteWindow::loadCategories()
{
    std::ifstream fin("..\\lab2-OOP\\categories.bin", std::ios::in | std::ios::binary);

    std::vector<std::string> categories;

    int length;
    fin.read(reinterpret_cast<char*>(&length), sizeof(int));
    while (!fin.eof())
    {
        char* str = new char[length + 1];
        fin.read(str, length + 1);
        categories.push_back(str);
        fin.read(reinterpret_cast<char*>(&length), sizeof(int));
    }

    fin.close();

    return categories;
}

void NoteWindow::loadNotes()
{
    std::ifstream infile("..\\lab2-OOP\\notes.bin", std::ios::binary);

    while (infile.is_open() && !infile.eof())
    {
        Note note;
        size_t titleSize, textSize, categoriesSize;
        infile.read(reinterpret_cast<char*>(&titleSize), sizeof(size_t));
        if (infile.eof()) break;
        note.title.resize(titleSize);
        infile.read(&note.title[0], titleSize);

        infile.read(reinterpret_cast<char*>(&textSize), sizeof(size_t));
        note.text.resize(textSize);
        infile.read(&note.text[0], textSize);

        infile.read(reinterpret_cast<char*>(&categoriesSize), sizeof(size_t));
        note.categories.resize(categoriesSize);
        for (size_t i = 0; i < categoriesSize; ++i)
        {
            size_t categorySize;
            infile.read(reinterpret_cast<char*>(&categorySize), sizeof(size_t));
            note.categories[i].resize(categorySize);
            infile.read(&note.categories[i][0], categorySize);
        }

        if (!note.containCategory("Завдання"))
            notes.push_back(new Note(note));
        else
        {
            bool done;
            infile.read(reinterpret_cast<char*>(&done), sizeof(bool));
            Task task(note, done);
            notes.push_back(new Task(task));
        }
    }

    infile.close();
}

void NoteWindow::on_createAction_triggered()
{
    CreateEditeWindow().exec();
    configureNotesListWidget();
}

void NoteWindow::on_exitAction_triggered()
{
    this->destroy();
}

void NoteWindow::on_aboutAction_triggered()
{
    QMessageBox::information(this, "Інформація", "Програма для нотаток і завдань.");
}
