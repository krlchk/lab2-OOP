#include "note.h"
#include <algorithm>
#include <fstream>

Note::Note() {
    title = "";
    text = "";
}

Note::Note(std::string title, std::string text) {
    this->title = title;
    this->text = text;
}

Note::Note(std::string title, std::string text, std::vector<std::string> categories) {
    this->title = title;
    this->text = text;
    this->categories = categories;
}

Note::~Note() {}

std::string Note::getTitle() const {
    return title;
}

std::string Note::getText() const {
    return text;
}

std::string Note::getShortText() const {
    if (text.size() <= 25)
        return text;
    return text.substr(0, 22) + "...";
}

void Note::setTitle(std::string value) {
    title = value;
}

void Note::setText(std::string value) {
    text = value;
}

void Note::setCategories(std::vector<std::string> value) {
    categories = value;
}

bool Note::containCategory(std::string category) const {
    return std::find(categories.begin(), categories.end(), category) != categories.end();
}

void Note::writeToFile(std::string filename) const {
    std::ofstream outfile(filename, std::ios::app | std::ios::binary);
    size_t titleSize = this->title.size();
    size_t textSize = this->text.size();
    size_t categoriesSize = this->categories.size();

    outfile.write(reinterpret_cast<const char*>(&titleSize), sizeof(size_t));
    outfile.write(this->title.c_str(), titleSize);
    outfile.write(reinterpret_cast<const char*>(&textSize), sizeof(size_t));
    outfile.write(this->text.c_str(), textSize);
    outfile.write(reinterpret_cast<const char*>(&categoriesSize), sizeof(size_t));
    for (const std::string& category : this->categories)
    {
        size_t categorySize = category.size();
        outfile.write(reinterpret_cast<const char*>(&categorySize), sizeof(size_t));
        outfile.write(category.c_str(), categorySize);
    }
    outfile.close();
}
