#include "Containers/Tables/SparseTable.h"
#include <iostream>

int main() {

    VLIB::SparseTable<int> table;

    table.insertNode(1, 1, 100);
    table.insertNode(1, 2, 200);
    table.insertNode(1, 6, 300);
    table.insertNode(2, 1, 100);
    table.insertNode(2, 2, 200);
    table.insertNode(2, 16, 300);
    table.insertNode(300, 100, 100);

    table.printAll();


    return 0;
}
