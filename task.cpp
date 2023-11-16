#include "task.h"
#include <fstream>

Task::Task() {
    done = false;
}

Task::Task(std::string title, std::string text) : Note(title, text) {
    done = false;
}

Task::Task(std::string title, std::string text, std::vector<std::string> categories) : Note(title, text, categories) {
    done = false;
}

Task::Task(const Note& note, bool done) : Note(note.title, note.text, note.categories) {
    this->done = done;
}

Task::~Task() {}

bool Task::isDone() const {
    return done;
}

void Task::switchState() {
    done = !done;
}

void Task::writeToFile(std::string filename) const {
    Note::writeToFile(filename);

    std::ofstream outfile(filename, std::ios::app | std::ios::binary);
    outfile.write(reinterpret_cast<const char*>(&done), sizeof(bool));
    outfile.close();
}
