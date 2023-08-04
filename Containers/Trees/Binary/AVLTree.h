// AVL Tree //

#ifndef AVLTREE_H
#define AVLTREE_H

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <stack>

using namespace std;

/*

AVL Tree

The withcoming AVLTreeProgram can be used to interact with the AVLTree class

*/

namespace VLIB{
template <class T>
struct AVLNode{
    T key;
    int32_t height;
    AVLNode *left;
    AVLNode *right;
    AVLNode(T key):key(key),height(1),left(nullptr),right(nullptr){}
};

template <class T>
class AVLTree{

    private: 
    AVLNode<T> *root;

    //manage tree
    void makeEmpty(AVLNode<T> *node);
    AVLNode<T> *insert(T key, AVLNode<T> *node);
    AVLNode<T> *remove(T key, AVLNode<T> *node);

    //rotation
    AVLNode<T>* rightRotation(AVLNode<T>* &node);
    AVLNode<T>* leftRotation(AVLNode<T>* &node);
    AVLNode<T>* doubleRight(AVLNode<T>* &node);
    AVLNode<T>* doubleLeft(AVLNode<T>* &node);

    // helper functions
    AVLNode<T>* findMinValueNode(AVLNode<T>* node);
    AVLNode<T>* findMaxValueNode(AVLNode<T>* node);
    int32_t height(AVLNode<T> *node);
    int32_t getBalance(AVLNode<T> *node);

    //traversal
    void inorder(AVLNode<T> *node);
    void preorder(AVLNode<T> *node);
    void postorder(AVLNode<T> *node);

    public: 

    AVLTree():root(nullptr){}
    ~AVLTree(){makeEmpty(root);}

    //user interactions
    void insert(T key){root = insert(key, root);}
    void remove(T key){root = remove(key, root);}
    AVLNode<T>* search(T key);
    void clear(){makeEmpty(root);}
    // debug
    void inorder(){inorder(root);}
    void preorder(){preorder(root);}
    void postorder(){postorder(root);}
    AVLNode<T>* getRoot() const {return root;}

};

/// PRIVATE ///

    // manage tree
    template <class T>
    void AVLTree<T>::makeEmpty(AVLNode<T> *node)
    { 
        if(node == NULL)
            return;
        makeEmpty(node->left);
        makeEmpty(node->right);
        delete node;
    }

    template <class T>
    AVLNode<T>* AVLTree<T>::insert(T key, AVLNode<T> *node)
    {
        if(node == NULL)
        {
            node = new AVLNode<T>(key);
        }
        else if(key < node->key)
        {
            node->left = insert(key, node->left);
            if(height(node->left) - height(node->right) == 2)
            {
                if(key < node->left->key)
                    node = rightRotation(node);
                else
                    node = doubleRight(node);
            }
        }
        else if(key > node->key)
        {
            node->right = insert(key, node->right);
            if(height(node->right) - height(node->left) == 2)
            {
                if(key > node->right->key)
                    node = leftRotation(node);
                else
                    node = doubleLeft(node);
            }
        }
        node->height = max(height(node->left), height(node->right))+1;
        return node;
    }

    template <class T>
    AVLNode<T>* AVLTree<T>::remove(T key, AVLNode<T> *node)
    {
         AVLNode<T>* temp;

        // Element not found
        if(node == NULL)
            return NULL;

        // Searching for element
        else if(key < node->key)
            node->left = remove(key, node->left);
        else if(key > node->key)
            node->right = remove(key, node->right);

        // Element found
        // With 2 children
        else if(node->left && node->right)
        {
            temp = findMinValueNode(node->right);
            node->key = temp->key;
            node->right = remove(node->key, node->right);
        }
        // With one or zero child
        else
        {
            temp = node;
            if(node->left == NULL)
                node = node->right;
            else if(node->right == NULL)
                node = node->left;
            delete temp;
        }
        if(node == NULL)
            return node;

        node->height = max(height(node->left), height(node->right))+1;

        // If node is unbalanced
        // If left node is deleted, right case
        if(height(node->left) - height(node->right) == -2)
        {
            // right right case
            if(height(node->right->right) - height(node->right->left) == 1)
                return leftRotation(node);
            // right left case
            else
                return doubleLeft(node);
        }
        // If right node is deleted, left case
        else if(height(node->right) - height(node->left) == 2)
        {
            // left left case
            if(height(node->left->left) - height(node->left->right) == 1){
                return rightRotation(node);
            }
            // left right case
            else
                return doubleRight(node);
        }
        return node;
    }

    template <class T>
    AVLNode<T>* AVLTree<T>::search(T searchedKey)
    {
        AVLNode<T>* temp = root;
        while(temp != NULL)
        {
            if(searchedKey == temp->key)
                return temp;
            else if(searchedKey < temp->key)
                temp = temp->left;
            else
                temp = temp->right;
        }
        return NULL;
    }

    //rotation
    template <class T>
    AVLNode<T>* AVLTree<T>::rightRotation(AVLNode<T>* &node)
    {  
       if (node->left != NULL) {
			AVLNode<T>* left = node->left;
			node->left = left->right;
			left->right = node;
			node->height = max(height(node->left), height(node->right)) + 1;
			left->height = max(height(left->left), node->height) + 1;
			return left;
		}
		return node;
    }

    template <class T>
    AVLNode<T>* AVLTree<T>::leftRotation(AVLNode<T>* &node)
    {
        if (node->right != NULL) {
		    AVLNode<T>* right = node->right;
		    node->right = right->left;
		    right->left = node;
		    node->height = max(height(node->left), height(node->right)) + 1;
		    right->height = max(height(node->right), node->height) + 1;
		    return right;
            }
            return node;
    }

    template <class T>
    AVLNode<T>* AVLTree<T>::doubleRight(AVLNode<T>* &node)
    {
        node->left = leftRotation(node->left);
        return rightRotation(node);
    }

    template <class T>
    AVLNode<T>* AVLTree<T>::doubleLeft(AVLNode<T>* &node)
    {
        node->right = rightRotation(node->right);
        return leftRotation(node);
    }

    // helper functions
    template <class T>
    AVLNode<T>* AVLTree<T>::findMinValueNode(AVLNode<T>* node)
    {
        if(node == NULL)
            return NULL;
        else if(node->left == NULL)
            return node;
        else
            return findMinValueNode(node->left);
    }

    template <class T>
    AVLNode<T>* AVLTree<T>::findMaxValueNode(AVLNode<T>* node)
    {
        if(node == NULL)
            return NULL;
        else if(node->right == NULL)
            return node;
        else
            return findMaxValueNode(node->right);
    }

    template <class T>
    int32_t AVLTree<T>::height(AVLNode<T> *node)
    {
        if(node == NULL)
            return -1;
        else
            return node->height;
    }

    template <class T>
    int32_t AVLTree<T>::getBalance(AVLNode<T> *node)
    {
        if(node == NULL)
            return 0;
        else
            return height(node->left) - height(node->right);
    }

    //traversal
    template <class T>
    void AVLTree<T>::inorder(AVLNode<T> *node)
    {
        if(node == NULL)
            return;
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }

    template <class T>
    void AVLTree<T>::preorder(AVLNode<T> *node)
    {
        if(node == NULL)
            return;
        cout << node->data << " ";
        preorder(node->left);
        preorder(node->right);
    }

    template <class T>
    void AVLTree<T>::postorder(AVLNode<T> *node)
    {
        if(node == NULL)
            return;
        postorder(node->left);
        postorder(node->right);
        cout << node->data << " ";
    }


    /// PUBLIC ///



}

#endif // AVLTREE_H
