#ifndef MAX_HEAP_TREE_H
#define MAX_HEAP_TREE_H

#include <vector>

/*
    Max Heap Tree is a complete binary tree where the value of each node is greater than or equal to the value of its children.
    The root node will always contain the largest value.
*/

namespace VLIB{

template <class T>
class MaxHeapTree{
private:
    std::vector<T> _vector;
    void BubbleDown(int index);
    void BubbleUp(int index);
    void Heapify();

public:
    MaxHeapTree(T* array, int length);
    MaxHeapTree(const std::vector<T>& vector);
    MaxHeapTree();

    void Insert(T value);
    T GetMax();
    void DeleteMax();

    // Helper function for visualization
    const std::vector<T> getHeapVector() const{ return _vector; }
    const bool empty() const { return _vector.empty(); }
};

template <class T>
MaxHeapTree<T>::MaxHeapTree(T* array, int length) : _vector(length)
{
    for(int i = 0; i < length; ++i)
    {
        _vector[i] = array[i];
    }

    Heapify();
}

template <class T>
MaxHeapTree<T>::MaxHeapTree(const std::vector<T>& vector) : _vector(vector)
{
    Heapify();
}

template <class T>
MaxHeapTree<T>::MaxHeapTree() 
{
}

template <class T>
void MaxHeapTree<T>::Heapify()
{
    int length = _vector.size();
    for(int i=length-1; i>=0; --i)
    {
        BubbleDown(i);
    }
}

template <class T>
void MaxHeapTree<T>::BubbleDown(int index)
{
    int length = _vector.size();
    int leftChildIndex = 2*index + 1;
    int rightChildIndex = 2*index + 2;

    if(leftChildIndex >= length)
        return; //index is a leaf

    int maxIndex = index;

    if(_vector[index] < _vector[leftChildIndex])
    {
        maxIndex = leftChildIndex;
    }
    
    if((rightChildIndex < length) && (_vector[maxIndex] < _vector[rightChildIndex]))
    {
        maxIndex = rightChildIndex;
    }

    if(maxIndex != index)
    {
        //need to swap
        T temp = _vector[index];
        _vector[index] = _vector[maxIndex];
        _vector[maxIndex] = temp;
        BubbleDown(maxIndex);
    }
}

template <class T>
void MaxHeapTree<T>::BubbleUp(int index)
{
    if(index == 0)
        return;

    int parentIndex = (index-1)/2;

    if(_vector[parentIndex] < _vector[index])
    {
        T temp = _vector[parentIndex];
        _vector[parentIndex] = _vector[index];
        _vector[index] = temp;
        BubbleUp(parentIndex);
    }
}

template <class T>
void MaxHeapTree<T>::Insert(T newValue)
{
    int length = _vector.size();
    _vector.push_back(newValue);

    BubbleUp(length);
}

template <class T>
T MaxHeapTree<T>::GetMax()
{
    return _vector[0];
}
    
template <class T>
void MaxHeapTree<T>::DeleteMax()
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
#endif // MAX_HEAP_TREE_H
