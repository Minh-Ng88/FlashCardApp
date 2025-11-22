#ifndef FLASHCARDLIST_H
#define FLASHCARDLIST_H

#include <string>

struct Flashcard {
    int id;
    std::string word;
    std::string meaning;
    std::string pronunciation;
    std::string example;
    std::string topic;
    bool learned;
};

struct Node {
    Flashcard data;
    Node* next;
    Node* prev;
};

class FlashcardList {
public:
    FlashcardList();
    ~FlashcardList();

    void loadFromFile(const std::string&);
    void saveToFile(const std::string&);

    void add(const Flashcard&);
    void update(const Flashcard&);
    bool next();
    bool prev();
    bool search(const std::string&);
    void removeCurrent();
    void removeLearned();
    int count();
    int countLearned();

    Flashcard* current();

private:
    Node* head;
    Node* tail;
    Node* currentNode;
    int size;
    int nextId;
};

#endif
