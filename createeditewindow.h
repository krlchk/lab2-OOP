#ifndef CREATEEDITEWINDOW_H
#define CREATEEDITEWINDOW_H

#include "qlistwidget.h"
#include <QDialog>

#include "note.h"
#include "task.h"

namespace Ui {
class CreateEditeWindow;
}

enum class CreateEditeMode { CREATE, EDITE };

class CreateEditeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreateEditeWindow(QWidget *parent = nullptr, CreateEditeMode mode = CreateEditeMode::CREATE, Note* current = nullptr);
    ~CreateEditeWindow();

public slots:
    void switchCategory(QListWidgetItem* item);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CreateEditeWindow *ui;

    CreateEditeMode mode;

    Note* current;

    std::vector<std::string> selectedCategories;

    void configureListWidget();
    void saveNote();
};

#endif // CREATEEDITEWINDOW_H
