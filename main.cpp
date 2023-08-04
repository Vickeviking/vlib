#include "Containers/Trees/Binary/AVLTreeProgram.h"
#include "Containers/Trees/Binary/AVLTree.h"

using namespace VLIB;

int main() {

    AVLTree<int> *tree = new AVLTree<int>();
    AVLTreeProgram<int> program(tree);
    program.startTreeProgram();

    return 0;
}
