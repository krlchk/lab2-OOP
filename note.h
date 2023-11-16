#ifndef NOTE_H
#define NOTE_H

#include <string>
#include <vector>

class Note
{
public:
    std::string title;
    std::string text;
    std::vector<std::string> categories;
public:
    Note();
    Note(std::string, std::string);
    Note(std::string, std::string, std::vector<std::string>);

    virtual ~Note();

    std::string getTitle() const;
    std::string getText() const;
    std::string getShortText() const;

    void setTitle(std::string);
    void setText(std::string);
    void setCategories(std::vector<std::string>);

    bool containCategory(std::string) const;

    void writeToFile(std::string) const;
};

#endif // NOTE_H
