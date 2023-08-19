// k-dimensional tree
#ifndef IDMappedKDTree_H
#define IDMappedKDTree_H

#include <array>
#include <memory>
#include <queue>
#include <utility>
#include <vector>
#include <unordered_map>
#include <functional>


// K-dimensional tree that stores cords among a K-dimensional space with an associated userData
// while inserting a point, a unique ID is generated and returned
// OBS: THE USER MUST KEEP TRACK OF THE UNIQUE ID TO BE ABLE TO RETRIEVE THE USER DATA AND HANDLE THE TREE

/*

    How to use:

    Create an instance of the IDMappedKDTree class
    Provide the dimensionality (number of dimensions) of the tree and UserData type
    
        IDMappedKDTree<double, 3, PlayerData> kdTree;

    Insert data into the KD-tree
    Each data point should be provided as a coordinate array and associated user data
    
        kdTree.insert({1.0, 2.0, 3.0}, PlayerData("John", 1001));
        kdTree.insert({4.0, 5.0, 6.0}, PlayerData("Alice", 1002));


*/

namespace VLIB{

// Base struct for user data
struct UserBaseStruct {
    virtual ~UserBaseStruct() = default;
};

// Node
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
    }

    // Constructor with unique ID generation
    KDNode(const std::array<CoordType, KDimensions>& p)
        : point(p), uniqueID(generateUniqueID()), left(nullptr), right(nullptr) {}
};

// KD Tree that stores points in K-dimensional space
template <class CoordType, std::size_t KDimensions, class DerivedUserData>
class IDMappedKDTree {
private:
    KDNode<CoordType, KDimensions>* root;
    std::unordered_map<std::uint64_t, std::pair<std::shared_ptr<DerivedUserData>, std::array<CoordType, KDimensions>>> dataMap;

    // Helper functions

        // map helper functions
        const std::uint64_t insertIntoMap(KDNode<CoordType, KDimensions>* node, const DerivedUserData& userData);

        // tree helper functions
        std::size_t getTreeDepth() const;
        std::size_t calculateTreeDepth(KDNode<CoordType, KDimensions>* currentNode) const;
        void updateKdCoords(const std::array<CoordType, KDimensions>& oldCoords, const std::array<CoordType, KDimensions>& newCoords);

        KDNode<CoordType, KDimensions>* findNode(const std::array<CoordType, KDimensions>& point, uint64_t uniqueID) const;
        void deleteFromKDTree(const std::array<CoordType, KDimensions>& point, std::uint64_t uniqueID);
        void deleteNode(KDNode<CoordType, KDimensions>* node, int i);
        KDNode<CoordType, KDimensions>* smallest(KDNode<CoordType, KDimensions>* node, int i, int j);

        KDNode<CoordType, KDimensions>* insertIntoTree(const std::array<CoordType, KDimensions>& point);
        void clearTree();
        void clearRecursive(KDNode<CoordType, KDimensions>* node);  

public:
    // constructor & destructor
    IDMappedKDTree() : root(nullptr) {}
    ~IDMappedKDTree() {
        clear();
        delete root; 
    }

    // Manage the tree
    std::uint64_t insert(const std::array<CoordType, KDimensions>& point, const DerivedUserData& userData);
    void remove(std::uint64_t uniqueID);
    void clear();
    std::size_t size() const;
    std::uint64_t nearestNeighbor(const std::array<CoordType, KDimensions>& point) const;
    std::uint64_t nearestNeighborWithinRange(const std::array<CoordType, KDimensions>& point, CoordType maxDistance) const;
    void nearestNeighborWithinRangeRecursive(KDNode<CoordType, KDimensions>* node, const std::array<CoordType, KDimensions>& queryPoint,
        CoordType maxDistance, std::uint64_t& nearestNeighborID, CoordType& nearestDistance) const;
    std::vector<std::uint64_t>  rangeSearch(const std::array<CoordType, KDimensions>& point, CoordType distance) const;
    bool contains(std::uint64_t uniqueID) const;

    // Manage user-defined data associated with a point & KD-tree synchronization
    void setPointCoordinates(std::uint64_t uniqueID, const std::array<CoordType, KDimensions>& newCoordinates);
    void setUserData(std::uint64_t uniqueID, const UserBaseStruct& userData);
    std::shared_ptr<UserBaseStruct> getUserData(std::uint64_t uniqueID) const;
    const std::array<CoordType, KDimensions>& getCoordinates(std::uint64_t uniqueID) const;
};

// PRIVATE MAP HELPER FUNCTIONS

