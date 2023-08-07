#ifndef STATIC_ARRAY_H
#define STATIC_ARRAY_H

/*
    A static sized array, which is a fixed-size array.
    The size of the array is determined at compile time.

    Initialization:
        StaticArray<int, 5> arr;

*/

#include <iostream>

namespace VLIB {

template <class T, size_t Size>
class StaticArray {
private:
    T data[Size];

public:
    // Accessor methods
    size_t getSize() const;
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
};

// StaticArray.cpp

// Accessor methods
template <typename T, size_t Size>
size_t StaticArray<T, Size>::getSize() const {
    return Size;
}

template <typename T, size_t Size>
T& StaticArray<T, Size>::operator[](size_t index) {
    if (index < Size) {
        return data[index];
    } else {
        // Handle out-of-bounds error
        // For example, you can throw an exception or return a default value
        throw std::out_of_range("Index out of bounds");
    }
}

template <typename T, size_t Size>
const T& StaticArray<T, Size>::operator[](size_t index) const {
    if (index < Size) {
        return data[index];
    } else {
        throw std::out_of_range("Index out of bounds");
    }
}

} // namespace VLIB

#endif // STATIC_ARRAY_H
