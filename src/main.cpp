#include "../Containers/Trees/MultiWay/BPlusTree.h"

using namespace VLIB;

int main() {

    // Create a B+ tree with a specific order (N and M)
    BPlusTree<int, 4, 4> bPlusTree;

    // Insert some values into the tree
    bPlusTree.Insert(10);

    // print 
    bPlusTree.Print();

    return 0;
}


