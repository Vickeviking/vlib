// Purpose: Generic Binary Search Tree
#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include <queue>
#include <stack>
#include <vector>
#include <iostream>
#include <iomanip>
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
~BSTree() {clear(); }
void clear() {clear(root); root = 0;}
bool isEmpty() const {return root == 0;}
void preorder() {preorder(root);}
void inorder() {inorder(root);}
void postorder() {postorder(root);}
T* search(const T& el) const {return search(root,el);}
BSTNode<T>* searchNode(const T& el) const {return searchNode(root,el);}
void breadthFirst();
void iterativePreorder();
void iterativeInorder();
void iterativePostorder();
void MorrisInorder();
vector<T>* getInOrderVector() const;
void insert(const T&);
void deleteByMerging(BSTNode<T>*&); 
void findAndDeleteByMerging(const T&); 
void deleteByCopying(BSTNode<T>*&); 
void balance(vector<T>*);
void printHor();
void printHor(const string &prefix, BSTNode<T> *parent, bool isLeft);
void TreePrinter(const BSTree<T>& tree);
BSTNode<T>* getRoot() const { return root; }

protected:
    BSTNode<T>* root;
    void clear(BSTNode<T>*);
    T* search(BSTNode<T>*, const T&) const; 
    BSTNode<T>* searchNode(BSTNode<T>*, const T&) const;
    void preorder(BSTNode<T>*);
    void inorder(BSTNode<T>*);
    void postorder(BSTNode<T>*);
    int countNodes(BSTNode<T>*);
    void balanceRecursive(vector<T>* data, int start, int end);
    virtual void visit(BSTNode<T>* p) {
        cout << p->el << ' ';
    }

    int height(const BSTNode<T> *root);
    int getcol(int h);
    void printTree(int **M, const BSTNode<T> *root, int col, int row, int height);
};

/*
*   Protected Methods
*/

template<class T>
void BSTree<T>::clear(BSTNode<T> *p) {
    if (p != 0) {
        clear(p->left);
        clear(p->right);
        delete p;
    }
}

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
BSTNode<T>* BSTree<T>::searchNode(BSTNode<T>* p, const T& el) const {
    while (p != 0)
        if (el == p->el)
             return p;
        else if (el < p->el)
             p = p->left;
        else p = p->right;
    return 0; 
}
template<class T>
void BSTree<T>::inorder(BSTNode<T> *p) {
     if (p != 0) {
         inorder(p->left);
         visit(p);
         inorder(p->right);
} }
template<class T>
void BSTree<T>::preorder(BSTNode<T> *p) {
    if (p != 0) {
        visit(p);
        preorder(p->left);
        preorder(p->right);
    }
}
template<class T>
void BSTree<T>::postorder(BSTNode<T>* p) {
    if (p != 0) {
        postorder(p->left);
        postorder(p->right);
        visit(p);
} }

template<class T>
int BSTree<T>::countNodes(BSTNode<T> *p) {
    if (p == 0) return 0;
    else return 1 + countNodes(p->left) + countNodes(p->right);
    
}

template<class T>
void BSTree<T>::balanceRecursive(vector<T>* data, int start, int end) {
    if (start > end)
        return;

    // Find the middle element of the array
    int middle = (start + end) / 2;

    // Insert the middle element into the tree
    insert((*data)[middle]);

    // Recursively balance the left and right subtrees
    balanceRecursive(data, start, middle - 1);
    balanceRecursive(data, middle + 1, end);
}


template <class T>
int BSTree<T>::height(const BSTNode<T> *root) {
    if (root == NULL)
        return 0;
    return max(height(root->left), height(root->right)) + 1;
}

template <class T>
int BSTree<T>::getcol(int h) {
    if (h == 1)
        return 1;
    return getcol(h - 1) + getcol(h - 1) + 1;
}
 
template <class T>
void BSTree<T>::printTree(int **M, const BSTNode<T> *root, int col, int row, int height) {
    if (root == NULL)
        return;
    
    int numWidth = std::to_string(root->el).length(); // Get the width of the number
    
    M[row][col] = root->el;
    printTree(M, root->left, col - pow(2, height - 2), row + 1, height - 1);
    printTree(M, root->right, col + pow(2, height - 2), row + 1, height - 1);
}

/*
*  Public Methods
*/

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

template<class T>
void BSTree<T>::iterativePreorder() {
    Stack<BSTNode<T>*> travStack;
    BSTNode<T> *p = root;
    if (p != 0) {
        travStack.push(p);
        while (!travStack.empty()) {
            p = travStack.pop();
            visit(p);
            if (p->right != 0)
                travStack.push(p->right);
            if (p->left != 0)   // left child pushed after right
                travStack.push(p->left); // to be on the top of
                                // the stack;
        }
    }
}

template<class T>
void BSTree<T>::iterativePostorder() {
    Stack<BSTNode<T>*> travStack;
    BSTNode<T>* p = root, *q = root;
    while (p != 0) {
        for ( ; p->left != 0; p = p->left)
            travStack.push(p);
        while (p->right == 0 || p->right == q) {
            visit(p);
            q = p;
            if (travStack.empty())
                 return;
            p = travStack.pop();
        }
        travStack.push(p);
        p = p->right;
} }

template<class T>
void BSTree<T>::iterativeInorder() {
    Stack<BSTNode<T>*> travStack;
    BSTNode<T> *p = root;
    while (p != 0) {
        while (p != 0) {
            if (p->right) // stack the right child (if any)
                travStack.push(p->right);
            travStack.push(p); // and the node itself when going 
            p = p->left; // to the left;
        }
        p = travStack.pop(); // pop a node with no left child 
        while (!travStack.empty() && p->right == 0) { // visit it
        visit(p);
        p = travStack.pop();
        }
        visit(p);
        if (!travStack.empty())
            p = travStack.pop();
        else p = 0;
    } 
}