// Private helper function to insert into the data map
template <class CoordType, std::size_t KDimensions, class DerivedUserData>
const std::uint64_t IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::insertIntoMap(KDNode<CoordType, KDimensions>* node, const DerivedUserData& userData){
    // Get the unique ID from the node
    std::uint64_t uniqueID = node->uniqueID;

    // Create a shared pointer to the derived class UserData
    std::shared_ptr<DerivedUserData> userPtr = std::make_shared<DerivedUserData>(userData);

    // Update the data map with coordinates and user data
    dataMap[uniqueID] = std::make_pair(userPtr, node->point);

    return uniqueID;
}




// PRIVATE TREE HELPER FUNCTIONS

template <class CoordType, std::size_t KDimensions, class DerivedUserData>
std::size_t IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::getTreeDepth() const {
    // Call a private helper function to calculate the depth of the tree recursively
    return calculateTreeDepth(root);
}

template <class CoordType, std::size_t KDimensions,class DerivedUserData>
std::size_t IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::calculateTreeDepth(KDNode<CoordType, KDimensions>* currentNode) const {
    if (currentNode == nullptr) {
        return 0;
    }

    // Recursively calculate the depth of the left and right subtrees
    std::size_t leftDepth = calculateTreeDepth(currentNode->left);
    std::size_t rightDepth = calculateTreeDepth(currentNode->right);

    // Return the maximum depth between the left and right subtrees, plus 1 for the current node
    return 1 + std::max(leftDepth, rightDepth);
}

template <class CoordType, std::size_t KDimensions, class DerivedUserData>
void IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::updateKdCoords(
    const std::array<CoordType, KDimensions>& oldCoords,
    const std::array<CoordType, KDimensions>& newCoords) {

    // Find the node in the KD-tree based on the oldCoords
    KDNode<CoordType, KDimensions>* currentNode = root;
    KDNode<CoordType, KDimensions>* parentNode = nullptr;
    std::size_t depth = 0; // Current traversal depth

    while (currentNode != nullptr) {
        if (currentNode->point == oldCoords) {
            // Update the coordinates of the node
            currentNode->point = newCoords;

            // Perform balancing or structural changes if needed
            // (Balancing strategy may vary based on your requirements)

            // Exit the loop since we found and updated the node
            break;
        }

        // Determine the splitting axis for the next level of traversal
        std::size_t axis = depth % KDimensions;

        // Decide whether to go left or right based on the splitting axis
        if (oldCoords[axis] < currentNode->point[axis]) {
            parentNode = currentNode;
            currentNode = currentNode->left;
        } else {
            parentNode = currentNode;
            currentNode = currentNode->right;
        }

        // Move to the next dimension for the next level of traversal
        depth = (depth + 1) % KDimensions;
    }

    // If needed, perform balancing or structural changes here
    // based on the new structure after the update

    // Note: Balancing and structural changes may vary based on your balancing strategy
}



template <class CoordType, std::size_t KDimensions, class DerivedUserData>
KDNode<CoordType, KDimensions>* IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::findNode(const std::array<CoordType, KDimensions>& point, std::uint64_t uniqueID) const {
    KDNode<CoordType, KDimensions>* currentNode = root;

    // Calculate the traversal depth using the tree depth
    std::size_t depth = getTreeDepth();

    // Traverse the KD-tree to find the node with matching coordinates and unique ID
    while (currentNode != nullptr) {
        // Check if the current node matches both coordinates and unique ID
        if (currentNode->point == point && currentNode->uniqueID == uniqueID) {
            return currentNode; // Found the node
        }

        // Determine the splitting axis for the next level of traversal
        std::size_t axis = depth % KDimensions;

        // Decide whether to go left or right based on the splitting axis
        if (point[axis] < currentNode->point[axis]) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        // Move to the next dimension for the next level of traversal
        depth = (depth + 1) % KDimensions;
    }

    return nullptr; // Node not found
}



template <class CoordType, std::size_t KDimensions,class DerivedUserData>
void IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::deleteFromKDTree(const std::array<CoordType, KDimensions>& point, std::uint64_t uniqueID) {

    KDNode<CoordType, KDimensions>* p = findNode(point, uniqueID);
    deleteNode(p, 0); // Pass the appropriate dimension index
}

template <class CoordType, std::size_t KDimensions,class DerivedUserData>
void IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::deleteNode(KDNode<CoordType, KDimensions>* node, int i) {
    if (node->left == nullptr && node->right == nullptr) {
        // Node is a leaf, just delete it
        delete node;
    } else {
        KDNode<CoordType, KDimensions>* q;
        if (node->right != nullptr) {
            q = smallest(node->right, i, (i + 1) % KDimensions);
        } else {
            q = smallest(node->left, i, (i + 1) % KDimensions);
        }

        if (node->right != nullptr) {
            node->left = node->right;
        } else {
            node->left = nullptr;
        }

        node->point = q->point;
        deleteNode(q, i);
    }
}

