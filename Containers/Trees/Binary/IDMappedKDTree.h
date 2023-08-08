// k-dimensional tree
#ifndef IDMappedKDTree_H
#define IDMappedKDTree_H

#include <array>
#include <vector>
#include <unordered_map>

// K-dimensional tree that stores cords among a K-dimensional space with an associated userData
// while inserting a point, a unique ID is generated and returned
// OBS: THE USER MUST KEEP TRACK OF THE UNIQUE ID TO BE ABLE TO RETRIEVE THE USER DATA AND HANDLE THE TREE

namespace VLIB{


// Node of a KD Tree
 template <class CoordType, std::size_t KDimensions>
    struct KDNode {
    std::array<CoordType, KDimensions> point;      // Coordinates of the point in K-dimensional space
    std::uint64_t uniqueID;      // Unique ID associated with the point
    KDNode<CoordType, KDimensions>* left;          // Pointer to the left child node
    KDNode<CoordType, KDimensions>* right;         // Pointer to the right child node

    // Private function to generate a unique ID
    static std::uint64_t generateUniqueID() {
        static std::uint64_t idCounter = 0; // static variable to keep track of the ID
        return ++idCounter;
    };

    // Constructor with unique ID generation
    KDNode(const std::array<CoordType, KDimensions>& p) : point(p), 
    uniqueID(generateUniqueID()), left(nullptr), right(nullptr) {}
    };


// KD Tree that stores points in K-dimensional space
template <class CoordType, std::size_t KDimensions, class UserData>
class IDMappedKDTree {
private:
    KDNode<CoordType, KDimensions>* root;
    std::unordered_map<std::uint64_t, std::tuple<UserData, std::array<CoordType, KDimensions>>> dataMap;

    // Helper functions
    std::size_t getTreeDepth() const;

public:
    // constructor & destructor
    IDMappedKDTree() : root(nullptr) {}
    ~IDMappedKDTree() { delete root; }

    // Manage the tree
    std::uint64_t insert(const std::array<CoordType, KDimensions>& point, const UserData& userData);
    void remove(std::uint64_t uniqueID);
    void balance();
    void clear();
    std::size_t size() const;
    std::uint64_t nearestNeighbor(const std::array<CoordType, KDimensions>& point) const;
    std::vector<std::uint64_t> nearestNeighborsWithinRange(const std::array<CoordType, KDimensions>& point, CoordType maxDistance) const;
    std::vector<std::uint64_t> rangeSearch(const std::array<CoordType, KDimensions>& point, CoordType distance) const;
    bool contains(std::uint64_t uniqueID) const;

    // Manage user-defined data associated with a point & KD-tree synchronization
    void setPointCoordinates(std::uint64_t uniqueID, const std::array<CoordType, KDimensions>& newCoordinates);
    void setUserData(std::uint64_t uniqueID, const UserData& userData);
    const UserData& getUserData(std::uint64_t uniqueID) const;
    const std::array<CoordType, KDimensions>& getCoordinates(std::uint64_t uniqueID) const;
};

// Manage the tree
template <class CoordType, std::size_t KDimensions, class UserData>
std::uint64_t IDMappedKDTree<CoordType, KDimensions, UserData>::insert(const std::array<CoordType, KDimensions>& point, const UserData& userData) {
    // Insert point into the KD-tree
    std::uint64_t uniqueID = KDNode<CoordType, KDimensions>::generateUniqueID();
    dataMap[uniqueID] = std::make_tuple(userData, point);
    // ... continue with inserting the point into the KD-tree ...
    return uniqueID;

    //TODO far from complete, be careful doing this function, check the book examples from drozdek data structures and algorithms in c++
}

// Manage user-defined data associated with a point & KD-tree synchronization
template <class CoordType, std::size_t KDimensions, class UserData>
void IDMappedKDTree<CoordType, KDimensions, UserData>::setPointCoordinates(std::uint64_t uniqueID, const std::array<CoordType, KDimensions>& newCoordinates) {
    auto it = dataMap.find(uniqueID);
    if (it != dataMap.end()) {
        std::get<1>(it->second) = newCoordinates; // Update coordinates in the tuple
        // Optionally, update the KD-tree internal structure if needed
        // TODO:  UPDATE KD-TREE INTERNAL STRUCTURE HERE
    }
}

template <class CoordType, std::size_t KDimensions, class UserData>
void IDMappedKDTree<CoordType, KDimensions, UserData>::setUserData(std::uint64_t uniqueID, const UserData& userData) {
    auto it = dataMap.find(uniqueID);
    if (it != dataMap.end()) {
        std::get<0>(it->second) = userData; // Update user data in the tuple
    }
}


// Retrieve user data using unique ID
template <class CoordType, std::size_t KDimensions, class UserData>
const UserData& IDMappedKDTree<CoordType, KDimensions, UserData>::getUserData(std::uint64_t uniqueID) const {
    return std::get<0>(dataMap.at(uniqueID));
}

// Retrieve coordinates using unique ID
template <class CoordType, std::size_t KDimensions, class UserData>
const std::array<CoordType, KDimensions>& IDMappedKDTree<CoordType, KDimensions, UserData>::getCoordinates(std::uint64_t uniqueID) const {
    return std::get<1>(dataMap.at(uniqueID));
}



}

#endif // IDMappedKDTree_H
