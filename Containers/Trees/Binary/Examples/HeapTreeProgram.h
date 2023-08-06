#include <iostream>
#include <vector>
#include <iomanip>

#include "../MinHeapTree.h"
#include "../MaxHeapTree.h"

/* an example of using HeapTree class

    HeapTreeProgram<int> heapTreeProgram;
    heapTreeProgram.startHeapTreeProgram();

*/


using namespace std;

// MinHeapTree class

#ifndef MINHEAPTREE_PROGRAM_H
#define MINHEAPTREE_PROGRAM_H


namespace VLIB{

// HeapTreeProgram class
template <class T>
class HeapTreeProgram {
private:
    MinHeapTree<T> minHeap;
    MaxHeapTree<T> maxHeap;
    bool isMinHeapMode; // This variable will store the selected mode

    // Helper function for visualization
    void visualizeTree(const vector<T>& values, int index, int level) const;
    void clearScreen() const{ cout << string( 100, '\n' );}

public:
    HeapTreeProgram();

    void startHeapTreeProgram();
    int drawHeapTreeMenu();
    void visualize() const;
    void restart() const;
};

// HeapTreeProgram Implementation
template <class T>
HeapTreeProgram<T>::HeapTreeProgram() : minHeap(), maxHeap(), isMinHeapMode(true) {}

template <class T>
void HeapTreeProgram<T>::startHeapTreeProgram() {
    int choice;
    T value;

    //create a try n catch block that welcomes the user and asks them to choose a mode
    try {
        cout << "Welcome to the Heap Tree Program!" << endl;
        cout << "Please choose a mode:" << endl;
        cout << "1. Min Heap" << endl;
        cout << "2. Max Heap" << endl;
        cout << "Choice: ";
        cin >> choice;

        //if the user chooses 1, then the program will run in min heap mode
        if (choice == 1) {
            isMinHeapMode = true;
            cout << "Min Heap Mode" << endl;
        }
        //if the user chooses 2, then the program will run in max heap mode
        else if (choice == 2) {
            isMinHeapMode = false;
            cout << "Max Heap Mode" << endl;
        }
        //if the user chooses anything else, then the program will throw an exception
        else {
            throw "Invalid choice!";
        }
        clearScreen();

    }
    //catch the exception and print out the error message
    catch (const char* msg) {
        cerr << msg << endl;
    }


    while (true) {
        choice = drawHeapTreeMenu();
        switch (choice) {
            case 1:
                clearScreen();
                cout << "Enter the value to insert: ";
                cin >> value;
                if (isMinHeapMode)
                    minHeap.Insert(value);
                else
                    maxHeap.Insert(value);
                restart();
                break;
            case 2:
                clearScreen();
                if (isMinHeapMode) {
                    if (!minHeap.empty()) {
                        cout << "Min Value: " << minHeap.GetMin() << endl;
                    } else {
                        cout << "Min Heap is empty!" << endl;
                    }
                } else {
                    if (!maxHeap.empty()) {
                        cout << "Max Value: " << maxHeap.GetMax() << endl;
                    } else {
                        cout << "Max Heap is empty!" << endl;
                    }
                }
                restart();
                break;
            case 3:
                clearScreen();
                if (isMinHeapMode)
                    minHeap.DeleteMin();
                else
                    maxHeap.DeleteMax();   
                restart();
                break;
            case 4:
                clearScreen();
                visualize();
                restart();
                break;
            case 5:
                clearScreen();
                isMinHeapMode = !isMinHeapMode;
                cout << (isMinHeapMode ? "Switched to Min Heap mode." : "Switched to Max Heap mode.") << endl;
                restart();
                break;
            case 6:
                clearScreen();
                exit(0);
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}

template <class T>
int HeapTreeProgram<T>::drawHeapTreeMenu() {
    clearScreen();
    int choice;
    cout << "Heap Tree Program" << endl;
    cout << "1. Insert" << endl;
    cout << "2. Get Min/Max" << endl;
    cout << "3. Delete Min/Max" << endl;
    cout << "4. Visualize Heap" << endl;
    cout << "5. Switch Mode" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

template <class T>
void HeapTreeProgram<T>::restart() const {
   
    cout << "Press any key to continue...";
    cin.ignore();
    cin.get();
    clearScreen();

}

template <class T>
void HeapTreeProgram<T>::visualize() const {
    if (isMinHeapMode) {
        visualizeTree(minHeap.getHeapVector(), 0, 0);
    } else {
        visualizeTree(maxHeap.getHeapVector(), 0, 0);
    }
}

template <class T>
void HeapTreeProgram<T>::visualizeTree(const vector<T>& values, int index, int level) const {
    if (index < values.size()) {
        visualizeTree(values, 2 * index + 2, level + 1);
        cout << setw(4 * level) << "";
        cout << values[index] << endl;
        visualizeTree(values, 2 * index + 1, level + 1);
    }
}
}
#endif // MINHEAPTREE_PROGRAM_H