template <class CoordType, std::size_t KDimensions, class DerivedUserData>
KDNode<CoordType, KDimensions>* IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::smallest(KDNode<CoordType, KDimensions>* node, int i, int j) {
    KDNode<CoordType, KDimensions>* qq = node;

    if (i == j) {
        if (node->left != nullptr) {
            qq = smallest(node->left, i, (j + 1) % KDimensions);
        } else {
            return node;
        }
    }

    if (node->left != nullptr) {
        KDNode<CoordType, KDimensions>* lt = smallest(node->left, i, (j + 1) % KDimensions);
        if (qq->point[i] >= lt->point[i]) {
            qq = lt;
        }
    }

    if (node->right != nullptr) {
        KDNode<CoordType, KDimensions>* rt = smallest(node->right, i, (j + 1) % KDimensions);
        if (qq->point[i] >= rt->point[i]) {
            qq = rt;
        }
    }

    return qq;
}


// Private helper function to insert into the KD-tree
template <class CoordType, std::size_t KDimensions, class DerivedUserData>
KDNode<CoordType, KDimensions>* IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::insertIntoTree(const std::array<CoordType, KDimensions>& point) {
    int i = 0;
    KDNode<CoordType, KDimensions>* p = root;
    KDNode<CoordType, KDimensions>* prev = nullptr;
    
    // Traverse the tree to find the appropriate insertion point
    while (p != nullptr) {
        prev = p;
        if (point[i] < p->point[i])
            p = p->left;
        else
            p = p->right;
        i = (i + 1) % KDimensions; // Move to the next dimension
    }

    // create a new node
    KDNode<CoordType, KDimensions>* newNode = new KDNode<CoordType, KDimensions>(std::array<CoordType, KDimensions>(point));


    if (root == nullptr)
        root = newNode;
    else if (point[(i - 1 + KDimensions) % KDimensions] < prev->point[(i - 1 + KDimensions) % KDimensions])
        prev->left = newNode;
    else
        prev->right = newNode;


    return newNode;
}


// Private helper function to clear the KD-tree
template <class CoordType, std::size_t KDimensions,class DerivedUserData>
void IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::clearTree() {
    // Call a private recursive function to clear the tree starting from the root
    clearRecursive(root);

    // Set the root pointer to nullptr
    root = nullptr;
}

template <class CoordType, std::size_t KDimensions,class DerivedUserData>
void IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::clearRecursive(KDNode<CoordType, KDimensions>* node) {
    if (node) {
        // Recursively clear the left subtree
        clearRecursive(node->left);
        // Recursively clear the right subtree
        clearRecursive(node->right);
        // Delete the current node
        delete node;
    }
}


/*
    End of private helper functions
*/

// Manage the tree
template <class CoordType, std::size_t KDimensions, class DerivedUserData>
std::uint64_t IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::insert(
    const std::array<CoordType, KDimensions>& point, const DerivedUserData& userData) {

    // call tree insert helper and make it return ID, set this ID to a var
    KDNode<CoordType, KDimensions>* node = insertIntoTree(point);
    // call map insert helper with (cords, ID, userData)
    std::uint64_t uniqueID = insertIntoMap(node, userData);
    // return ID
    return uniqueID;
}


template <class CoordType, std::size_t KDimensions,class DerivedUserData>
void IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::remove(std::uint64_t uniqueID) {
    auto it = dataMap.find(uniqueID);
    if (it != dataMap.end()) {
        // Get the coordinates from the tuple
        const std::array<CoordType, KDimensions>& coordinates = std::get<1>(it->second);

        // Call the private function to delete the node from the KD-tree
        deleteFromKDTree(coordinates, uniqueID);

        // Remove the entry from the tuple map
        dataMap.erase(it);
    }
}

// Public method to clear the KD-tree
template <class CoordType, std::size_t KDimensions,class DerivedUserData>
void IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::clear() {
    // Clear the KD-tree
    clearTree();
    // Clear the data map
    dataMap.clear();
}


template <class CoordType, std::size_t KDimensions,class DerivedUserData>
std::size_t IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::size() const {
    // Return the size of the dataMap, which is equivalent to the number of points in the KD-tree
    return dataMap.size();
}

