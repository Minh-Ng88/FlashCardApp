#include "FlashcardList.h"
#include <fstream>

FlashcardList::FlashcardList() {
    head = tail = currentNode = nullptr;
    size = 0;
    nextId = 1;
}

FlashcardList::~FlashcardList() {
    Node* p = head;
    while (p != nullptr) {
        Node* nx = p->next;
        delete p;
        p = nx;
    }
}

void FlashcardList::loadFromFile(const std::string& f){
    // clear old list
    Node* p = head;
    while (p != nullptr) {
        Node* nx = p->next;
        delete p;
        p = nx;
    }
    head = tail = currentNode = nullptr;
    size = 0;
    nextId = 1;

    std::ifstream in(f);
    if (!in) return;

    Flashcard c;
    while (std::getline(in, c.word)) {
        std::getline(in, c.meaning);
        std::getline(in, c.pronunciation);
        std::getline(in, c.example);
        std::getline(in, c.topic);
        in >> c.learned;
        in.ignore();

        c.id = nextId++;

        Node* n = new Node{ c, nullptr, tail };
        if (!head) head = n;
        if (tail) tail->next = n;
        tail = n;

        size++;
    }

    currentNode = head;
}

void FlashcardList::saveToFile(const std::string& f){
    std::ofstream out(f);
    Node* p = head;

    while (p != nullptr) {
        out << p->data.word << "\n"
            << p->data.meaning << "\n"
            << p->data.pronunciation << "\n"
            << p->data.example << "\n"
            << p->data.topic << "\n"
            << p->data.learned << "\n";
        p = p->next;
    }
}

void FlashcardList::add(const Flashcard& c){
    Flashcard newC = c;
    newC.id = nextId++;

    Node* n = new Node{ newC, nullptr, tail };
    if (!head) head = n;
    if (tail) tail->next = n;
    tail = n;

    currentNode = n;
    size++;
    saveToFile("flashcards.txt");
}

void FlashcardList::update(const Flashcard& c){
    if (!currentNode) return;
    currentNode->data = c;
    saveToFile("flashcards.txt");
}

bool FlashcardList::next(){
    if (currentNode && currentNode->next){
        currentNode = currentNode->next;
        return true;
    }
    return false;
}

bool FlashcardList::prev(){
    if (currentNode && currentNode->prev){
        currentNode = currentNode->prev;
        return true;
    }
    return false;
}

Flashcard* FlashcardList::current(){
    if (!currentNode) return nullptr;
    return &currentNode->data;
}

bool FlashcardList::search(const std::string& w){
    Node* p = head;
    while (p){
        if (p->data.word == w){
            currentNode = p;
            return true;
        }
        p = p->next;
    }
    return false;
}

void FlashcardList::removeCurrent(){
    if (!currentNode) return;

    Node* p = currentNode;

    if (p->prev) p->prev->next = p->next;
    else head = p->next;

    if (p->next) p->next->prev = p->prev;
    else tail = p->prev;

    if (p->next) currentNode = p->next;
    else currentNode = p->prev;

    delete p;
    size--;

    saveToFile("flashcards.txt");
}

void FlashcardList::removeLearned(){
    Node* p = head;
    while (p){
        Node* nx = p->next;
        if (p->data.learned){
            if (p->prev) p->prev->next = p->next;
            else head = p->next;

            if (p->next) p->next->prev = p->prev;
            else tail = p->prev;

            delete p;
            size--;
        }
        p = nx;
    }
    currentNode = head;
    saveToFile("flashcards.txt");
}

int FlashcardList::count(){ return size; }

int FlashcardList::countLearned(){
    int c = 0;
    Node* p = head;
    while (p){
        if (p->data.learned) c++;
        p = p->next;
    }
    return c;
}
