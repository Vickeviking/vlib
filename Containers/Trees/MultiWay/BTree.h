#ifndef BTree_H
#define BTree_H

#include <stdexcept>
#include <iostream>

namespace VLIB
{
// A node in a BTree with a maximum of n children
template <typename T, int NumOfChildren>
class BTreeNode {
private:
    T data[NumOfChildren - 1];  // Array to store data
    BTreeNode<T, NumOfChildren> *children[NumOfChildren];  // Array of child pointers
    int numElements;  // Number of elements currently in the node
    BTreeNode<T, NumOfChildren> *parent;  // Pointer to the parent node

public:
    BTreeNode();
    
    // Getters and setters
    int getNumElements();
    bool isFull() { return getNumElements() == (NumOfChildren - 1); }
    bool isLeaf();
    bool isEmpty() { return getNumElements() == 0; }
    bool hasSpace() { return getNumElements() < (NumOfChildren - 1); }
    T* getData() { return data; }
    T getElement(int index) { return data[index]; }
    T _getHighestElement();
    BTreeNode<T, NumOfChildren>** getChildren() { return children; }
    BTreeNode<T, NumOfChildren>* getParent() { return parent; }
    BTreeNode<T, NumOfChildren>* getChild(int index) { return children[index]; }
    BTreeNode<T, NumOfChildren>* getLeftSibling();
    BTreeNode<T, NumOfChildren>* getRightSibling();

    void setChild(int index, BTreeNode<T, NumOfChildren> *child) { children[index] = child; }
    void setParent(BTreeNode<T, NumOfChildren> *parent) { this->parent = parent; }
    void replaceElement(int index, T data) { this->data[index] = data; }
    int findChildIndex(BTreeNode<T, NumOfChildren> *child);
    void removeChild(int index) { children[index] = nullptr; }

    // Methods for B-tree operations
    bool search(T data);
    void insert(const T& value);
    T* remove(T data);
    T removeElement(int index);
    void clearElements();
};

// A BTree with a maximum of n children
template <typename T, int n>
class BTree {
private:
    BTreeNode<T, n> *root;

    void _insert(T data);
    void _remove(T data);
    bool _search(T data, BTreeNode<T, n>* node);
    BTreeNode<T, n> * _splitNode(BTreeNode<T, n> *node);
    void _mergeNodes(BTreeNode<T, n> *currentNode, int indexOfCurrentNode);
    void _leftRotate(BTreeNode<T, n> *currentNode, int indexOfCurrentNode);
    void _replaceWithSmallest(BTreeNode<T, n> *currentNode);
    void _printTree();
    void _inOrderTraversal(BTreeNode<T, n> *node, int depth);

public:
    BTree();
    ~BTree();
    