template <class CoordType, std::size_t KDimensions,class DerivedUserData>
std::uint64_t IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::nearestNeighbor(
    const std::array<CoordType, KDimensions>& point)const  {

    // Initialize the best candidate and its distance
    KDNode<CoordType, KDimensions>* bestCandidate = nullptr;
    CoordType bestDistance = std::numeric_limits<CoordType>::max();

    // Helper function to calculate squared Euclidean distance between two points
    auto calculateSquaredDistance = [](const std::array<CoordType, KDimensions>& p1,
                                        const std::array<CoordType, KDimensions>& p2) -> CoordType {
        CoordType squaredDistance = 0;
        for (std::size_t i = 0; i < KDimensions; ++i) {
            CoordType diff = p1[i] - p2[i];
            squaredDistance += diff * diff;
        }
        return squaredDistance;
    };

    // Recursive helper function to traverse the KD-tree and find the nearest neighbor
    std::function<void(KDNode<CoordType, KDimensions>*, std::size_t)> traverse;
    traverse = [&](KDNode<CoordType, KDimensions>* node, std::size_t depth) {
        if (node == nullptr) {
            return;
        }

        // Calculate the distance between the point and the current node's coordinates
        CoordType distance = calculateSquaredDistance(point, node->point);

        // Update the best candidate if the current node is closer
        if (distance < bestDistance) {
            bestCandidate = node;
            bestDistance = distance;
        }

        // Determine the splitting axis for the current node
        std::size_t axis = depth % KDimensions;

        // Choose the appropriate child node to visit first based on the splitting axis
        KDNode<CoordType, KDimensions>* firstChild = (point[axis] < node->point[axis]) ? node->left : node->right;
        KDNode<CoordType, KDimensions>* secondChild = (point[axis] < node->point[axis]) ? node->right : node->left;

        // Recursively visit the child that is on the same side as the query point
        traverse(firstChild, depth + 1);

        // Check if we need to visit the other side based on the distance to the splitting plane
        if ((node->point[axis] - point[axis]) * (node->point[axis] - point[axis]) < bestDistance) {
            traverse(secondChild, depth + 1);
        }
    };

    // Start traversing the KD-tree from the root
    traverse(root, 0);

    // Return the unique ID of the nearest point
    return bestCandidate->uniqueID;


}

template <class CoordType, std::size_t KDimensions,class DerivedUserData>
std::uint64_t IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::nearestNeighborWithinRange(
    const std::array<CoordType, KDimensions>& queryPoint, CoordType maxDistance) const
{
    if (!root) {
        // Handle the case when the tree is empty
        // For example, you can throw an exception or return a sentinel value.
        return 0; // Replace with an appropriate value or logic.
    }

    // Helper function to calculate the squared distance between two points
    auto squaredDistance = [](const std::array<CoordType, KDimensions>& p1,
                              const std::array<CoordType, KDimensions>& p2) {
        CoordType distance = 0;
        for (std::size_t i = 0; i < KDimensions; ++i) {
            CoordType diff = p1[i] - p2[i];
            distance += diff * diff;
        }
        return distance;
    };

    // Initialize the nearest neighbor ID and distance
    std::uint64_t nearestNeighborID = 0;
    CoordType nearestDistance = std::numeric_limits<CoordType>::max();

    // Start recursive traversal to find nearest neighbor within range
    nearestNeighborWithinRangeRecursive(root, queryPoint, maxDistance, nearestNeighborID, nearestDistance);

    return nearestNeighborID;
}

template <class CoordType, std::size_t KDimensions,class DerivedUserData>
void IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::nearestNeighborWithinRangeRecursive(
    KDNode<CoordType, KDimensions>* node, const std::array<CoordType, KDimensions>& queryPoint,
    CoordType maxDistance, std::uint64_t& nearestNeighborID, CoordType& nearestDistance) const
{
    if (!node)
        return;

    // Calculate the squared distance from the query point to the node's point
    CoordType distance = 0;
    for (std::size_t i = 0; i < KDimensions; ++i) {
        CoordType diff = node->point[i] - queryPoint[i];
        distance += diff * diff;
    }

    // Update the nearest neighbor if the current node is closer and within range
    if (distance < nearestDistance && distance <= maxDistance * maxDistance) {
        nearestNeighborID = node->uniqueID;
        nearestDistance = distance;
    }

    // Decide which subtree to visit first based on splitting dimension
    std::size_t splitDim = getTreeDepth() % KDimensions;
    if (queryPoint[splitDim] < node->point[splitDim]) {
        nearestNeighborWithinRangeRecursive(node->left, queryPoint, maxDistance, nearestNeighborID, nearestDistance);
        if (node->point[splitDim] - queryPoint[splitDim] <= maxDistance)
            nearestNeighborWithinRangeRecursive(node->right, queryPoint, maxDistance, nearestNeighborID, nearestDistance);
    } else {
        nearestNeighborWithinRangeRecursive(node->right, queryPoint, maxDistance, nearestNeighborID, nearestDistance);
        if (queryPoint[splitDim] - node->point[splitDim] <= maxDistance)
            nearestNeighborWithinRangeRecursive(node->left, queryPoint, maxDistance, nearestNeighborID, nearestDistance);
    }
}



