
#ifndef __DLLIST_H__
#define __DLLIST_H__

#include <iostream>
#include "../../flags.h"

namespace VLIB{
/// NODE CLASS ///

template <class T> // T is a data type that is not known yet
class DLLNode
{
private:

public:
    DLLNode() {
        next = prev = 0;
    }
    DLLNode(const T& el, DLLNode *n = 0, DLLNode *p = 0, int c = 0) {
        info = el; next = n; prev = p;
    }
    T info;
    int count; // for count flag
    DLLNode *next, *prev;
};

/// END NODE CLASS ///


/// LIST CLASS ///

template <class T>
class DLList
{
protected:
    VLIB::selfOrganizingListFlags organizeFlag;
    DLLNode<T> *head, *tail;


public:

    DLList() {
        head = tail = 0;
        organizeFlag = VLIB::NO_FLAG;
    }
    DLList(VLIB::selfOrganizingListFlags flag) {
        head = tail = 0;
        organizeFlag = flag;
    }
    ~DLList();
    int isEmpty() {return head == 0;}
    void printAll() const;
    void printAllBackwards() const;
    void reverse();
    void deleteNode(DLLNode<T> *);
    void deleteNodeFromHead();
    void deleteNodeFromTail();
    void insertNodeBetween(DLLNode<T>* sucessor, DLLNode<T>* predecessor, const T& el); 
    void insertNodeFromHead(const T& el);
    void insertNodeFromTail(const T& el);
    void organizeList(DLLNode<T>* node); // node is the node that was used
    T* find(const T& el); // find the first node with the given info
};

template <class T>
DLList<T>::~DLList()
{
    for (DLLNode<T> *p; !isEmpty(); ) {
        p = head->next;
        delete head;
        head = p;
    }
}

template <class T>
void DLList<T>::printAll() const
{
    if(tail == 0 || head == 0)
        return;
    DLLNode<T>* n = head;

    while(n != tail)
    {
        std::cout << n->info << " ";
        n = n->next;
    }
    std::cout << n->info << " ";

}

template <class T>
void DLList<T>::printAllBackwards() const
{
    if(tail == 0 || head == 0)
        return;
    DLLNode<T>* n = tail;

    while(n != head)
    {
        std::cout << n->info << " ";
        n = n->prev;
    }
    std::cout << n->info << " ";
}

template <class T>
void DLList<T>::reverse()
{
    if(tail == 0 || head == 0)
        throw std::runtime_error("List is empty");
    DLLNode<T>* current = head;
    DLLNode<T>* prev = 0;
    DLLNode<T>* next = 0;

    while(current != 0)
    {
        next = current->next;
        current->next = prev;
        current->prev = next;
        prev = current;
        current = next;
    }

    tail = head;
    head = prev;
}

template <class T>
void DLList<T>::deleteNode(DLLNode<T> *p)
{

    if(tail == 0 || head == 0)
        throw std::runtime_error("List is empty");
    DLLNode<T>* n = head;
    DLLNode<T>* prev = 0;

    if(p == n) // was head
    {
        DLLNode<T>* next = n->next;
        delete head;
        next->prev = 0;
        head = next;
        return;
    }
    else if(p == tail) // was tail
    {
        prev = tail->prev;
        delete tail;
        prev->next = 0;
        tail = prev;
        return;
    }
    DLLNode<T>* next = 0;
    while (n != tail) // neither head or tail
    {
        if(n == p)
        {
            prev = n->prev;
            next = n->next;
            delete n;
            prev->next = next;
            next->prev = prev;
            return;
        }
        n = n->next;
    }
}

template <class T>
void DLList<T>::deleteNodeFromHead()
{
    if(tail == 0 || head == 0)
        throw std::runtime_error("List is empty");
    DLLNode<T>* n = head;
    DLLNode<T>* next = n->next;
    delete head;
    next->prev = 0;
    head = next;
}

template <class T>
void DLList<T>::deleteNodeFromTail()
{
    if(tail == 0 || head == 0)
        throw std::runtime_error("List is empty");
    DLLNode<T>* n = tail;
    DLLNode<T>* prev = n->prev;
    delete tail;
    prev->next = 0;
    tail = prev;
}

template <class T>
void DLList<T>::insertNodeBetween(DLLNode<T>* successor, DLLNode<T>* predecessor, const T& el)
{
    if(tail == 0 || head == 0)
        throw std::runtime_error("List is empty");
    DLLNode<T>* n = head;

    if(successor == n) // was head
    {
        DLLNode<T>* newNode = new DLLNode<T>(el, successor, 0);
        successor->prev = newNode;
        head = newNode;
        return;
    }
    else if(predecessor == tail) // was tail
    {
        DLLNode<T>* newNode = new DLLNode<T>(el, 0, predecessor);
        predecessor->next = newNode;
        tail = newNode;
        return;
    }
    while (n != tail) // neither head or tail
    {
        if(n == successor)
        {
            DLLNode<T>* newNode = new DLLNode<T>(el, successor, predecessor);
            successor->prev = newNode;
            predecessor->next = newNode;
            return;
        }
        n = n->next;
    }
}

template <class T>
void DLList<T>::insertNodeFromHead(const T& el)
{
    if(tail == 0 || head == 0)
    {
        DLLNode<T>* newNode = new DLLNode<T>(el, 0, 0);
        head = newNode;
        tail = newNode;
        return;
    }
    DLLNode<T>* newNode = new DLLNode<T>(el, head, 0);
    head->prev = newNode;
    head = newNode;
    
}

template <class T>
void DLList<T>::insertNodeFromTail(const T& el)
{
    if(tail == 0 || head == 0)
    {
        DLLNode<T>* newNode = new DLLNode<T>(el, 0, 0);
        head = newNode;
        tail = newNode;
        return;
    }
    DLLNode<T>* newNode = new DLLNode<T>(el, 0, tail);
    tail->next = newNode;
    tail = newNode;
}

template <class T>
void DLList<T>::organizeList(DLLNode<T>* node) 
{
    switch (organizeFlag)
    {
    case VLIB::MOVE_TO_FRONT:
    {
        if(node == head)
            return;
        DLLNode<T> tmp = *node;
        deleteNode(node);
        insertNodeFromHead(tmp.info);
    }
        break;
    case VLIB::TRANSPOSE:
    {
        //switch node with previous node
        if (node == head) // if head
            return;
        if (node == tail) // if tail
        {
            DLLNode<T> tmp = *node->prev;
            deleteNode(node->prev);
            insertNodeFromTail(tmp.info);
            return;
        } // if one after head
        if(node->prev == head)
        {
            DLLNode<T> tmp = *node;
            deleteNode(node);
            insertNodeFromHead(tmp.info);
            return;
        }
        // normal case atleast pos 3 and not tail
        DLLNode<T> tmp = *node;
        deleteNode(node);
        insertNodeBetween(tmp.next, tmp.prev, tmp.info);


    }
        break;
    case VLIB::COUNT:
    {
        //increment count
        node->count++;

        //if node count is less than previous node or head then return
        if(node == head || node->count < node->prev->count)
            return;
        
        // now we know node count is less than previous node count and not head

        if(node == tail) // if tail
        {
            DLLNode<T> tmp = *node->prev;
            deleteNode(node->prev);
            insertNodeFromTail(tmp.info);
            findNode(tmp.info)->count = tmp.count;
            return;
        }
        if(node->prev == head)  // if one after head
        {
            DLLNode<T> tmp = *node;
            deleteNode(node);
            insertNodeFromHead(tmp.info);
            findNode(tmp.info)->count = tmp.count;
            return;
        }
        // normal case atleast pos 3 and not tail
        DLLNode<T> tmp = *node;
        deleteNode(node);
        insertNodeBetween(tmp.next, tmp.prev, tmp.info);
        findNode(tmp.info)->count = tmp.count;

    }
        break;
    case VLIB::ORDERING:
        {
            //if node is head or prev->info < node->info then return
            if(node == head || node->prev->info < node->info)
                return;
            // same as transpose now
            if (node == tail) // if tail
            {
                DLLNode<T> tmp = *node->prev;
                deleteNode(node->prev);
                insertNodeFromTail(tmp.info);
                return;
            } // if one after head
            if(node->prev == head)
            {
                DLLNode<T> tmp = *node;
                deleteNode(node);
                insertNodeFromHead(tmp.info);
                return;
            }
            // normal case atleast pos 3 and not tail
            DLLNode<T> tmp = *node;
            deleteNode(node);
            insertNodeBetween(tmp.next, tmp.prev, tmp.info);
            

        }
        break;
    case VLIB::NO_FLAG:
        break;
    default:
        break;
    }
}

template <class T>
T* DLList<T>::find(const T& el) 
{
    if(tail == 0 || head == 0)
        throw std::runtime_error("List is empty");
    DLLNode<T>* n = head;

    while(n != tail)
    {
        if(n->info == el)
        {
            organizeList(n); //organize list based on flag before returning
            return &n->info;
        }
        n = n->next;
    }
    if(n->info == el) // check tail
    {
        organizeList(n); //organize list based on flag before returning
        return &n->info;
    }
    return 0;
}



/// END LIST CLASS ///
}

#endif // __DLLIST_H__
