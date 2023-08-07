#ifndef __BSTREEPROGRAM_H__
#define __BSTREEPROGRAM_H__


#include <iostream>
#include <vector>

#include "../BSTree.h"

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
class BSTreeProgram{
private: 
    BSTree<T> *tree;

    void ClearConsole(){cout << "\n \n \n \n \n \n \n \n \n \n \n \n \n \n \n ";}
    int drawBSTreeMenu();
    void printHor();
    void printHor(const string &prefix, BSTNode<T> *parent, bool isLeft);
    void TreePrinter(const BSTree<T>& tree);
    int height(const BSTNode<T> *root);
    int getcol(int h);
    void printTree(int **M, const BSTNode<T> *root, int col, int row, int height);

public:
    BSTreeProgram(BSTree<T> *tree):tree(tree){}
    ~BSTreeProgram(){}
    void startTreeProgram();

};


// Program related 

template <class T>
void BSTreeProgram<T>::startTreeProgram()
{
    int choice;
    int el;
    char ch;
    BSTree<T> *MyTree = tree;

    while (1) {
        choice = drawBSTreeMenu();
        switch (choice) {
            case 1: 
                    ClearConsole();
                    cout << "(1) Insert custom element, (2) Insert random elements " << endl;
                    cout << "Enter your choice: ";
                    cin >> el;
                    if(el == 1)
                    {
                        cout << "How many integers to insert:  ";
                        cin >> el;
                        // add the integers to the tree
                        for (int i = 0; i < el; i++)
                        {
                            int temp;
                            cin >> temp;
                            // check if the input exists
                            if(MyTree->search(temp) == 0)
                                MyTree->insert(temp);
                        }
                    }
                    else
                    {
                        cout << "How many integers to insert:  ";
                        cin >> el;
                        cout << "" << endl;
                        int min, max;
                        cout << "Enter lower bound: ";
                        cin >> min;
                        cout << "" << endl;
                        cout << "Enter upper bound: ";
                        cin >> max;

                        for (int i = 0; i < el; i++)
                        {
                            int temp = min + (rand() % (int)(max - min + 1));
                            if(MyTree->search(temp) == 0)
                                MyTree->insert(temp);
                        }
                    }

                    ClearConsole();
                    cout << "Inserted " << el << " elements "<< "\n" << endl;
                    break;
            case 2: ClearConsole();
                    cout << "Enter integer to delete: ";
                    cin >> el;
                    MyTree->findAndDeleteByMerging(el);
                    ClearConsole();
                    break;
            case 3: ClearConsole();
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

            case 4: ClearConsole();
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
            case 5: MyTree->Clear();
                    ClearConsole();
                    cout << "Tree Cleared! \n" << endl;
                    break;
            case 6: ClearConsole();
                    cout << "Tree Balanced! \n" << endl;
                    MyTree->balance();
                    break;
            case 7: exit(1);
            default: cout << "Wrong choice" << endl;
        }
    }
}

template <class T>
int BSTreeProgram<T>::drawBSTreeMenu()
{
    int choice;
    cout << "Binary Search Tree Program" << endl;
    cout << "1. Insert" << endl;
    cout << "2. Delete" << endl;
    cout << "3. Search" << endl;
    cout << "4. Print" << endl;
    cout << "5. Clear" << endl;
    cout << "6. Balance" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

// Printing

template <class T>
void BSTreeProgram<T>::TreePrinter(const BSTree<T>& Tree)
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
void BSTreeProgram<T>::printHor(const string &prefix, BSTNode<T> *parent, bool isLeft)
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
void BSTreeProgram<T>::printHor()
{
    BSTNode<T>* root = tree->getRoot();
    if(root != nullptr)
        printHor("", root, false);
    else
        cout << "Empty tree!" << endl;
}

template <class T>
int BSTreeProgram<T>::height(const BSTNode<T> *root) {
    if (root == NULL)
        return 0;
    return max(height(root->left), height(root->right)) + 1;
}

template <class T>
int BSTreeProgram<T>::getcol(int h) {
    if (h == 1)
        return 1;
    return getcol(h - 1) + getcol(h - 1) + 1;
}
 
template <class T>
void BSTreeProgram<T>::printTree(int **M, const BSTNode<T> *root, int col, int row, int height) {
    if (root == NULL)
        return;
    M[row][col] = root->el;
    printTree(M, root->left, col - pow(2, height - 2), row + 1, height - 1);
    printTree(M, root->right, col + pow(2, height - 2), row + 1, height - 1);
}


}

#endif // __BSTREEPROGRAM_H__