template <class CoordType, std::size_t KDimensions,class DerivedUserData>
std::vector<std::uint64_t> IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::rangeSearch(
    const std::array<CoordType, KDimensions>& point, CoordType distance) const {

    // Initialize a vector to store unique IDs within the range
    std::vector<std::uint64_t> result;

    // Helper function to calculate squared Euclidean distance between two points
    auto calculateSquaredDistance = [](const std::array<CoordType, KDimensions>& p1,
                                        const std::array<CoordType, KDimensions>& p2) -> CoordType {
        CoordType squaredDistance = 0;
        for (std::size_t i = 0; i < KDimensions; ++i) {
            CoordType diff = p1[i] - p2[i];
            squaredDistance += diff * diff;
        }
        return squaredDistance;
    };

    // Recursive helper function to traverse the KD-tree and find points within the range
    std::function<void(KDNode<CoordType, KDimensions>*)> traverse;
    traverse = [&](KDNode<CoordType, KDimensions>* node) {
        if (node == nullptr) {
            return;
        }

        // Calculate the squared distance between the point and the current node's coordinates
        CoordType squaredDistance = calculateSquaredDistance(point, node->point);

        // If the squared distance is within the specified range, add the unique ID to the result
        if (squaredDistance <= distance * distance) {
            result.emplace_back(node->uniqueID);
        }

        // Determine the splitting axis for the current node using getTreeDepth()
        std::size_t axis = getTreeDepth() % KDimensions;

        // Check if we need to visit the left or right child based on the splitting axis
        if (point[axis] < node->point[axis]) {
            traverse(node->left);
            if ((node->point[axis] - point[axis]) * (node->point[axis] - point[axis]) < distance * distance) {
                traverse(node->right);
            }
        } else {
            traverse(node->right);
            if ((node->point[axis] - point[axis]) * (node->point[axis] - point[axis]) < distance * distance) {
                traverse(node->left);
            }
        }
    };

    // Start traversing the KD-tree from the root
    traverse(root);

    // Return the vector of unique IDs within the specified range
    return result;
}



template <class CoordType, std::size_t KDimensions,class DerivedUserData>
bool IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::contains(std::uint64_t uniqueID) const {
    // Check if the uniqueID exists in the dataMap
    return dataMap.find(uniqueID) != dataMap.end();
}


// Manage user-defined data associated with a point & KD-tree synchronization
template <class CoordType, std::size_t KDimensions,class DerivedUserData>
void IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::setPointCoordinates(std::uint64_t uniqueID, const std::array<CoordType, KDimensions>& newCoordinates) {
    auto it = dataMap.find(uniqueID);
    if (it != dataMap.end()) {
        //get old cords
        std::array<CoordType, KDimensions> oldCords = std::get<1>(it->second);
        //update cords
        std::get<1>(it->second) = newCoordinates; // Update coordinates in the tuple
        // Optionally, update the KD-tree internal structure if needed
        updateKdCoords(oldCords, newCoordinates);
    }
}

template <class CoordType, std::size_t KDimensions,class DerivedUserData>
void IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::setUserData(std::uint64_t uniqueID, const UserBaseStruct& userData) {
    auto it = dataMap.find(uniqueID);
    if (it != dataMap.end()) {
        std::get<0>(it->second) = userData; // Update user data in the tuple
    }
}


// Retrieve user data using unique ID
template <class CoordType, std::size_t KDimensions, class DerivedUserData>
std::shared_ptr<UserBaseStruct> IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::getUserData(std::uint64_t uniqueID) const {
    auto it = dataMap.find(uniqueID);
    if (it != dataMap.end()) {
        return it->second.first;
    }
    return nullptr; // Return nullptr if the uniqueID is not found in the map
}



// Retrieve coordinates using unique ID
template <class CoordType, std::size_t KDimensions,class DerivedUserData>
const std::array<CoordType, KDimensions>& IDMappedKDTree<CoordType, KDimensions, DerivedUserData>::getCoordinates(std::uint64_t uniqueID) const {
    return std::get<1>(dataMap.at(uniqueID));
}



}

#endif // IDMappedKDTree_H
