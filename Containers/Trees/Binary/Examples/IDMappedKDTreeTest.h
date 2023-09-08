#ifndef __IDMAPKDTREETEST_H__
#define __IDMAPKDTREETEST_H__

#include "../IDMappedKDTree.h"
#include <iostream>
#include <vector>

// Define the PlayerData structure
struct PlayerData : public VLIB::UserBaseStruct {
    std::string name;
    std::uint64_t id;

    PlayerData(const std::string& n, std::uint64_t i) : name(n), id(i) {}
};

class exampleKDTree {
public:
    // Function to run the KD-tree test
    static void runKDTreeTest() {
        // Create an instance of the IDMappedKDTree class
        VLIB::IDMappedKDTree<double, 3, PlayerData> kdTree;

        // Insert data into the KD-tree
        kdTree.insert({1.0, 2.0, 3.0}, PlayerData("John", 1001));
        kdTree.insert({4.0, 5.0, 6.0}, PlayerData("Alice", 1002));

        // Perform queries on the KD-tree
        std::array<double, 3> queryPoint = {2.0, 4.0, 5.0};

        // Nearest neighbor query
        std::uint64_t nearestNeighborID = kdTree.nearestNeighbor(queryPoint);
        const std::shared_ptr<VLIB::UserBaseStruct>& nearestNeighborUserDataPtr = kdTree.getUserData(nearestNeighborID);

        if (nearestNeighborUserDataPtr) {
            const std::shared_ptr<PlayerData>& nearestNeighborData = std::dynamic_pointer_cast<PlayerData>(nearestNeighborUserDataPtr);
            if (nearestNeighborData) {
                std::cout << "Nearest neighbor: " << nearestNeighborData->name << ", ID: " << nearestNeighborID << std::endl;
            } else {
                std::cout << "Nearest neighbor data is not of type PlayerData" << std::endl;
            }
        } else {
            std::cout << "Nearest neighbor data not found" << std::endl;
        }

        // Nearest neighbors within range query
        double maxDistance = 8.0;
        std::uint64_t nearestNeighborInRangeID = kdTree.nearestNeighborWithinRange(queryPoint, maxDistance);
        const std::shared_ptr<VLIB::UserBaseStruct>& nearestNeighborInRangeUserDataPtr = kdTree.getUserData(nearestNeighborInRangeID);
        if (nearestNeighborInRangeUserDataPtr) {
            const std::shared_ptr<PlayerData>& nearestNeighborInRangeData = std::dynamic_pointer_cast<PlayerData>(nearestNeighborInRangeUserDataPtr);
            if (nearestNeighborInRangeData) {
                std::cout << "Nearest neighbor within range: " << nearestNeighborInRangeData->name << ", ID: " << nearestNeighborInRangeID << std::endl;
            } else {
                std::cout << "Nearest neighbor within range data is not of type PlayerData" << std::endl;
            }
        } else {
            std::cout << "Nearest neighbor within range data not found" << std::endl;
        }

        // Range search query
        double searchDistance = 3.0;
        std::vector<std::uint64_t> pointsInRangeIDs = kdTree.rangeSearch(queryPoint, searchDistance);
        std::cout << "Points within range:" << std::endl;
        for (std::uint64_t id : pointsInRangeIDs) {
            const std::shared_ptr<VLIB::UserBaseStruct>& userDataPtr = kdTree.getUserData(id);
            if (userDataPtr) {
                const std::shared_ptr<PlayerData>& playerData = std::dynamic_pointer_cast<PlayerData>(userDataPtr);
                if (playerData) {
                    std::cout << "Player: " << playerData->name << ", ID: " << id << std::endl;
                }
            }
        }
    }
};


#endif // __IDMAPKDTREETEST_H__