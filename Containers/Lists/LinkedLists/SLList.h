#ifndef __SLLIST_H__
#define __SLLIST_H__

#include <iostream>
#include "../../../flags.h"

namespace VLIB {

/// NODE CLASS ///

template <class T> // T is a data type that is not known yet
class SLNode {
public:
    SLNode() : next(nullptr) {}
    SLNode(const T& el) : info(el), next(nullptr) {}

    T info;
    SLNode* next;
};

/// END NODE CLASS ///


/// LIST CLASS ///

template <class T>
class SLList {
protected:
    VLIB::selfOrganizingListFlags organizeFlag;
    SLNode<T>* head;

public:
    SLList() : head(nullptr), organizeFlag(VLIB::NO_FLAG) {}
    SLList(VLIB::selfOrganizingListFlags flag) : head(nullptr), organizeFlag(flag) {}
    ~SLList();

    int isEmpty() const { return head == nullptr; }
    void printAll() const;
    void insertNode(const T& el);
    void deleteNode(const T& el);
    T* find(const T& el); // find the first node with the given info
};

template <class T>
SLList<T>::~SLList() {
    SLNode<T>* current = head;
    while (current != nullptr) {
        SLNode<T>* temp = current;
        current = current->next;
        delete temp;
    }
}

template <class T>
void SLList<T>::printAll() const {
    SLNode<T>* current = head;
    while (current != nullptr) {
        std::cout << current->info << " ";
        current = current->next;
    }
}

template <class T>
void SLList<T>::insertNode(const T& el) {
    SLNode<T>* newNode = new SLNode<T>(el);
    if (head == nullptr) {
        head = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
}

template <class T>
void SLList<T>::deleteNode(const T& el) {
    if (head == nullptr)
        return;

    if (head->info == el) {
        SLNode<T>* temp = head;
        head = head->next;
        delete temp;
    } else {
        SLNode<T>* prevNode = head;
        SLNode<T>* current = head->next;
        while (current != nullptr) {
            if (current->info == el) {
                prevNode->next = current->next;
                delete current;
                break;
            }
            prevNode = current;
            current = current->next;
        }
    }
}

template <class T>
T* SLList<T>::find(const T& el) {
    SLNode<T>* current = head;
    while (current != nullptr) {
        if (current->info == el)
            return &(current->info);
        current = current->next;
    }
    return nullptr;
}

} // namespace VLIB

#endif // __SLLIST_H__
