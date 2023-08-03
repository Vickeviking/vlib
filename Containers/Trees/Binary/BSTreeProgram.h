#ifndef __BSTREEPROGRAM_H__
#define __BSTREEPROGRAM_H__


#include <iostream>
#include "BSTree.h"
#include <vector>


using namespace std;   

namespace VLIB{
 
void BSTreeClearConsole()
{
    cout << "\n \n \n \n \n \n \n \n \n \n \n \n \n \n \n ";
}

int drawBSTreeMenu() 
{
    int choice;
    cout << "Binary Search Tree Program" << endl;
    cout << "1. Insert" << endl;
    cout << "2. Delete" << endl;
    cout << "3. Search" << endl;
    cout << "4. Print" << endl;
    cout << "5. Clear" << endl;
    cout << "6. Balance" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;

}

void startBSTreeProgram()
{
    int choice;
    int el;
    char ch;
    BSTree<int> tree;
    while (1) {
        choice = drawBSTreeMenu();
        switch (choice) {
            case 1: 
                    BSTreeClearConsole();
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
                            if(tree.search(temp) == 0)
                                tree.insert(temp);
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
                            if(tree.search(temp) == 0)
                                tree.insert(temp);
                        }
                    }

                    BSTreeClearConsole();
                    cout << "Inserted " << el << " elements "<< "\n" << endl;
                    break;
            case 2: BSTreeClearConsole();
                    cout << "Enter integer to delete: ";
                    cin >> el;
                    tree.findAndDeleteByMerging(el);
                    BSTreeClearConsole();
                    break;
            case 3: BSTreeClearConsole();
                    cout << "Enter integer to search: ";
                    cin >> el;
                    if (tree.search(el) != 0)
                        cout << "Found" << endl;
                    else
                        cout << "Not found" << endl;
                    
                    cout << " Back to menu? (y/n) ";
                    cin >> ch;
                    if(ch == 'y' || ch == 'Y')
                        BSTreeClearConsole();
                    else
                        exit(1);
                    break;

            case 4: BSTreeClearConsole();
                    BSTreeClearConsole();
                    cout << " (1) print Vertical , (2) print Horizontal " << endl;
                    cout << " Enter your choice: ";
                    cin >> el;
                    cout << "\n \n \n \n \n \n \n \n \n \n \n \n \n \n \n ";
                    if(el == 1)
                        tree.TreePrinter(tree);
                    else
                        tree.printHor();
                    cout << "\n \n ";
                    cout << " Back to menu? (y/n) ";
                    cin >> ch;
                    if(ch == 'y' || ch == 'Y')
                        BSTreeClearConsole();
                    else
                        exit(1);
                    break;
            case 5: tree.clear();
                    BSTreeClearConsole();
                    cout << "Tree Cleared! \n" << endl;
                    break;
            case 6: BSTreeClearConsole();
                    cout << "Tree Balanced! \n" << endl;
                    tree.balance(tree.getInOrderVector());
                    break;
            case 7: exit(1);
            default: cout << "Wrong choice" << endl;
        }
    }


}




}

#endif // __BSTREEPROGRAM_H__
