#ifndef __SPARSE_TABLE_H__
#define __SPARSE_TABLE_H__
#include <iostream>

namespace VLIB{

/// Node for the sparse table ///
template <class T>
class SparseNode
{
public:
    SparseNode(int rowID, int columnID, const T& el, SparseNode<T>* nextRow, SparseNode<T>* nextColumn)
    {
        this->rowID = rowID;
        this->columnID = columnID;
        this->el = el;
        this->nextRow = nextRow;
        this->nextColumn = nextColumn;
    }
    SparseNode<T>* nextRow;
    SparseNode<T>* nextColumn;
    T el;
    int rowID;
    int columnID;
};
/// End of Node for the sparse table ///

/// Sparse Table ///
template <class T>
class SparseTable
{
protected:
    SparseNode<T>* head;

public:
    SparseTable()
    {
        head = new SparseNode<T>(1, 1, 10, 0, 0);
    }
    ~SparseTable();
    void printAll() const;
    void printRow(int rowID) const;
    void printColumn(int columnID) const;
    void insertNode(int rowID, int columnID, const T& el);
    void deleteNode(SparseNode<T> *);
    void deleteNode(int rowID, int columnID);
    SparseNode<T>* find(int rowID, int columnID); // find the first node with the given info

};

template <class T>
SparseTable<T>::~SparseTable()
{
    // for each row delete all the columns

    SparseNode<T>* currentRow = head;

    while(currentRow != 0)
    {
        SparseNode<T>* currentColumn = currentRow;
        SparseNode<T>* nextRow = currentRow->nextRow;
        while(currentColumn != 0)
        {
            SparseNode<T> temp = *currentColumn;
            delete currentColumn;
            currentColumn = temp.nextColumn;
        }
        currentRow = nextRow;
    }

}

template <class T>
void SparseTable<T>::printAll() const
{
    SparseNode<T>* currentRow = head;

    while(currentRow != 0)
    {
        std::cout << currentRow->rowID << ": " ;
        SparseNode<T>* currentColumn = currentRow;
        while(currentColumn != 0)
        {
            std::cout << "c"<< currentColumn->columnID << ',' << currentColumn->el << " | ";
            currentColumn = currentColumn->nextColumn;
        }
        std::cout << std::endl;
        currentRow = currentRow->nextRow;
    }

}

template <class T>
void SparseTable<T>::printRow(int rowID) const
{
    SparseNode<T>* currentRow = head;

    while(currentRow != 0)
    {
        if(currentRow->rowID == rowID)
        {
            std::cout << currentRow->rowID << ": " ;
            SparseNode<T>* currentColumn = currentRow;
            while(currentColumn != 0)
            {
                std::cout << "c"<< currentColumn->columnID << ',' << currentColumn->el << " | ";
                currentColumn = currentColumn->nextColumn;
            }
            std::cout << std::endl;
            return;
        }
        currentRow = currentRow->nextRow;
    }
    std::cout << "Row not found" << std::endl;
}

template <class T>
void SparseTable<T>::printColumn(int columnID) const
{
    SparseNode<T>* currentRow = head;

    while(currentRow != 0)
    {
        SparseNode<T>* currentColumn = currentRow;
        while(currentColumn != 0)
        {
            if(currentColumn->columnID == columnID)
            {
                std::cout << "c"<< currentColumn->columnID << ',' << currentColumn->el << " | " << std::endl;
                break;
            }
            currentColumn = currentColumn->nextColumn;
        }
        currentRow = currentRow->nextRow;
    }
    std::cout << std::endl;
}

template <class T>
void SparseTable<T>::insertNode(int rowID, int columnID, const T& el)
{
    SparseNode<T>* currentRow = head;
    SparseNode<T>* currentColumn = 0;
    SparseNode<T>* lastColumnIDThatWasLower = 0;
    SparseNode<T>* lastRowIDThatWasLower = 0;
    SparseNode<T>* newNode = new SparseNode<T>(rowID, columnID, el, 0, 0);


    while(currentRow != 0)
    {

        if(currentRow->rowID != rowID) 
        {
            if(currentRow->rowID < rowID)
            {
                lastRowIDThatWasLower = currentRow;
            }
            currentRow = currentRow->nextRow;
            if(currentRow == 0)
            {
                // ROW NOT FOUND which means no column will be found
                newNode->nextRow = lastRowIDThatWasLower->nextRow;
                lastRowIDThatWasLower->nextRow = newNode;
                return;
            }

            
        }
        else
        break;
    }

    currentColumn = currentRow;
    while (currentColumn != 0)
    {
        
        if(currentColumn->columnID != columnID)
        {
            if(currentColumn->columnID < columnID)
            {
                lastColumnIDThatWasLower = currentColumn;
            } else if (lastColumnIDThatWasLower == 0)
            {
                // ROW FOUND but COLUMN NOT FOUND while it should be before current column
                newNode->nextRow = currentRow->nextRow;
                if (lastRowIDThatWasLower != 0) // there is a row before this one
                {
                    //is there a column at lastRowIDThatWasLower? that matches columnID
                    SparseNode<T>* temp = lastRowIDThatWasLower;
                    while(temp != 0)
                    {
                        if(temp->columnID == columnID)
                        {
                            temp->nextRow = newNode;
                            break;
                        }
                        temp = temp->nextColumn;
                    }
                }
                else if(columnID < currentColumn->columnID)
                    head = newNode;
                newNode->nextColumn = currentColumn;
                if (newNode != head && lastColumnIDThatWasLower != 0) //nothing to chain it to except head
                    lastColumnIDThatWasLower->nextColumn = newNode;
                break;
            }
            else
            {
                // ROW FOUND but COLUMN NOT FOUND while it should be after last column
                newNode->nextRow = currentRow->nextRow;
                if (lastRowIDThatWasLower != 0) 
                    lastRowIDThatWasLower->nextRow = newNode;

                newNode->nextColumn = lastColumnIDThatWasLower->nextColumn;
                lastColumnIDThatWasLower->nextColumn = newNode;
                break;
            }
            currentColumn = currentColumn->nextColumn;
            if(currentColumn == 0)
            {

                // COLUMN NOT FOUND but ROW FOUND
                newNode->nextRow = currentRow->nextRow;
                if(lastRowIDThatWasLower!=0)
                {
                    //is there a column at lastRowIDThatWasLower? that matches columnID
                    SparseNode<T>* temp = lastRowIDThatWasLower;
                    while(temp != 0)
                    {
                        if(temp->columnID == columnID)
                        {
                            temp->nextRow = newNode;
                            break;
                        }
                        temp = temp->nextColumn;
                    }
                }
                else if(columnID < head->columnID)
                    head = newNode;
                newNode->nextColumn = lastColumnIDThatWasLower->nextColumn;
                if (newNode != head) //nothing to chain it to except head
                    lastColumnIDThatWasLower->nextColumn = newNode;
                return;

            }
            continue;
        }
        else
        {
            //IF ORIGIN
            if(currentRow == head && currentColumn == head)
            {
                head->el = el;
                return;
            }
            // COLUMN FOUND
            newNode->nextRow = currentRow->nextRow;
            currentRow->nextRow = newNode;
            newNode->nextColumn = currentColumn->nextColumn;
            currentColumn->nextColumn = newNode;
            return;
        }            

    }
}

template <class T>
void SparseTable<T>::deleteNode(SparseNode<T> * nodeToDelete)
{
    SparseNode<T>* currentRow = head;
    SparseNode<T>* previousRow = 0;
    SparseNode<T>* currentColumn = 0;
    SparseNode<T>* previousColumn = 0;

    while(currentRow != 0)
    {
        if(currentRow->rowID != nodeToDelete->rowID)
        {
            previousRow = currentRow;
            currentRow = currentRow->nextRow;
            continue;
        }

        currentColumn = currentRow;
        while (currentColumn != 0)
        {
            if(currentColumn->columnID != nodeToDelete->columnID)
            {
                previousColumn = currentColumn;
                currentColumn = currentColumn->nextColumn;
                continue;
            }
            else
            {
                // COLUMN && ROW FOUND

                //is it the head?
                if(nodeToDelete == head)
                {
                    //first see if there is any column after
                    if(currentColumn->nextColumn != 0)
                        head = currentColumn->nextColumn;
                    else if(currentRow->nextRow != 0)
                        head = currentRow->nextRow;
                    else
                        head = 0;
                    delete nodeToDelete;
                    return;
                }

                //Any rows before?
                if(previousRow == 0) // head
                    head = currentRow->nextRow;
                else
                    previousRow->nextRow = currentRow->nextRow;
                
                //Any columns before?
                if(previousColumn != 0) 
                    previousColumn->nextColumn = currentColumn->nextColumn;

                delete currentColumn;
                return;
            }            

        }
    }
    throw std::invalid_argument("Node not found");
}

template <class T>
void SparseTable<T>::deleteNode(int rowID, int columnID) 
{
    deleteNode(find(rowID, columnID));
}

template <class T>
SparseNode<T>* SparseTable<T>::find(int rowID, int columnID)
{
    SparseNode<T>* currentRow = head;
    SparseNode<T>* currentColumn = 0;
    while(currentRow != 0)
    {
        if(currentRow->rowID != rowID)
        {
            currentRow = currentRow->nextRow;
            continue;
        }

        currentColumn = currentRow;
        while (currentColumn != 0)
        {
            if(currentColumn->columnID != columnID)
            {
                currentColumn = currentColumn->nextColumn;
                continue;
            }
            else
            {
                // COLUMN && ROW FOUND
                return currentColumn;
            }            

        }
    }
    return 0;
  }
}
#endif // __SPARSE_TABLE_H__


