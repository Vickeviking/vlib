#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>

/*

    A dynamic sized array, which is a resizable array.
    The size of the array is determined at runtime.

    Initialization:
        DynamicArray<int> arr;

*/

namespace VLIB {

template <class T>
class DynamicArray {
private:
    T* data;
    size_t size;
    size_t capacity;

public:
    // Constructor
    DynamicArray();

    // Destructor
    ~DynamicArray();

    // Accessor methods
    size_t getSize() const;
    size_t getCapacity() const;
    bool isEmpty() const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    // Mutator methods
    void pushBack(const T& value);
    void popBack();
    void resize(size_t newCapacity);

    // Test function
    void TestFunction();
};

// DynamicArray.cpp

// Constructor
template <typename T>
DynamicArray<T>::DynamicArray() : data(nullptr), size(0), capacity(0) {}

// Destructor
template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

// Accessor methods
template <typename T>
size_t DynamicArray<T>::getSize() const {
    return size;
}

template <typename T>
size_t DynamicArray<T>::getCapacity() const {
    return capacity;
}

template <typename T>
bool DynamicArray<T>::isEmpty() const {
    return size == 0;
}

template <typename T>
T& DynamicArray<T>::operator[](size_t index) {
    if (index < size) {
        return data[index];
    } else {
        // Handle out-of-bounds error
        // For example, you can throw an exception or return a default value
        throw std::out_of_range("Index out of bounds");
    }
}

template <typename T>
const T& DynamicArray<T>::operator[](size_t index) const {
    if (index < size) {
        return data[index];
    } else {
        throw std::out_of_range("Index out of bounds");
    }
}

// Mutator methods
template <typename T>
void DynamicArray<T>::pushBack(const T& value) {
    if (size == capacity) {
        // If the array is full, resize it
        resize(capacity * 2 + 1);
    }
    data[size++] = value;
}

template <typename T>
void DynamicArray<T>::popBack() {
    if (!isEmpty()) {
        --size;
    }
}

// Test function
template <typename T>
void DynamicArray<T>::TestFunction() {
    // Implement your test function body here
}

// Private helper method
template <typename T>
void DynamicArray<T>::resize(size_t newCapacity) {
    T* newData = new T[newCapacity];
    for (size_t i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    capacity = newCapacity;
    delete[] data;
    data = newData;
}



} //namespace VLIB

#endif // DYNAMIC_ARRAY_H
