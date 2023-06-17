#include "Containers/Tables/SparseTable.h"
#include <iostream>

int main() {

    VLIB::SparseTable<int> table;

    table.insertNode(1, 4, 100);
    table.insertNode(1, 2, 200);
    table.insertNode(1, 1, 500);
    table.insertNode(2, 3, 300);
    table.insertNode(2, 1, 600);
    table.insertNode(3, 1, 700);
    table.insertNode(3, 2, 800);
    table.insertNode(3, 3, 900);
    table.insertNode(6, 4, 1000);
    table.insertNode(6, 1, 100);
    table.insertNode(600, 200, 200);

    table.printAll();



    return 0;
}
