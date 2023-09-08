#include "../Containers/Trees/MultiWay/BTree.h"

using namespace VLIB;

int main() {


    // Create a B-tree with a maximum of 'n' children
    BTree<int, 4> tree; // Example: using int data type and a maximum of 4 children per node

    // Insert some data into the tree
    tree.insert(5);
    tree.insert(10);
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    tree.insert(8);

    // Print the tree to visualize its structure
    std::cout << "B-tree structure:" << std::endl;
    tree.printTree();

    // Search for a specific element
    int searchData = 3;
    bool found = tree.search(searchData);
    if (found) {
        std::cout << "Found " << searchData << " in the tree." << std::endl;
    } else {
        std::cout << searchData << " not found in the tree." << std::endl;
    }

    // Remove an element from the tree
    int removeData = 7;
    tree.remove(removeData);

    // Print the updated tree structure
    std::cout << "Updated B-tree structure:" << std::endl;
    tree.printTree();

    return 0;
}


