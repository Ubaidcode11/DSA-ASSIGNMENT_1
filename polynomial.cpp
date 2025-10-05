#include "Polynomial.h"
#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;

// Derived class implementing Polynomial using linked list
class LinkedListPolynomial : public Polynomial {
private:
    struct Node {
        int coeff;
        int exp;
        Node* next;
        Node(int c, int e) : coeff(c), exp(e), next(nullptr) {}
    };
    Node* head;

public:
    LinkedListPolynomial() : head(nullptr) {}

    void insertTerm(int coefficient, int exponent) override {
        if (coefficient == 0) return;

        Node* newNode = new Node(coefficient, exponent);

        if (!head || exponent > head->exp) {
            newNode->next = head;
            head = newNode;
            return;
        }

        Node* current = head;
        Node* prev = nullptr;

        while (current && current->exp > exponent) {
            prev = current;
            current = current->next;
        }

        if (current && current->exp == exponent) {
            current->coeff += coefficient;
            if (current->coeff == 0) {
                if (prev == nullptr) head = current->next;
                else prev->next = current->next;
                delete current;
            }
            delete newNode;
            return;
        }

        newNode->next = current;
        if (prev) prev->next = newNode;
        else head = newNode;
    }

    string toString() const override {
        if (!head) return "0";

        ostringstream out;
        Node* current = head;
        bool first = true;

        while (current) {
            int c = current->coeff, e = current->exp;
            if (c == 0) { current = current->next; continue; }

            if (!first) out << (c > 0 ? " + " : " - ");
            else if (c < 0) out << "-";

            int absCoeff = abs(c);
            if (absCoeff != 1 || e == 0) out << absCoeff;
            if (e > 0) {
                out << "x";
                if (e > 1) out << "^" << e;
            }

            first = false;
            current = current->next;
        }

        return out.str();
    }

    Polynomial* add(const Polynomial& other) const override {
        const LinkedListPolynomial& o = dynamic_cast<const LinkedListPolynomial&>(other);
        LinkedListPolynomial* result = new LinkedListPolynomial();

        Node* p1 = head;
        Node* p2 = o.head;

        while (p1 || p2) {
            if (p2 == nullptr || (p1 && p1->exp > p2->exp)) {
                result->insertTerm(p1->coeff, p1->exp);
                p1 = p1->next;
            } else if (p1 == nullptr || p2->exp > p1->exp) {
                result->insertTerm(p2->coeff, p2->exp);
                p2 = p2->next;
            } else {
                result->insertTerm(p1->coeff + p2->coeff, p1->exp);
                p1 = p1->next;
                p2 = p2->next;
            }
        }

        return result;
    }

    Polynomial* multiply(const Polynomial& other) const override {
        const LinkedListPolynomial& o = dynamic_cast<const LinkedListPolynomial&>(other);
        LinkedListPolynomial* result = new LinkedListPolynomial();

        for (Node* t1 = head; t1; t1 = t1->next)
            for (Node* t2 = o.head; t2; t2 = t2->next)
                result->insertTerm(t1->coeff * t2->coeff, t1->exp + t2->exp);

        return result;
    }

    Polynomial* derivative() const override {
        LinkedListPolynomial* result = new LinkedListPolynomial();
        for (Node* curr = head; curr; curr = curr->next)
            if (curr->exp != 0)
                result->insertTerm(curr->coeff * curr->exp, curr->exp - 1);
        return result;
    }

    ~LinkedListPolynomial() {
        Node* curr = head;
        while (curr) {
            Node* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
    }
};