template<class T> // makes tree into a single right path
void BSTree<T>::MorrisInorder() { // no stack or threads used!
    BSTNode<T> *p = root, *tmp;
    while (p != 0)
        if (p->left == 0) {          // if no left subtree 
             visit(p);               // visit this node
             p = p->right;           // and go to the right
        }
        else {                       // if left subtree exists
            tmp = p->left;           // make a pointer to it.
            while (tmp->right != 0 && tmp->right != p)  // go to the rightmost node
                tmp = tmp->right;                       
            if (tmp->right == 0) {   // if we're here the first time
                tmp->right = p;      // thread the node to p 
                p = p->left;         // go to left subtree
            }
            else {                   // 2nd time here
                visit(p);            // visit this node
                tmp->right = 0;      // thread back to normal
                p = p->right;        // go right while thread is complete
            }
        }
}
template<class T>
vector<T>* BSTree<T>::getInOrderVector() const {
    // Use Morris Inorder traversal to get the array, instead of visit(p), add p->el to the array

    // Create the vector on the heap
    vector<T>* v = new vector<T>();

    BSTNode<T>* p = root;
    BSTNode<T>* tmp;
    while (p != nullptr) {
        if (p->left == nullptr) {
            v->push_back(p->el);
            p = p->right;
        } else {
            tmp = p->left;
            while (tmp->right != nullptr && tmp->right != p)
                tmp = tmp->right;
            if (tmp->right == nullptr) {
                tmp->right = p;
                p = p->left;
            } else {
                v->push_back(p->el);
                tmp->right = nullptr;
                p = p->right;
            }
        }
    }

    return v;
}



template<class T>
void BSTree<T>::insert(const T& el) {
    BSTNode<T> *p = root, *prev = 0;
    while (p != 0) {  // find a place for inserting new node;
        prev = p;
        if (el < p->el)
             p = p->left;
        else p = p->right;
    }
    if (root == 0)    // tree is empty;
         root = new BSTNode<T>(el);
    else if (el < prev->el)
         prev->left   = new BSTNode<T>(el);
    else prev->right  = new BSTNode<T>(el);
}

template<class T>
void BSTree<T>::deleteByMerging(BSTNode<T>*& node) {
    BSTNode<T> *tmp = node;
    if (node != 0) {
        if (!node->right)
            node = node->left;
        else if (node->left == 0) 
            node = node->right;
        else {
            tmp = node->left;
            while (tmp->right != 0)
                tmp = tmp->right;
            tmp->right =
                node->right;

            tmp = node;
            node = node->left;
        }
        delete tmp;
    }
}

template<class T>
void BSTree<T>::findAndDeleteByMerging(const T& el) {
    BSTNode<T> *node = root, *prev = 0;
    while (node != 0) {
        if (node->el == el)
             break;
        prev = node;
        if (el < node->el)
             node = node->left;
        else node = node->right;
    }
    if (node != 0 && node->el == el)
         if (node == root)
              deleteByMerging(root);
         else if (prev->left == node)
              deleteByMerging(prev->left);
         else deleteByMerging(prev->right);
    else if (root != 0)
         cout << "element" << el << "is not in the tree\n";
    else cout << "the tree is empty\n";
}

template<class T>
void BSTree<T>::deleteByCopying(BSTNode<T>*& node) {
    BSTNode<T> *previous, *tmp = node;
    if (node->right == 0)
      node = node->left;
    else if (node->left == 0)
      node = node->right;
    else {
      tmp = node->left;
      previous = node;
      while (tmp->right != 0) {
          previous = tmp;
          tmp = tmp->right;
      }
      node->el = tmp->el;
      if (previous == node)
           previous ->left  = tmp->left;
      else previous ->right = tmp->left;
    }
    delete tmp;
}


template<class T>
void BSTree<T>::balance(vector<T>* data) {

    // Clear the tree
    clear();
    // Build a balanced tree from the sorted array
    balanceRecursive(data, 0, data->size() - 1);

}


template<class T>
void BSTree<T>::printHor()
{
    if(root != nullptr)
        printHor("", root, false);
    else
        cout << "Empty tree!" << endl;
}



template<class T>
void BSTree<T>::printHor(const string &prefix, BSTNode<T> *parent, bool isLeft)
{
    if (parent != nullptr)
    {
        cout << prefix;

        cout << (isLeft ? "L├──" : "R└──");

        // print the value of the node
        cout << parent->el << endl;

        // enter the next tree level - left and right branch
        printHor(prefix + (isLeft ? " │   " : "    "), parent->left, true);
        printHor(prefix + (isLeft ? " │   " : "    "), parent->right, false);
    }
}
 

template <class T>
void BSTree<T>::TreePrinter(const BSTree<T>& tree) {
    int h = height(tree.root);
    int col = getcol(h);
    int **M = new int*[h];
    for (int i = 0; i < h; i++) {
        M[i] = new int[col];
    }
    printTree(M, tree.root, col / 2, 0, h);
    
    int maxNumWidth = 0; // Maximum width of numbers in the tree
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < col; j++) {
            if (M[i][j] != 0) {
                int numWidth = std::to_string(M[i][j]).length();
                if (numWidth > maxNumWidth) {
                    maxNumWidth = numWidth;
                }
            }
        }
    }
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < col; j++) {
            if (M[i][j] == 0) {
                std::cout << std::setw(maxNumWidth + 1) << " ";
            } else {
                std::cout << std::setw(maxNumWidth) << M[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
    
    // Clean up the allocated memory
    for (int i = 0; i < h; i++) {
        delete[] M[i];
    }
    delete[] M;
}

} /* namespace closing bracket */

#endif /* BSTREE_H_ */