    void insert(T data) { _insert(data); }
    void remove(T data) { _remove(data); }
    bool search(T data) { return _search(data, root); }
    void printTree() { _printTree(); }
};


//** Body for node struct **//

template <typename T, int NumOfChildren>
BTreeNode<T, NumOfChildren>::BTreeNode() {
    numElements = 0;
    parent = nullptr;
    for (int i = 0; i < NumOfChildren; i++) {
        children[i] = nullptr;
    }
}

template <typename T, int NumOfChildren>
int BTreeNode<T, NumOfChildren>::getNumElements() {
    return numElements; // Simply return the tracked numElements variable
}


template <typename T, int NumOfChildren>
bool BTreeNode<T, NumOfChildren>::isLeaf() { //returns true if the node is a leaf
    for (int i = 0; i < NumOfChildren; i++) {
        if (children[i] != nullptr) {
            return false;
        }
    }
    return true;
}

template <typename T, int NumOfChildren>
bool BTreeNode<T, NumOfChildren>::search(T dataToFind) {
    // Traverse the element array and see if one matches the dataToFind
    for (int i = 0; i < numElements; i++) {
        if (dataToFind == data[i]) {
            return &data[i]; // Return a pointer to the matching element
        }
    }
    return false; // Data not found
}


template <typename T, int NumOfChildren>
void BTreeNode<T, NumOfChildren>::insert(const T& value) {
    // Check if the node is full
    if (isFull()) {
        throw std::overflow_error("Node is full");
    }

    int i = 0;
    // Find the correct position to insert 'value' among the existing data elements
    while (i < getNumElements() && data[i] < value) {
        i++;
    }

    // Shift elements to the right to make space for 'value' if node is not empty
    if (!isEmpty()) {
    for (int j = getNumElements() - 1; j >= i; j--) {
        data[j + 1] = data[j];
    }
    }

    // Insert 'value' at the correct position
    data[i] = value;

    // Increment the number of elements in the node
    numElements++;
}



template <typename T, int NumOfChildren>
T *BTreeNode<T, NumOfChildren>::remove(T data)
{
    // remove data from node and return it
    for (int i = 0; i < NumOfChildren; i++) {
        if (data == data[i]) {
            data[i] = nullptr;
            return data[i];
        }
    }
}

template <typename T, int NumOfChildren>
T BTreeNode<T, NumOfChildren>::removeElement(int index) {
    if (index >= 0 && index < numElements) {
        T removedElement = data[index]; // Store the element to be removed

        // Shift the elements to the left to remove the specified element
        for (int i = index; i < numElements - 1; i++) {
            data[i] = data[i + 1];
        }

        numElements--; // Decrement the count of elements
        return removedElement; // Return the removed element
    } else {
        // Handle the case where the index is out of bounds
        // You can throw an exception or return a default value as needed
        // For simplicity, we'll return a default-constructed value here
        return T(); // Replace with an appropriate default value for your type
    }
}


template <typename T, int NumOfChildren>
void BTreeNode<T, NumOfChildren>::clearElements() {
    // Clear all elements from the node
    for (int i = 0; i < numElements; i++)
    {
        data[i] = 0;
    }
    numElements = 0;
    
}

template <typename T, int NumOfChildren>
T BTreeNode<T, NumOfChildren>::_getHighestElement(){
    this->getNumElements();
    if (numElements > 0) {
        return data[numElements - 1]; // Return the last element (the highest)
    } else {
        // Handle the case where the node is empty (no highest element)
        // You can throw an exception or return a default value as needed
        // For simplicity, we'll return a default-constructed value here
        return T(); // Replace with an appropriate default value for your type
    }
}


template <typename T, int NumOfChildren>
BTreeNode<T, NumOfChildren>* BTreeNode<T, NumOfChildren>::getLeftSibling()
{
    // get the left sibling of the node
    // if the node is the leftmost child, return nullptr
    if (parent->findChildIndex(this) == 0) {
        return nullptr;
    } else {
        return parent->getChild(parent->findChildIndex(this) - 1);
    }
}

template <typename T, int NumOfChildren>
BTreeNode<T, NumOfChildren>* BTreeNode<T, NumOfChildren>::getRightSibling()
{
    // get the right sibling of the node
    // if the node is the rightmost child, return nullptr
    if (parent->findChildIndex(this) == NumOfChildren - 1) {
        return nullptr;
    } else {
        return parent->getChild(parent->findChildIndex(this) + 1);
    }
}

template <typename T, int NumOfChildren>
int BTreeNode<T, NumOfChildren>::findChildIndex(BTreeNode<T, NumOfChildren> *child)
{
    for (int i = 0; i < NumOfChildren; i++) {
        if (children[i] == child) {
            return i;
        }
    }
    return -1;
}


//** Body for BTree class **//

template <typename T, int n>
BTree<T, n>::BTree() {
    root = nullptr;
    }

template <typename T, int n>
BTree<T, n>::~BTree() {
    // delete all nodes in the tree
    delete root;
    }
template <typename T, int n>
void BTree<T, n>::_insert(T data) {
    // Check if the root node is null, and create it if needed
    if (!root) {
        root = new BTreeNode<T, n>();
        root->insert(data); // Insert the data into the new root
        return; // Return immediately, as the root node now contains the data
    }

    // Start at the root node
    BTreeNode<T, n> *currentNode = root;
    BTreeNode<T, n> *parentNode = nullptr; // Keep track of the parent node

    // Traverse the tree to find the leaf node where the data should be inserted
    while (!currentNode->isLeaf()) {
        // Save the current node as the parent before moving to the child
        parentNode = currentNode;

        // Find the child node where data should be inserted
        int i = 0;
        while (i < currentNode->getNumElements() && data >= currentNode->getElement(i)) {
            i++;
        }
        currentNode = currentNode->getChild(i);
    }

    // Now 'currentNode' is a leaf node where we can insert the data
    if (currentNode->isFull()) {
        // If the leaf node is full, split it, the node being promoted is returned
        BTreeNode<T, n> *newNode = _splitNode(currentNode); // new node is the node that was promoted, parent of currentNode is updated

        // Check if the promoted node is the root node
        if(currentNode == root) { root = newNode; }
        // since the leaf node is no longer full, we can insert the data
        insert(data);
        return;
    }

    // Insert the data into the leaf node
    currentNode->insert(data);

}




// Helper function to split a node into two nodes and return the node that was promoted
template <typename T, int n>
BTreeNode<T, n> *BTree<T, n>::_splitNode(BTreeNode<T, n> *node) {
    // Step 1: Create two children nodes
    BTreeNode<T, n> *leftChild = new BTreeNode<T, n>();
    BTreeNode<T, n> *rightChild = new BTreeNode<T, n>();

    // Step 2: Calculate the middle index and element
    int middleIndex = (n - 1) / 2;
    T middleElement = node->getElement(middleIndex);

    // Step 3: Copy elements to the left and right children nodes
    for (int i = 0; i < middleIndex; i++) {
        leftChild->insert(node->getElement(i));
    }
    for (int i = middleIndex + 1; i < n -1; i++) {
        rightChild->insert(node->getElement(i));
    }


    // Step 4: Connect the children nodes to the parent node
    leftChild->setParent(node);
    rightChild->setParent(node);
    node->setChild(0, leftChild);
    node->setChild(1, rightChild);

    // Step 5: Update the original node with the middle element
    node->clearElements(); // Clear all elements from the original node
    node->insert(middleElement);

    // Return the node that was promoted
    return node;
}



template <typename T, int n>
void BTree<T, n>::_remove(T data) {
    // Start at the root node
    BTreeNode<T, n> *currentNode = root;

    // Traverse the tree to find the node containing the key 'data'
    while (currentNode) {
        int i = 0;
        while (i < currentNode->getNumElements() && data > currentNode->getElement(i)) {
            i++;
        }
        
        if (i < currentNode->getNumElements() && data == currentNode->getElement(i)) {
            // Case 1: Key 'data' is found in the current node
            currentNode->removeElement(i);

            // Re-balance the tree if needed
            if (currentNode != root && currentNode->getNumElements() < (n - 1) / 2) {
                BTreeNode<T, n> *leftSibling = currentNode->getLeftSibling();
                BTreeNode<T, n> *rightSibling = currentNode->getRightSibling();

                // Case 1a: Left sibling has more than the minimum required elements
                if (leftSibling && leftSibling->getNumElements() > (n - 1) / 2) {
                    _leftRotate(currentNode, i - 1);
                }
                // Case 1b: Right sibling has more than the minimum required elements
                else if (rightSibling && rightSibling->getNumElements() > (n - 1) / 2) {
                    _leftRotate(currentNode, i);
                }
                // Case 1c: Both siblings have exactly the minimum required elements
                else if (leftSibling) {
                    _mergeNodes(leftSibling, i - 1);
                    currentNode = leftSibling;
                } else if (rightSibling) {
                    _mergeNodes(currentNode, i);
                }
            }

            return; // Key removed
        }

        // Move to the appropriate child node
        currentNode = currentNode->getChild(i);
    }

    // Key 'data' not found in the tree
    // Handle the case where the key is not present
}


template <typename T, int n>
bool BTree<T, n>::_search(T data, BTreeNode<T, n>* node) {
    // Base Case 1: If the current node is a leaf node and the element is not found, return false
    if (node->isLeaf()) {
        return node->search(data);
    }

    // Base Case 2: If the element is found in the current node, return true
    if (node->search(data)) {
        return true;
    }

    // Find the appropriate child node to traverse to
    int i = 0;
    while (i < node->getNumElements() && data > node->getElement(i)) {
        i++;
    }

    // Recursively call _search on the chosen child node
    return _search(data, node->getChild(i));
}





// Perform a left rotation between the current node and its right sibling
template <typename T, int n>
void BTree<T, n>::_leftRotate(BTreeNode<T, n> *currentNode, int indexOfCurrentNode) {
    BTreeNode<T, n> *rightSibling = currentNode->getParent()->getChild(indexOfCurrentNode + 1);

    // Move a key from the parent node to the left node
    currentNode->insert(currentNode->getParent()->getElement(indexOfCurrentNode));
    currentNode->getParent()->replaceElement(indexOfCurrentNode, rightSibling->getElement(0));

    // Move the first child pointer from the right node to the current node
    currentNode->setChild(currentNode->getNumElements(), rightSibling->getChild(0));

    // Remove the first key from the right node
    rightSibling->removeElement(0);
    rightSibling->removeChild(0);
}

// Merge the current node with its right sibling
template <typename T, int n>
void BTree<T, n>::_mergeNodes(BTreeNode<T, n> *currentNode, int indexOfCurrentNode) {
    BTreeNode<T, n> *rightSibling = currentNode->getParent()->getChild(indexOfCurrentNode + 1);

    // Move a key from the parent node to the left node
    currentNode->insert(currentNode->getParent()->getElement(indexOfCurrentNode));
    currentNode->getParent()->removeElement(indexOfCurrentNode);

    // Move keys and child pointers from the right node to the current node
    for (int i = 0; i < rightSibling->getNumElements(); i++) {
        currentNode->insert(rightSibling->getElement(i));
        currentNode->setChild(currentNode->getNumElements(), rightSibling->getChild(i));
    }

    // Set the last child pointer of the current node
    currentNode->setChild(currentNode->getNumElements(), rightSibling->getChild(rightSibling->getNumElements()));

    // Remove the right sibling node from the parent
    currentNode->getParent()->removeChild(indexOfCurrentNode + 1);
}

// Replace the current node with the smallest element in the right subtree
template <typename T, int n>
void BTree<T, n>::_replaceWithSmallest(BTreeNode<T, n> *currentNode) {
    BTreeNode<T, n> *successorNode = currentNode->getChild(0);

    // Traverse down the left subtree to find the smallest element
    while (!successorNode->isLeaf()) {
        successorNode = successorNode->getChild(0);
    }

    // Replace the current node's key with the smallest key from the right subtree
    currentNode->replaceElement(0, successorNode->getElement(0));

    // Recursively delete the smallest element from the right subtree
    _remove(successorNode->getElement(0));
}

// Debug
template <typename T, int n>
void BTree<T, n>::_printTree() {
    _inOrderTraversal(root, 0);
}

template <typename T, int n>
void BTree<T, n>::_inOrderTraversal(BTreeNode<T, n> *node, int depth) {
    if (node) {
        // Recursively traverse and print the right subtree
        _inOrderTraversal(node->getChild(node->getNumElements()), depth + 1);

        // Print indentation based on depth
        for (int i = 0; i < depth; i++) {
            std::cout << "    "; // Use four spaces for each level of depth
        }

        // Print the elements in the current node
        T *data = node->getData();
        for (int i = 0; i < node->getNumElements(); i++) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl; // Add a newline for clarity

        // Recursively traverse and print the left subtree
        _inOrderTraversal(node->getChild(0), depth + 1);
    }
}




}

#endif // BTree_H