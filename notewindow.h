#ifndef NOTEWINDOW_H
#define NOTEWINDOW_H

#include "qlistwidget.h"
#include <QMainWindow>

#include <iostream>
#include <fstream>

#include "note.h"
#include "task.h"

QT_BEGIN_NAMESPACE
namespace Ui { class NoteWindow; }
QT_END_NAMESPACE

class NoteWindow : public QMainWindow
{
    Q_OBJECT

public:
    NoteWindow(QWidget *parent = nullptr);
    ~NoteWindow();

    static std::vector<std::string> loadCategories();

    static std::vector<Note*> notes;

public slots:
    void switchCategory(QListWidgetItem* item);
    void selectNote(QListWidgetItem* item);

private slots:
    void on_createAction_triggered();

    void on_exitAction_triggered();

    void on_aboutAction_triggered();

private:
    Ui::NoteWindow *ui;

    void configureCategoriesListWidget();
    void configureNotesListWidget();
    void loadNotes();
};
#endif // NOTEWINDOW_H
