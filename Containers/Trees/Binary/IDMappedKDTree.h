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
template <class T, std::size_t K, class UserData>
class IDMappedKDTree {
private:
    KDNode<T,K>* root;
    std::unordered_map<std::uint64_t, UserData> userMap; // Map to store user-defined data
    
    // Helper functions
    std::size_t getTreeDepth() const;

public:

    // constructor & destructor
    IDMappedKDTree() : root(nullptr) {}
    ~IDMappedKDTree() { delete root; }

    // Manage the tree
    std::uint64_t insert(const std::array<T, K>& point, const UserData& userData); //returns handles to the unique ID
    void remove(std::uint64_t uniqueID);
    void balance();
    void clear();
    std::size_t size() const;
    // search
    std::uint64_t nearestNeighbor(const std::array<T, K>& point) const;
    std::vector<std::uint64_t> rangeSearch(const std::array<T, K>& point, T distance) const;
    bool contains(std::uint64_t uniqueID) const;


    // Manage user-defined data associated with a point & KD-tree synchronization

    void setPointCoordinates(std::uint64_t uniqueID, const std::array<T, K>& newCoordinates);
    void setUserData(std::uint64_t uniqueID, const UserData& userData);
    
    const UserData& getUserData(std::uint64_t uniqueID) const;
    const std::array<T, K>& getPointCoordinates(std::uint64_t uniqueID) const;

};

// Manage the tree

template <class T, std::size_t K, class UserData>
std::uint64_t IDMappedKDTree<T, K, UserData>::insert(const std::array<T, K>& point, const UserData& userData) {
    // Insert point into the KD-tree
    // Generate a unique ID for the point
    std::uint64_t uniqueID = KDNode<T, K>::generateUniqueID();
    // Insert the user data into the hashmap using the unique ID as the key
    userMap[uniqueID] = userData;
    // ... continue with inserting the point into the KD-tree ...

    // return the ID
    return uniqueID;
}


// Manage the user-defined data associated with a point

template <class T, std::size_t K, class UserData>
void IDMappedKDTree<T, K, UserData>::setPointCoordinates(std::uint64_t uniqueID, const std::array<T, K>& newCoordinates) {
    auto it = userMap.find(uniqueID);
    if (it != userMap.end()) {
        // Update the coordinates within the user data structure
        it->second.setCoordinates(newCoordinates); // Assuming UserData has a setCoordinates method

        // Optionally, update the KD-tree internal structure
        // This step is not strictly necessary, but it's an option to keep the tree synchronized
        // with the user data changes. It depends on how you want to manage this synchronization.
        // If you choose to update the tree here, you'll need to implement a mechanism to
        // find the node associated with the uniqueID and update its coordinates.
    }
}


}

#endif // IDMappedKDTree_H
