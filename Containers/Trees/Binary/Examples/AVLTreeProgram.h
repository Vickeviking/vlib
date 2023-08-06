#ifndef __AVLTREEPROGRAM_H__
#define __AVLTREEPROGRAM_H__


#include <iostream>
#include <iomanip>
#include <vector>

#include "AVLTree.h"

/*
GENERAL INFO

This is a program that allows the user to interact with the Binary Trees

initiate example:

    BSTree<int> *tree = new BSTree<int>();
    TreeProgram<int> program(tree);
    program.startTreeProgram();

*/

using namespace std;   

namespace VLIB{


template <class T>
class AVLTreeProgram{
private: 
    AVLTree<T> *tree;

    void ClearConsole(){cout << "\n \n \n \n \n \n \n \n \n \n \n \n \n \n \n ";}
    int drawBSTreeMenu();
    void printHor();
    void printHor(const string &prefix, AVLNode<T> *parent, bool isLeft);
    void TreePrinter(const AVLTree<T>& tree);
    int height(const AVLNode<T> *root);
    int getcol(int h);
    void printTree(int **M, const AVLNode<T> *root, int col, int row, int height);

public:
    AVLTreeProgram(AVLTree<T> *tree):tree(tree){}
    ~AVLTreeProgram(){}
    void startTreeProgram();

};


// Program related 

template <class T>
void AVLTreeProgram<T>::startTreeProgram()
{
    int choice;
    int el;
    char ch;
    AVLTree<T> *MyTree = tree;

    while (1) {
        choice = drawBSTreeMenu();
       switch (choice) {
                case 1: {
                int tempChoice;
                ClearConsole();
                cout << "(1) Insert custom element, (2) Insert random elements " << endl;
                cout << "Enter your choice: ";
                if (!(cin >> tempChoice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid choice." << endl;
                    break;
                }
                cin.sync(); // Clear the input buffer

                if (tempChoice == 1) {
                    int numElements;
                    cout << "How many integers to insert:  ";
                    if (!(cin >> numElements)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a valid number." << endl;
                        break;
                    }
                    cin.sync(); // Clear the input buffer

                    // add the integers to the tree
                    for (int i = 0; i < numElements; i++) {
                        int tempElement;
                        cout << "Enter integer " << (i + 1) << ": ";
                        if (!(cin >> tempElement)) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input. Please enter a valid integer." << endl;
                            i--; // Decrement i to re-enter the current integer.
                            continue;
                        }
                        cin.sync(); // Clear the input buffer

                        // check if the input exists
                        MyTree->insert(tempElement);
                    }
                    } else {
                    int numElements;
                    cout << "How many integers to insert:  ";
                    if (!(cin >> numElements)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a valid number." << endl;
                        break;
                    }
                    cin.sync(); // Clear the input buffer
                    cout << "" << endl;

                    int min, max;
                    cout << "Enter lower bound: ";
                    if (!(cin >> min)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a valid integer." << endl;
                        break;
                    }
                    cin.sync(); // Clear the input buffer
                    cout << "" << endl;

                    cout << "Enter upper bound: ";
                    if (!(cin >> max)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a valid integer." << endl;
                        break;
                    }
                    cin.sync(); // Clear the input buffer

                    for (int i = 0; i < numElements; i++) {
                        int tempNum = min + (rand() % (max - min + 1));
                        if (MyTree->search(tempNum) == nullptr)
                            MyTree->insert(tempNum);
                    }
                }

                ClearConsole();
                cout << "Inserted " << el << " elements " << "\n" << endl;
                break;
            }

            case 2: 
            {   
                ClearConsole();
                cout << "Enter integer to delete: ";
                cin >> el;
                MyTree->remove(el);
                ClearConsole();
                break;
            }
            case 3: {
                ClearConsole();
                cout << "Enter integer to search: ";
                cin >> el;
                if (MyTree->search(el) != 0)
                    cout << "Found" << endl;
                else
                    cout << "Not found" << endl;
                
                cout << " Back to menu? (y/n) ";
                cin >> ch;
                if(ch == 'y' || ch == 'Y')
                    ClearConsole();
                else
                    exit(1);
                break;
            }
            case 4: { 
                ClearConsole();
                ClearConsole();
                cout << " (1) print Vertical , (2) print Horizontal " << endl;
                cout << " Enter your choice: ";
                cin >> el;
                cout << "\n \n \n \n \n \n \n \n \n \n \n \n \n \n \n ";
                if(el == 1)
                    TreePrinter(*MyTree);
                else
                    printHor();
                cout << "\n \n ";
                cout << " Back to menu? (y/n) ";
                cin >> ch;
                if(ch == 'y' || ch == 'Y')
                    ClearConsole();
                else
                    exit(1);
                break;
            }
            case 5: {
                MyTree->clear();
                ClearConsole();
                cout << "Tree Cleared! \n" << endl;
                break;
                    }
            case 6: {
                exit(1);
                break;
                    }
            default: {
                cout << "Wrong choice" << endl;
                    break;
            }
        }
    }
}

template <class T>
int AVLTreeProgram<T>::drawBSTreeMenu()
{
    int choice;
    cout << "Binary Search Tree Program" << endl;
    cout << "1. Insert" << endl;
    cout << "2. Delete" << endl;
    cout << "3. Search" << endl;
    cout << "4. Print" << endl;
    cout << "5. Clear" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

// Printing

template <class T>
void AVLTreeProgram<T>::TreePrinter(const AVLTree<T>& Tree)
{
    int h = height(Tree.getRoot());
    int col = getcol(h);
    int **M = new int*[h];
    for (int i = 0; i < h; i++) {
        M[i] = new int[col];
    }
    printTree(M, Tree.getRoot(), col / 2, 0, h);
    
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

template <class T>
void AVLTreeProgram<T>::printHor(const string &prefix, AVLNode<T> *parent, bool isLeft)
{
    if (parent != nullptr)
    {
        cout << prefix;

        cout << (isLeft ? "L├──" : "R└──");

        // print the value of the node
        cout << parent->key << endl;

        // enter the next tree level - left and right branch
        printHor(prefix + (isLeft ? " │   " : "    "), parent->left, true);
        printHor(prefix + (isLeft ? " │   " : "    "), parent->right, false);
    }
}

template <class T>
void AVLTreeProgram<T>::printHor()
{
    AVLNode<T>* root = tree->getRoot();
    if(root != nullptr)
        printHor("", root, false);
    else
        cout << "Empty tree!" << endl;
}

template <class T>
int AVLTreeProgram<T>::height(const AVLNode<T> *root) {
    if (root == NULL)
        return 0;
    return max(height(root->left), height(root->right)) + 1;
}

template <class T>
int AVLTreeProgram<T>::getcol(int h) {
    if (h == 1)
        return 1;
    return getcol(h - 1) + getcol(h - 1) + 1;
}
 
template <class T>
void AVLTreeProgram<T>::printTree(int **M, const AVLNode<T> *root, int col, int row, int height) {
    if (root == NULL)
        return;
    M[row][col] = root->key;
    printTree(M, root->left, col - pow(2, height - 2), row + 1, height - 1);
    printTree(M, root->right, col + pow(2, height - 2), row + 1, height - 1);
}


}

#endif // __AVLTREEPROGRAM_H__
