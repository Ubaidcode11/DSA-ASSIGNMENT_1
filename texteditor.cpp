#include "texteditor.h"

struct Node {
    char data;
    Node* prev;
    Node* next;
    Node(char c) : data(c), prev(nullptr), next(nullptr) {}
};

class EditorList {
public:
    Node* head;
    Node* tail;
    Node* cursor; // points to node RIGHT of cursor (nullptr if at end)

    EditorList() : head(nullptr), tail(nullptr), cursor(nullptr) {}

    void insertChar(char c) {
        Node* newNode = new Node(c);

        if (!head) { // empty
            head = tail = newNode;
            cursor = nullptr;
        }
        else if (cursor == head) { // insert at beginning
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        else if (cursor == nullptr) { // insert at end
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        else { // insert in middle
            Node* left = cursor->prev;
            left->next = newNode;
            newNode->prev = left;
            newNode->next = cursor;
            cursor->prev = newNode;
        }
    }

    void deleteChar() {
        if (!head) return;
        if (cursor == head) return;

        Node* target;
        if (cursor == nullptr) {
            target = tail;
            tail = tail->prev;
            if (tail) tail->next = nullptr;
            else head = nullptr;
        } else {
            target = cursor->prev;
            Node* left = target->prev;
            Node* right = cursor;
            if (left) left->next = right;
            else head = right;
            right->prev = left;
        }
        delete target;
    }

    void moveLeft() {
        if (!head) return;
        if (cursor == head) return;
        if (cursor == nullptr) cursor = tail;
        else cursor = cursor->prev;
    }

    void moveRight() {
        if (!head) return;
        if (cursor == nullptr) return;
        cursor = cursor->next;
    }

    std::string getTextWithCursor() const {
        std::string result;
        Node* temp = head;
        while (temp) {
            if (temp == cursor)
                result += '|';
            result += temp->data;
            temp = temp->next;
        }
        if (cursor == nullptr)
            result += '|';
        return result;
    }
};

static EditorList editor;

void TextEditor::insertChar(char c) { editor.insertChar(c); }
void TextEditor::deleteChar() { editor.deleteChar(); }
void TextEditor::moveLeft() { editor.moveLeft(); }
void TextEditor::moveRight() { editor.moveRight(); }
std::string TextEditor::getTextWithCursor() const { return editor.getTextWithCursor(); }
