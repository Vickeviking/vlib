// k-dimensional tree
#ifndef KDTREE_H
#define KDTREE_H

#include <array>
#include <vector>

// K-dimensional tree that stores cords among a K-dimensional space
//  Aswell as a unique ID for each point to be able to identify them
// use the ID to accociate the point with a class instance stored in a map
// use the ID as a key to the map

namespace VLIB{


// Node of a KD Tree
 template <class T, std::size_t K>
    struct KDNode {
    std::array<T, K> point;      // Coordinates of the point in K-dimensional space
    std::uint64_t uniqueID;      // Unique ID associated with the point
    KDNode<T, K>* left;          // Pointer to the left child node
    KDNode<T, K>* right;         // Pointer to the right child node

    // Private function to generate a unique ID
    static std::uint64_t generateUniqueID() {
        static std::uint64_t idCounter = 0; // static variable to keep track of the ID
        return ++idCounter;
    };

    // Constructor with unique ID generation
    KDNode(const std::array<T, K>& p) : point(p), 
    uniqueID(generateUniqueID()), left(nullptr), right(nullptr) {}
    };


// KD Tree that stores points in K-dimensional space
template <class T, std::size_t K>
class KDTree {
private:
    KDNode<T,K >* root; // Root of the KD tree

    // Additional private helper functions can go here

public:
    KDTree() : root(nullptr) {}  // Constructor
    ~KDTree() {}  // Destructor

    void insert(const std::array<T, K>& point); // Insert a point into the KD tree
    bool remove(const std::array<T, K>& point); // Remove a point from the KD tree
    bool contains(const std::array<T, K>& point) const; // Check if a point is in the KD tree

    std::vector<std::array<T, K> > rangeSearch(const std::array<T, K>& minPoint, const std::array<T, K>& maxPoint) const; 
    std::array<T, K> nearestNeighbor() const;
};
}

#endif // KDTREE_H
