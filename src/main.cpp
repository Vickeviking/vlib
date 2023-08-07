#include "../Containers/Trees/Binary/KdTree.h"
#include "../Containers/Trees/Binary/AVLTree.h"
#include "../Containers/Trees/Binary/BSTree.h"
#include "../Containers/Lists/LinkedLists/DLList.h"
#include "../Containers/Lists/LinkedLists/SLList.h"
#include "../Containers/Lists/LinkedLists/SkipList.h"

//include binary trees examples
#include "../Containers/Trees/Binary/Examples/AVLTreeProgram.h"


#include <iostream>
int main() {

    using namespace VLIB;

    AVLTree<int> *tree = new AVLTree<int>();

    AVLTreeProgram<int> program(tree);

    program.startTreeProgram();

    return 0;
}
