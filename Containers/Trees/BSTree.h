// Purpose: Generic Binary Search Tree
#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include <queue>
#include <stack>
#include <iostream>
using namespace std;

namespace VLIB{

/*
* Stack
*/
template<class T> 
class Stack : public stack<T> 
{
    public:
    T pop() {
        T tmp = this->top();
        stack<T>::pop();
    return tmp; }
}; 

/*
* Queue
*/
template<class T>
class Queue : public queue<T> {
public:
    T dequeue() {
        T tmp = this->front();
        queue<T>::pop();
    return tmp; }
    void enqueue(const T& el) {
        push(el);
} };

/*
*  Binary Search Tree Node
*/
template<class T>
class BSTNode {
public:
    BSTNode() {
        left = right = 0;
    }
    BSTNode(const T& e, BSTNode<T> *l = 0, BSTNode<T> *r = 0) {
        el = e; left = l; right = r;
    }
    T el;
    BSTNode<T> *left, *right;
};

/*
*  Binary Search Tree
*/
template<class T>
class BSTree {
public:
    BSTree() {
        root = 0;
}
~BSTree() {
clear(); }
    void clear() {
        clear(root); root = 0;
    }
    bool isEmpty() const {
        return root == 0;
    }
    void preorder() {
        preorder(root);
    }
    void inorder() {
        inorder(root);
    }
    void postorder() {
        postorder(root);
    }
    T* search(const T& el) const {
        return search(root,el);
}
void breadthFirst();
void iterativePreorder();
void iterativeInorder();
void iterativePostorder();
void MorrisInorder();
void insert(const T&);
void deleteByMerging(BSTNode<T>*&); 
void findAndDeleteByMerging(const T&); 
void deleteByCopying(BSTNode<T>*&); 
void balance(T*,int,int);

protected:
    BSTNode<T>* root;
void clear(BSTNode<T>*);
T* search(BSTNode<T>*, const T&) const; // Figure 6.9
    void preorder(BSTNode<T>*);
    void inorder(BSTNode<T>*);
    void postorder(BSTNode<T>*);
    virtual void visit(BSTNode<T>* p) {
        cout << p->el << ' ';
    }
};

template<class T>
T* BSTree<T>::search(BSTNode<T>* p, const T& el) const {
    while (p != 0)
        if (el == p->el)
             return &p->el;
        else if (el < p->el)
             p = p->left;
        else p = p->right;
    return 0; 
}

template<class T>
void BSTree<T>::breadthFirst() {
    Queue<BSTNode<T>*> queue;
    BSTNode<T> *p = root;
    if (p != 0) {
        queue.enqueue(p);
        while (!queue.empty()) {
            p = queue.dequeue();
            visit(p);
            if (p->left != 0)
                 queue.enqueue(p->left);
            if (p->right != 0)
                queue.enqueue(p->right);
        }       
    }
}


}
#endif