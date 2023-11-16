#ifndef TASK_H
#define TASK_H

#include "note.h"

class Task : public Note
{
private:
    bool done;
public:
    Task();
    Task(std::string, std::string);
    Task(std::string, std::string, std::vector<std::string>);
    Task(const Note&, bool);

    ~Task();

    bool isDone() const;
    void switchState();

    void writeToFile(std::string) const;
};

#endif // TASK_H
