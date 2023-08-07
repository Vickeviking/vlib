// min heap tree

#ifndef MIN_HEAP_TREE_H
#define MIN_HEAP_TREE_H

#include <vector>

/*
    Min Heap Tree is a complete binary tree where the value of each node is less than or equal to the value of its children.
    The root node will always contain the smallest value.
*/
namespace VLIB{

template <class T>
class MinHeapTree{
private:
    std::vector<T> _vector;
    void BubbleDown(int index);
    void BubbleUp(int index);
    void Heapify();

public:
    MinHeapTree(T* array, int lenght);
    MinHeapTree(const std::vector<T>& vector);
    MinHeapTree();

    void Insert(T value);
    T GetMin();
    void DeleteMin();

    // Helper function for visualization
    const std::vector<T> getHeapVector() const{ return _vector; }
    const bool empty() const { return _vector.empty(); }
};

template <class T>
MinHeapTree<T>::MinHeapTree(T* array, int length) : _vector(length)
{
    for(int i = 0; i < length; ++i)
    {
        _vector[i] = array[i];
    }

    Heapify();
}

template <class T>
MinHeapTree<T>::MinHeapTree(const std::vector<T>& vector) : _vector(vector)
{
    Heapify();
}

template <class T>
MinHeapTree<T>::MinHeapTree() 
{
}

template <class T>
void MinHeapTree<T>::Heapify()
{
    int length = _vector.size();
    for(int i=length-1; i>=0; --i)
    {
        BubbleDown(i);
    }
}

template <class T>
void MinHeapTree<T>::BubbleDown(int index)
{
    int length = _vector.size();
    int leftChildIndex = 2*index + 1;
    int rightChildIndex = 2*index + 2;

    if(leftChildIndex >= length)
        return; //index is a leaf

    int minIndex = index;

    if(_vector[index] > _vector[leftChildIndex])
    {
        minIndex = leftChildIndex;
    }
    
    if((rightChildIndex < length) && (_vector[minIndex] > _vector[rightChildIndex]))
    {
        minIndex = rightChildIndex;
    }

    if(minIndex != index)
    {
        //need to swap
        int temp = _vector[index];
        _vector[index] = _vector[minIndex];
        _vector[minIndex] = temp;
        BubbleDown(minIndex);
    }
}

template <class T>
void MinHeapTree<T>::BubbleUp(int index)
{
    if(index == 0)
        return;

    int parentIndex = (index-1)/2;

    if(_vector[parentIndex] > _vector[index])
    {
        int temp = _vector[parentIndex];
        _vector[parentIndex] = _vector[index];
        _vector[index] = temp;
        BubbleUp(parentIndex);
    }
}

template <class T>
void MinHeapTree<T>::Insert(T newValue)
{
    int length = _vector.size();
    _vector[length] = newValue;

    BubbleUp(length);
}

template <class T>
T MinHeapTree<T>::GetMin()
{
    return _vector[0];
}
    
template <class T>
void MinHeapTree<T>::DeleteMin()
{
    int length = _vector.size();

    if(length == 0)
    {
        return;
    }
    
    _vector[0] = _vector[length-1];
    _vector.pop_back();

    BubbleDown(0);
}

}
#endif // MIN_HEAP_TREE_H
