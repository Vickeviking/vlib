#ifndef __BPLUS_TREE_H__
#define __BPLUS_TREE_H__

//Here template is used as T(dataType),N(Max Keys in Internal Node),M(Max Keys in External Node) keys are soley used as
// routing keys in internal nodes meanwhile keys in external ones are used as data keys.

// The data is embedded in a common base class for internal and external nodes.

#include <iostream>
#include <stdexcept>


namespace VLIB{ // VLIB

// Forward declarations
template <typename T, int N, int M> class BPlusTreeNode;
template <typename T, int N, int M> class BPlusTreeExternalNode;
template <typename T, int N, int M> class BPlusTreeInternalNode;




//**** Base class for data elements ****//
// common base class for data elements
template <typename T, int N, int M>
class BPlusTreeData {
    // Common properties or methods for data elements
public:
    BPlusTreeData() = default; // Default constructor
    virtual ~BPlusTreeData() = default; // Default destructor
    
    virtual T GetDataValue() const = 0; // Pure virtual method to retrieve the data
    virtual BPlusTreeNode<T, N, M>* GetNodePointer() const = 0; // Pure virtual method to retrieve the node pointer
    virtual void setDataValue(T data) = 0;
    virtual void setNodePointer(BPlusTreeNode<T, N, M>* node) = 0;
};


// Derived classes for different types of data (e.g., actual data, child node pointers)
// External node data
template <typename T, int N, int M>
class BPlusTreeDataValue final : public BPlusTreeData<T, N, M> {
private:
    // Properties specific to data values
    T value; // Example property

public:
    BPlusTreeDataValue(T dataValue) : value(dataValue){}
    BPlusTreeDataValue() = default; // Default constructor
    ~BPlusTreeDataValue() = default; // Default destructor

    // pure virtual methods from the base class implementations
    //dont use these
    void setNodePointer(BPlusTreeNode<T, N, M>* node) override { throw std::runtime_error("External node data does not have a node pointer");}
    BPlusTreeNode<T, N, M>* GetNodePointer() const override { throw std::runtime_error("External node data does not have a node pointer");}

    // Retrieve the data value
    T GetDataValue() const override {return value;}
    // Set the data value
    void setDataValue(T data) override {value = data;}
};

// Internal node data
template <typename T, int N, int M>
class BPlusTreeDataNodePointer final : public BPlusTreeData<T, N, M> {
private:
    // Properties specific to node pointers
    BPlusTreeNode<T, N, M>* nodePointer; // Example property

public:
    BPlusTreeDataNodePointer(BPlusTreeNode<T, N, M>* pointer) : nodePointer(pointer) {}
    ~BPlusTreeDataNodePointer() = default; // Default destructor

    // Implement the pure virtual methods from the base class
    //dont use these
    T GetDataValue() const override { throw std::runtime_error("Internal node data does not have a data value");}
    void setDataValue(T data) override { throw std::runtime_error("Internal node data does not have a data value");}

    // Retrieve the node pointer
    BPlusTreeNode<T, N, M>* GetNodePointer() const override {return nodePointer;}
    // Set the node pointer
    void setNodePointer(BPlusTreeNode<T, N, M>* node) override {nodePointer = node;}
};

//**** B+ Tree nodes 1 base, 2 derived offsprings ****//

// Common base class for internal and external nodes
template <typename T, int N, int M>
class BPlusTreeNode {
public:
    BPlusTreeNode() {
        // Constructor logic here (if needed)
    }
    virtual ~BPlusTreeNode() = default; // Default destructor

    enum class NodeType { Internal, External };

    // Basic operations
    virtual NodeType GetNodeType() const = 0;
    // 3 variants, Insert(Base) is entry point and route to the right one with casting
    virtual bool Insert(BPlusTreeData<T, N, M>* data);
    virtual bool InsertInternal(BPlusTreeDataNodePointer<T, N, M>* data) = 0;
    virtual bool InsertExternal(BPlusTreeDataValue<T, N, M>* data) = 0;
    // 3 variants, Delete(Base) is entry point and route to the right one with casting
    virtual bool Delete(BPlusTreeData<T, N, M>* data);
    virtual bool DeleteInternal(BPlusTreeDataNodePointer<T, N, M>* data) = 0;
    virtual bool DeleteExternal(BPlusTreeDataValue<T, N, M>* data) = 0;
    // 3 variants, Search(Base) is entry point and route to the right one with casting, cast return type to the right one
    virtual BPlusTreeData<T, N, M>* Search(BPlusTreeData<T, N, M>* data);
    virtual BPlusTreeDataNodePointer<T, N, M>* SearchInternal(BPlusTreeDataNodePointer<T, N, M>* data) = 0;
    virtual BPlusTreeDataValue<T, N, M>* SearchExternal(BPlusTreeDataValue<T, N, M>* data) = 0;
    // While no return type and no params we can soley use pure virtual and overide respectively
    virtual BPlusTreeNode<T, N, M>* Split();
    virtual BPlusTreeExternalNode<T, N, M>* SplitExternal() = 0;
    virtual BPlusTreeInternalNode<T, N, M>* SplitInternal() = 0;

    virtual int ChildCount() = 0;
    virtual BPlusTreeNode<T, N, M>* GetChild(int index) = 0;

    // While no return type needed(no new allocated memory = no handles created) and no params(parent ptrs) we can soley use pure virtual and overide respectively
    virtual bool Merge() = 0;
    // While no return type needed(no new allocated memory = no handles created) and no params we can soley use pure virtual and overide respectively
    virtual void Print() = 0;
    virtual bool isFull() = 0;
    virtual BPlusTreeExternalNode<T, N, M>* FindLeafNode(T data);
    virtual int FindChildIndex(BPlusTreeNode<T, N, M>* child) = 0;

    //getters and setters
    virtual BPlusTreeNode<T, N, M>* GetParent() = 0;
    virtual int GetNumValues() = 0;
    bool IsRoot();

};

// Derived internal node class
template <typename T, int N, int M>
class BPlusTreeInternalNode final : public BPlusTreeNode<T, N, M> {
private:

    // Define maximum and minimum children to merge
    const int MAXIMUM_CHILDREN_TO_MERGE = N - 1;
    const int MINIMUM_CHILDREN_TO_MERGE = N / 2;


    T keys[N - 1]; // One less key than children
    BPlusTreeNode<T, N, M>* children[N];
    BPlusTreeNode<T, N, M>* parent; // Array to store data values
public:

    BPlusTreeInternalNode();
    virtual ~BPlusTreeInternalNode(); // Default destructor

    //must be implemented but not used, IGNORE THEM
    bool DeleteExternal(BPlusTreeDataValue<T, N, M>* data) override{ return false;};
    bool InsertExternal(BPlusTreeDataValue<T, N, M>* data) override{ return false;};
    BPlusTreeDataValue<T, N, M>* SearchExternal(BPlusTreeDataValue<T, N, M>* data) override{ return nullptr;};
    BPlusTreeExternalNode<T, N, M>* SplitExternal() override { return nullptr;};



    typename BPlusTreeNode<T, N, M>::NodeType GetNodeType() const override {
    return BPlusTreeNode<T, N, M>::NodeType::Internal;
    }

    //Basic Operations
    bool InsertInternal(BPlusTreeDataNodePointer<T, N, M>* data) override;
    bool DeleteInternal(BPlusTreeDataNodePointer<T, N, M>* data) override;
    BPlusTreeDataNodePointer<T, N, M>* SearchInternal(BPlusTreeDataNodePointer<T, N, M>* data) override;
    BPlusTreeInternalNode<T, N, M>* SplitInternal() override; // Splitting logic not same as external node
    bool Merge() override; // Merging logic not same as external node
    void MergeWithParent(BPlusTreeInternalNode<T, N, M>* parent, int indexInParent);
    void Print() override; // Print logic not same as external node
    bool isFull() override;
    int FindChildIndex(BPlusTreeNode<T, N, M>* child) override;
    int ChildCount() override;
    void RemoveChild(BPlusTreeNode<T, N, M>* child);
    bool InsertKeyAndChild(T key, BPlusTreeNode<T, N, M>* child);
    void RemoveKeyAndChild(int index);
    

    //getters and setters
    BPlusTreeNode<T, N, M>* GetChild(int index) override {return children[index];};
    void SetChild(int index, BPlusTreeNode<T, N, M>* child){children[index] = child;};
    T GetKey(int index) const {return keys[index];};
    void SetKey(int index, T key){keys[index] = key;};
    int GetKeyCount();
    BPlusTreeNode<T, N, M>* GetParent() override {return parent;};
    int GetNumValues() override;

};

// Derived external (leaf) node class
template <typename T, int N, int M>
class BPlusTreeExternalNode final : public BPlusTreeNode<T, N, M> {
private:
    // Define maximum and minimum children to merge
    const int MAXIMUM_CHILDREN_TO_MERGE = M - 1;
    const int MINIMUM_CHILDREN_TO_MERGE = M / 2;

    BPlusTreeDataValue<T, N, M> values[M];  // Array to store data values
    BPlusTreeNode<T, N, M>* parent;  // Pointer to the parent node
public:

    BPlusTreeExternalNode();
    ~BPlusTreeExternalNode(); // Default destructor

    // must be implemented but not used, IGNORE THEM
    bool DeleteInternal(BPlusTreeDataNodePointer<T, N, M>* data)override{ return false;};
    bool InsertInternal(BPlusTreeDataNodePointer<T, N, M>* data)override{ return false;};
    BPlusTreeDataNodePointer<T, N, M>* SearchInternal(BPlusTreeDataNodePointer<T, N, M>* data) override{ return nullptr;};
    int FindChildIndex(BPlusTreeNode<T, N, M>* child) override{ return -1;};
    BPlusTreeInternalNode<T, N, M>* SplitInternal()override{return nullptr;};
    int ChildCount() override{};
    BPlusTreeNode<T, N, M>* GetChild(int index) override{ return nullptr;};
 
    typename BPlusTreeNode<T, N, M>::NodeType GetNodeType() const override {
        return BPlusTreeNode<T, N, M>::NodeType::External;
    }

    //Basic Operations
    bool InsertExternal(BPlusTreeDataValue<T, N, M>* data) override;
    bool DeleteExternal(BPlusTreeDataValue<T, N, M>* data) override;
    BPlusTreeDataValue<T, N, M>* SearchExternal(BPlusTreeDataValue<T, N, M>* data) override;
    BPlusTreeExternalNode<T, N, M>* SplitExternal() override; // Splitting logic not same as internal node
    bool Merge() override; // Merging logic not same as internal node
    void Print() override; // Print logic not same as internal node
    bool isFull() override;
    bool Contains(T data);

    //getters and setters

    BPlusTreeNode<T, N, M>* GetParent()override{return parent;};
    void SetParent(BPlusTreeNode<T, N, M>* parent){this->parent = parent;};
    int GetNumValues() override;
    BPlusTreeDataValue<T, N, M>* GetValues(){return values;};
    void SetValues(BPlusTreeDataValue<T, N, M>* values){this->values = values;};
    T GetMaxValue();

};

//**** B+ Tree ****//
template <typename T, int N, int M> 
class BPlusTree {
private: // B+ Tree attributes
    BPlusTreeNode<T, N, M>* root; // Pointer to the root node

    bool _Insert(T data);
    bool _Delete(T data);
    bool _Search(T data);
    void _Print(BPlusTreeNode<T, N, M>* node); // Helper method to print the tree from a given node
    void _InOrderTraversal(BPlusTreeNode<T, N, M>* node, int depth);
    void _Clear(BPlusTreeNode<T, N, M>* node); // Helper method to clear the tree from a given node
    BPlusTreeExternalNode<T, N, M>* _FindAssociatedLeafNode(BPlusTreeNode<T, N, M>* node, T data);


public: // B+ Tree constructor & destructor
    BPlusTree() : root(nullptr) {};
    ~BPlusTree() {_Clear(root);root = nullptr;};

    // Public B+ Tree operations
    bool Insert(T data); //the user will want to insert a data of type T and not a NodePtr, that is an ofspring of our structure and something we internaly create
    bool Delete(T data);
    bool Search(T data){return _Search(data);};
    void Print(BPlusTreeNode<T, N, M>* node){_Print(node);};
    void Print(){_Print(root);};
    void Clear(){_Clear(root); root = nullptr;};
    void UpdateKeys(BPlusTreeNode<T, N, M>* node);
    T CalculateNewKey(BPlusTreeNode<T, N, M>* node);

    // Getters and setters
    bool IsRoot(BPlusTreeNode<T, N, M>* node){return node == root;};
    BPlusTreeNode<T, N, M>* GetRoot(){return root;};

};

    //** B+ Base Tree node (Common Base for external and internal nodes:) ) **//
template <typename T, int N, int M>
bool BPlusTreeNode<T, N, M>::Delete(BPlusTreeData<T, N, M>* data) {
    // Attempt to cast the data to BPlusTreeDataValue (external node data)
    BPlusTreeDataValue<T, N, M>* dataValue = dynamic_cast<BPlusTreeDataValue<T, N, M>*>(data);

    if (dataValue) {
        // Handle deletion logic for external (leaf) nodes
        return DeleteExternal(dataValue);
    }

    // Attempt to cast the data to BPlusTreeDataNodePointer (internal node data)
    BPlusTreeDataNodePointer<T, N, M>* dataNodePointer = dynamic_cast<BPlusTreeDataNodePointer<T, N, M>*>(data);

    if (dataNodePointer) {
        // Handle deletion logic for internal nodes
        return DeleteInternal(dataNodePointer);
    }

    // Handle unsupported data types or errors
    return false;
}


template <typename T, int N, int M>
bool BPlusTreeNode<T, N, M>::Insert(BPlusTreeData<T, N, M>* data) {
    // Attempt to cast the data to BPlusTreeDataValue (external node data)
    BPlusTreeDataValue<T, N, M>* dataValue = dynamic_cast<BPlusTreeDataValue<T, N, M>*>(data);

    if (dataValue) {

        return InsertExternal(dataValue);
    }

    // Attempt to cast the data to BPlusTreeDataNodePointer (internal node data)
    BPlusTreeDataNodePointer<T, N, M>* dataNodePointer = dynamic_cast<BPlusTreeDataNodePointer<T, N, M>*>(data);

    if (dataNodePointer) {
        // Handle insertion logic for internal nodes
        return InsertInternal(dataNodePointer);
    }

    // Handle unsupported data types or errors
    return false;
}




    template <typename T, int N, int M>
    BPlusTreeData<T, N, M>* BPlusTreeNode<T, N, M>::Search(BPlusTreeData<T, N, M>* data) {
        // Attempt to cast the data to BPlusTreeDataValue (external node data)
        BPlusTreeDataValue<T, N, M>* dataValue = dynamic_cast<BPlusTreeDataValue<T, N, M>*>(data);
        
        if (dataValue) {
            // Handle search logic for external (leaf) nodes
            return SearchExternal(dataValue);
        }
        
        // Attempt to cast the data to BPlusTreeDataNodePointer (internal node data)
        BPlusTreeDataNodePointer<T, N, M>* dataNodePointer = dynamic_cast<BPlusTreeDataNodePointer<T, N, M>*>(data);
        
        if (dataNodePointer) {
            // Handle search logic for internal nodes
            return SearchInternal(dataNodePointer);
        }
        
        // Handle unsupported data types or errors
        return nullptr;
    }

    template <typename T, int N, int M>
    BPlusTreeNode<T, N, M>* BPlusTreeNode<T, N, M>::Split() 
    {
        //route to right split
        if(GetNodeType() == BPlusTreeNode<T, N, M>::NodeType::Internal)
            return SplitInternal();
        else
            return SplitExternal();
    }


 template <typename T, int N, int M>
    BPlusTreeExternalNode<T, N, M>* BPlusTreeNode<T, N, M>::FindLeafNode(T data)
    {
        // Check if the current node is a leaf node
        if (GetNodeType() == BPlusTreeNode<T, N, M>::NodeType::External)
        {
            return dynamic_cast<BPlusTreeExternalNode<T, N, M>*>(this);
        }

        // Cast to internal node
        BPlusTreeInternalNode<T, N, M>* internalNode = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(this);

        // Find the correct child node to search
        int searchIndex = 0;

        while (searchIndex < internalNode->ChildCount() && data >= internalNode->GetKey(searchIndex))
        {
            searchIndex++;
        }

        // Check if the child pointer was found
        if (searchIndex < internalNode->ChildCount())
        {
            // Check if the child is an external node
            BPlusTreeExternalNode<T, N, M>* leafNode = internalNode->GetChild(searchIndex)->FindLeafNode(data);
            
            if (leafNode)
            {
                return leafNode; // Found the leaf node
            }
            else
            {
                // Continue searching in the next internal node
                return internalNode->GetChild(searchIndex)->FindLeafNode(data);
            }
        }

        return nullptr; // Child pointer not found
    }

    template <typename T, int N, int M>
    bool BPlusTreeNode<T, N, M>::IsRoot()
    {
        if(GetParent() == nullptr)
            return true;
        else
            return false;
    }


//** Derived internal node class **//

    template <typename T, int N, int M>
    BPlusTreeInternalNode<T, N, M>::BPlusTreeInternalNode()
    {
        parent = nullptr;
        for (int i = 0; i < N; i++)
        {
            children[i] = nullptr;
        }
        for (int i = 0; i < N - 1; i++)
        {
            keys[i] = 0;
        }
    }

    template <typename T, int N, int M>
    BPlusTreeInternalNode<T, N, M>::~BPlusTreeInternalNode()
    {
        parent = nullptr;
        for (int i = 0; i < N; i++)
        {
            delete children[i];
            children[i] = nullptr;
        }
        for (int i = 0; i < N - 1; i++)
        {
            keys[i] = 0;
        }
    }

    //Basic Operations
    template <typename T, int N, int M>
    bool BPlusTreeInternalNode<T, N, M>::InsertInternal(BPlusTreeDataNodePointer<T, N, M>* data) {
        // Check if the node is full (should not happen if called correctly)
        if (isFull())
            throw std::runtime_error("Internal node is full, Insertion to InternalNode used incorrectly please move/split beforehand");

        // Find the correct position to insert based on keys
        int insertIndex = 0;
        while (insertIndex < N && children[insertIndex] != nullptr && GetKey(insertIndex) < data->GetDataValue()) {
            insertIndex++;
        }

        // Shift elements to make space for the new one
        for (int i = N - 1; i > insertIndex; i--) {
            children[i] = children[i - 1];
            SetKey(i, GetKey(i - 1)); // Shift keys along with children
        }

        // Insert the new element and its corresponding key
        children[insertIndex] = data->GetNodePointer();
        SetKey(insertIndex, data->GetDataValue());

        return true;
    }


    
    template <typename T, int N, int M>
    bool BPlusTreeInternalNode<T, N, M>::DeleteInternal(BPlusTreeDataNodePointer<T, N, M>* data)
    {
    //check for underflow and call exception if needed
    if (children[MINIMUM_CHILDREN_TO_MERGE+1] == nullptr)
        throw std::runtime_error("Internal node is underflow, not meeting minimum children, Deletion from InternalNode used incorrectly please move/merge beforehand");

    // Find the correct position to delete
    int deleteIndex = 0;

    // Go through each child and check if it is the one we want to delete
    for (auto child : children)
    {
        if (child == data->GetNodePointer())
        {
            break;
        }
        deleteIndex++;
    }

    // Check if the child pointer was found
    if (deleteIndex < N && children[deleteIndex] == data->GetNodePointer())
    {
        // Delete the element and set to null
        delete children[deleteIndex];
        children[deleteIndex] = nullptr;

        // Shift the remaining child pointers to fill the gap
        for (int i = deleteIndex; i < N - 1; i++)
        {
            children[i] = children[i + 1];
        }
        children[N - 1] = nullptr;

        return true; // Deletion successful
    }

    return false; // Child pointer not found
    }


    template <typename T, int N, int M>
    BPlusTreeDataNodePointer<T, N, M>* BPlusTreeInternalNode<T, N, M>::SearchInternal(BPlusTreeDataNodePointer<T, N, M>* data)
    {
    // Find the correct child node if it exists
    // Go through each child and check if it is the one we want to delete
    for (auto child : children) {
        if (child == data->GetNodePointer()) {
            return new BPlusTreeDataNodePointer<T, N, M>(child); // Create and return a new BPlusTreeDataNodePointer
        }
    }
    return nullptr; // Child pointer not found
    }



    template <typename T, int N, int M>
    BPlusTreeInternalNode<T, N, M>* BPlusTreeInternalNode<T, N, M>::SplitInternal()
    {
        // Create a new sibling internal node
        BPlusTreeInternalNode<T, N, M>* newSibling = new BPlusTreeInternalNode<T, N, M>();

        // Calculate the midpoint to split the keys and children
        int midpoint = N / 2;

        // Move the keys and corresponding children from the current node to the new sibling
        for (int i = midpoint; i < N; i++)
        {
            newSibling->children[i - midpoint] = children[i];
            children[i] = nullptr; // Clear the reference in the current node
        }

        // Update the parent reference for the new sibling
        newSibling->parent = parent;

        // Update the keys in the current node
        // Update the children in the current node
        for (int i = midpoint; i < N; i++)
        {
            // Move the child reference to the new sibling
            newSibling->children[i - midpoint] = children[i];
            children[i] = nullptr; // Clear the reference in the current node
        }


        // Update the parent's reference to the new sibling
        if (parent)
            parent->InsertInternal(new BPlusTreeDataNodePointer<T, N, M>(newSibling));

        return newSibling; // Return the new sibling node
    }


    template <typename T, int N, int M>
    bool BPlusTreeInternalNode<T, N, M>::Merge()
    {
        // Check if this node is underfull (has fewer keys than required)
        if (GetKeyCount() >= MINIMUM_CHILDREN_TO_MERGE)
        {
            return false; // Cannot merge
        }

        BPlusTreeInternalNode<T, N, M>* leftSibling = nullptr;
        BPlusTreeInternalNode<T, N, M>* rightSibling = nullptr;

        // Find left and right siblings if they exist
        BPlusTreeInternalNode<T, N, M>* parent = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(GetParent());

        if (parent)
        {
            int indexInParent = parent->FindChildIndex(this);

            if (indexInParent > 0)
            {
                leftSibling = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(parent->GetChild(indexInParent - 1));
            }

            if (indexInParent < parent->ChildCount() - 1)
            {
                rightSibling = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(parent->GetChild(indexInParent + 1));
            }
        }

        // Merge with the left sibling if possible
        if (leftSibling && leftSibling->GetKeyCount() > MINIMUM_CHILDREN_TO_MERGE)
        {
            // Redistribute keys from leftSibling
            int totalKeys = GetKeyCount() + leftSibling->GetKeyCount();
            int keysToMove = (totalKeys / 2) - GetKeyCount(); // Move half of the keys

            // Move keys from leftSibling to this node
            for (int i = 0; i < keysToMove; i++)
            {
                int leftSiblingIndex = leftSibling->GetKeyCount() - 1;
                T redistributedKey = leftSibling->GetKey(leftSiblingIndex);
                BPlusTreeNode<T, N, M>* redistributedChild = leftSibling->GetChild(leftSiblingIndex + 1);

                // Insert the redistributed key and child
                InsertKeyAndChild(redistributedKey, redistributedChild);

                // Remove the key and child from leftSibling
                leftSibling->RemoveKeyAndChild(leftSiblingIndex);
            }

            // Update parent's key
            if (parent)
            {
                parent->SetKey(parent->FindChildIndex(this), GetKey(0));
            }

            return true;
        }

        // Merge with the right sibling if possible
        if (rightSibling && rightSibling->GetKeyCount() > MINIMUM_CHILDREN_TO_MERGE)
        {
            // Redistribute keys from rightSibling
            int totalKeys = GetKeyCount() + rightSibling->GetKeyCount();
            int keysToMove = (totalKeys / 2) - GetKeyCount(); // Move half of the keys

            // Move keys from rightSibling to this node
            for (int i = 0; i < keysToMove; i++)
            {
                T redistributedKey = rightSibling->GetKey(0);
                BPlusTreeNode<T, N, M>* redistributedChild = rightSibling->GetChild(0);

                // Insert the redistributed key and child
                InsertKeyAndChild(redistributedKey, redistributedChild);

                // Remove the key and child from rightSibling
                rightSibling->RemoveKeyAndChild(0);
            }

            // Update parent's key
            if (parent)
            {
                parent->SetKey(parent->FindChildIndex(this), rightSibling->GetKey(0));
            }

            return true;
        }

        // If merging with siblings is not possible, consider merging with the parent
        if (parent)
        {
            // Check if merging with the parent is possible and beneficial
            if (parent->ChildCount() > MINIMUM_CHILDREN_TO_MERGE)
            {
                // Merge this node with the parent node
                int indexInParent = parent->FindChildIndex(this);

                // Merge this node with the parent
                MergeWithParent(parent, indexInParent);

                return true;
            }
        }

        return false; // Cannot merge
    }

    template <typename T, int N, int M>
    void BPlusTreeInternalNode<T, N, M>::MergeWithParent(BPlusTreeInternalNode<T, N, M>* parent, int indexInParent) {
        if (parent && indexInParent >= 0 && indexInParent < parent->GetKeyCount()) {
            // Get the key from the parent at the specified index
            T parentKey = parent->GetKey(indexInParent);

            // Remove the key at the specified index in the parent
            parent->RemoveKeyAndChild(indexInParent);

            // Insert the key from the parent into the current node
            InsertKeyAndChild(parentKey, nullptr);

            // Transfer children from the parent to the current node
            // Start transferring from the parent's child at indexInParent + 1
            BPlusTreeInternalNode<T, N, M>* parentChild = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(parent->GetChild(indexInParent + 1));
            for (int i = 0; i < parentChild->GetKeyCount(); i++) {
                InsertKeyAndChild(parentChild->GetKey(i), parentChild->GetChild(i));
            }

            // Remove the parentChild from the parent's children
            parent->RemoveChild(parentChild);

            // Free memory for the parentChild (optional)
            delete parentChild;
        }
    }



    template <typename T, int N, int M>
    void BPlusTreeInternalNode<T, N, M>::Print() // Print logic not same as external node
    {
        std::cout << "[";
        for (int i = 0; i < N; i++)
        {
            if (children[i] != nullptr)
            {
                children[i]->Print();
            }
            if (i < N - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << "]";
    }

    template <typename T, int N, int M>
    bool BPlusTreeInternalNode<T, N, M>::isFull()
    {
        for (int i = 0; i < N; i++)
        {
            if (children[i] == nullptr)
                return false;
        }
        return true;
    }

    template <typename T, int N, int M>
    int BPlusTreeInternalNode<T, N, M>::FindChildIndex(BPlusTreeNode<T, N, M>* child)
    {
        for (int i = 0; i < N; i++)
        {
            if (children[i] == child)
                return i;
        }
        return -1;
    }

    template <typename T, int N, int M>
    int BPlusTreeInternalNode<T, N, M>::ChildCount()
    {
        int count = 0;
        for (int i = 0; i < N; i++)
        {
            if (children[i] != nullptr)
                count++;
        }
        return count;
    }

    template <typename T, int N, int M>
    int BPlusTreeInternalNode<T, N, M>::GetNumValues()
    {
        int count = 0;
        for (int i = 0; i < N; i++)
        {
            if (children[i] != nullptr)
                count++;
        }
        return count;
    }

    template <typename T, int N, int M>
    int BPlusTreeInternalNode<T, N, M>::GetKeyCount()
    {
        int count = 0;
        for (int i = 0; i < N - 1; i++)
        {
            if (keys[i] != 0)
                count++;
        }
        return count;
    }

    template <typename T, int N, int M>
    void BPlusTreeInternalNode<T, N, M>::RemoveChild(BPlusTreeNode<T, N, M>* child)
    {
        for (int i = 0; i < N; i++)
        {
            if (children[i] == child)
            {
                children[i] = nullptr;
                break;
            }
        }
    }

    template <typename T, int N, int M>
    bool BPlusTreeInternalNode<T, N, M>::InsertKeyAndChild(T key, BPlusTreeNode<T, N, M>* child)
    {
        // Check if the node is full (should not happen if called correctly)
        if (isFull())
            throw std::runtime_error("Internal node is full, Insertion to InternalNode used incorrectly please move/split beforehand");

        // Find the correct position to insert based on keys
        int insertIndex = 0;
        while (insertIndex < N && children[insertIndex] != nullptr && GetKey(insertIndex) < key) {
            insertIndex++;
        }

        // Shift elements to make space for the new one
        for (int i = N - 1; i > insertIndex; i--) {
            children[i] = children[i - 1];
            SetKey(i, GetKey(i - 1)); // Shift keys along with children
        }

        // Insert the new element and its corresponding key
        children[insertIndex] = child;
        SetKey(insertIndex, key);

        return true;
    }

   template <typename T, int N, int M>
    void BPlusTreeInternalNode<T, N, M>::RemoveKeyAndChild(int indexInParent) {
        if (indexInParent >= 0 && indexInParent < GetKeyCount()) {
            // Shift keys and children to remove the key at indexInParent
            for (int i = indexInParent; i < GetKeyCount() - 1; i++) {
                keys[i] = keys[i + 1];
                children[i + 1] = children[i + 2];
            }
        }
    }


//** Derived external (leaf) node class **//

    

    template <typename T, int N, int M>
    BPlusTreeExternalNode<T, N, M>::BPlusTreeExternalNode() : parent(nullptr) {
        for (int i = 0; i < M; i++) {
            // Initialize each element of the 'values' array here
            values[i] = BPlusTreeDataValue<T, N, M>(0); // Use the default constructor of BPlusTreeDataValue
        }
    }




    template <typename T, int N, int M>
    BPlusTreeExternalNode<T, N, M>::~BPlusTreeExternalNode() // Default destructor
    {
        parent = nullptr;
        for (int i = 0; i < M; i++)
        {
            values[i] = 0;
        }
    }

    //Basic Operations
    template <typename T, int N, int M>
    bool BPlusTreeExternalNode<T, N, M>::InsertExternal(BPlusTreeDataValue<T, N, M>* data)
    {
        // Check if the node is full (should not happen if called correctly)
        if (isFull())
            throw std::runtime_error("External node is full, Insertion to ExternalNode used incorrectly please move/split beforehand");

        // Find the correct position to insert
        int insertIndex = 0;
        while (insertIndex < M && values[insertIndex].GetDataValue() != 0 && values[insertIndex].GetDataValue() < data->GetDataValue())
        {
            insertIndex++;
        }

        // Shift elements to make space for the new ones
        for (int i = M - 1; i >= insertIndex; i--)
        {
            values[i + 1] = values[i];
        }

        // Insert the new element
        values[insertIndex].setDataValue(data->GetDataValue());

        return true;
    }



    template <typename T, int N, int M>
    bool BPlusTreeExternalNode<T, N, M>::DeleteExternal(BPlusTreeDataValue<T, N, M>* data)
    {
        // node should have atleast M/2 elements
        if(values[MINIMUM_CHILDREN_TO_MERGE+1].GetDataValue() == 0)
            throw std::runtime_error("External node is underflow, not meeting minimum children, Deletion from ExternalNode used incorrectly please move/merge beforehand");
        // we are allowed to delete now lets find the element
        int deleteIndex = 0;
        for(auto value: values)
        {
            if(value.GetDataValue() == data->GetDataValue())
            {
                break;
            }
            deleteIndex++;
        }
        // Check if the value was found
        if (deleteIndex < M && values[deleteIndex].GetDataValue() == data->GetDataValue())
        {
            // Delete the element and set to null
            values[deleteIndex].setDataValue(0);

            // Shift the remaining values to fill the gap
            for (int i = deleteIndex; i < M - 1; i++)
            {
                values[i] = values[i + 1];
            }
            values[M - 1].setDataValue(0);

            return true; // Deletion successful
        }
        else
            return false; // Value not found
    }

   template <typename T, int N, int M>
    BPlusTreeDataValue<T, N, M>* BPlusTreeExternalNode<T, N, M>::SearchExternal(BPlusTreeDataValue<T, N, M>* data)
    {
        // Find the correct value if it exists
        for (auto& value : values)
        {
            if (value.GetDataValue() == data->GetDataValue())
            {
                return &value; // Return a pointer to the found value
            }
        }
        return nullptr; // Return nullptr if not found
    }


    template <typename T, int N, int M>
    BPlusTreeExternalNode<T, N, M>* BPlusTreeExternalNode<T, N, M>::SplitExternal()
    {
        // Create a new sibling external node
        BPlusTreeExternalNode<T, N, M>* newSibling = new BPlusTreeExternalNode<T, N, M>();

        // Calculate the midpoint to split the data values
        int midpoint = M / 2;

        // Move the data values from the current node to the new sibling
        for (int i = midpoint; i < M; i++)
        {
            newSibling->values[i - midpoint].setDataValue(values[i].GetDataValue());
            values[i].setDataValue(0); // Clear the reference in the current node
        }

        // Update the parent reference for the new sibling
        newSibling->parent = parent;

        // Update the parent's reference to the new sibling using the InsertKeyAndChild function
        if (parent)
        {
            // parent will never be leaf node therefore we can cast to internal node
            auto newparent = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(parent);
            newparent->InsertKeyAndChild(newSibling->values[0].GetDataValue(), newSibling);
        }

        return newSibling; // Return the new sibling node
    }





   template <typename T, int N, int M>
    bool BPlusTreeExternalNode<T, N, M>::Merge()
    {
        // Ensure this node is not underfull (has enough values to merge)
        if (GetNumValues() >= MINIMUM_CHILDREN_TO_MERGE)
        {
            return false; // Cannot merge
        }

        // Find the left sibling and right sibling of this node if they exist
        BPlusTreeExternalNode<T, N, M>* leftSibling = nullptr;
        BPlusTreeExternalNode<T, N, M>* rightSibling = nullptr;

        BPlusTreeInternalNode<T, N, M>* parent = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(GetParent());

        if (parent)
        {
            int indexInParent = parent->FindChildIndex(this);

            if (indexInParent > 0)
            {
                leftSibling = dynamic_cast<BPlusTreeExternalNode<T, N, M>*>(parent->GetChild(indexInParent - 1));
            }

            if (indexInParent < parent->ChildCount() - 1)
            {
                rightSibling = dynamic_cast<BPlusTreeExternalNode<T, N, M>*>(parent->GetChild(indexInParent + 1));
            }
        }

        // Merge with the left sibling if possible
        if (leftSibling && leftSibling->GetNumValues() > MINIMUM_CHILDREN_TO_MERGE)
        {
            // Transfer values from the left sibling to this node
            for (int i = GetNumValues(); i > 0; i--)
            {
                values[i] = values[i - 1];
            }

            values[0] = leftSibling->values[leftSibling->GetNumValues() - 1];

            // Update the parent's key
            if (parent)
            {
                parent->SetKey(parent->FindChildIndex(this), values[0].GetDataValue());
            }

            // Remove the value from the left sibling
            leftSibling->values[leftSibling->GetNumValues() - 1].setDataValue(0);

            return true;
        }

        // Merge with the right sibling if possible
        if (rightSibling && rightSibling->GetNumValues() > MINIMUM_CHILDREN_TO_MERGE)
        {
            // Transfer values from the right sibling to this node
            values[GetNumValues()] = rightSibling->values[0];

            // Update the parent's key
            if (parent)
            {
                parent->SetKey(parent->FindChildIndex(rightSibling), rightSibling->values[1].GetDataValue());
            }

            // Shift values in the right sibling
            for (int i = 0; i < rightSibling->GetNumValues() - 1; i++)
            {
                rightSibling->values[i] = rightSibling->values[i + 1];
            }

            // Remove the last value from the right sibling
            rightSibling->values[rightSibling->GetNumValues() - 1].setDataValue(0);

            return true;
        }

        // If merging with siblings is not possible, consider merging with the parent
        if (parent)
        {
            // Check if merging with the parent is possible and beneficial
            if (parent->ChildCount() > MINIMUM_CHILDREN_TO_MERGE)
            {
                int indexInParent = parent->FindChildIndex(this);

                if (indexInParent > 0)
                {
                    // Merge with the left sibling and update the parent's key
                    BPlusTreeExternalNode<T, N, M>* leftSibling = dynamic_cast<BPlusTreeExternalNode<T, N, M>*>(parent->GetChild(indexInParent - 1));
                    leftSibling->Merge();
                    parent->RemoveChild(this);
                    delete this;
                }
                else if (indexInParent < parent->ChildCount() - 1)
                {
                    // Merge with the right sibling and update the parent's key
                    BPlusTreeExternalNode<T, N, M>* rightSibling = dynamic_cast<BPlusTreeExternalNode<T, N, M>*>(parent->GetChild(indexInParent + 1));
                    rightSibling->Merge();
                    parent->RemoveChild(rightSibling);
                    delete rightSibling;
                }
            }
        }

        return false; // Cannot merge
    }



    template <typename T, int N, int M>
    void BPlusTreeExternalNode<T, N, M>::Print() // Print logic not same as internal node
    {
        std::cout << "[";
        for (int i = 0; i < M; i++)
        {
            if (values[i].GetDataValue() != 0)
            {
                std::cout << values[i].GetDataValue();
            }
            if (i < M - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << "]";
    }

    template <typename T, int N, int M>
    bool BPlusTreeExternalNode<T, N, M>::isFull()
    {
        for (int i = 0; i < M; i++)
        {
            if (values[i].GetDataValue() == 0)
                return false;
        }
        return true;
    }

    template <typename T, int N, int M>
    bool BPlusTreeExternalNode<T, N, M>::Contains(T data)
    {
        for (int i = 0; i < M; i++)
        {
            if (values[i].GetDataValue() == data)
                return true;
        }
        return false;
    }
 

    template <typename T, int N, int M>
    int BPlusTreeExternalNode<T, N, M>::GetNumValues()
    {
        int count = 0;
        for (int i = 0; i < M; i++)
        {
            if (values[i].GetDataValue() != 0)
                count++;
        }
        return count;
    }

    template <typename T, int N, int M>
    T BPlusTreeExternalNode<T, N, M>::GetMaxValue()
    {
        return values[M - 1].GetDataValue();
    }
//** B+ Tree **//

    template <typename T, int N, int M>
    bool BPlusTree<T, N, M>::Insert(T data) {
        // Create a BPlusTreeDataValue object with the provided data
        BPlusTreeDataValue<T, N, M> dataValue(data);

        // Attempt to insert the data into the tree
        bool insertionResult = _Insert(dataValue.GetDataValue());

        if (insertionResult) {
            // If insertion was successful, update the keys in internal nodes
            UpdateKeys(root);
        }

        return insertionResult;
    }

    template <typename T, int N, int M>
    bool BPlusTree<T, N, M>::Delete(T data) {
        // Attempt to delete the data from the tree
        bool deletionResult = _Delete(data);

        if (deletionResult) {
            // If deletion was successful, update the keys in internal nodes
            UpdateKeys(root);
        }

        return deletionResult;
    }


    template <typename T, int N, int M>
    bool BPlusTree<T, N, M>::_Insert(T data) {
        // Check if the tree is empty
        if (root == nullptr) {
            // Create a new root external node
            BPlusTreeExternalNode<T, N, M>* newRoot = new BPlusTreeExternalNode<T, N, M>();

            // Insert the new data value into the new root external node
            newRoot->InsertExternal(new BPlusTreeDataValue<T, N, M>(data));

            // Set the root to the new root
            root = newRoot;
            return true; // Insertion was successful
        }

        // Find the leaf node where the data should be inserted
        BPlusTreeExternalNode<T, N, M>* leafNode = root->FindLeafNode(data);

        // Check if the leaf node is full and needs to be split
        if (leafNode->isFull()) {
            // Split the leaf node and update the parent nodes as necessary
            BPlusTreeExternalNode<T, N, M>* newSibling = leafNode->SplitExternal();
            if (newSibling) {
                // Create a new internal node as the parent of the split nodes
                BPlusTreeInternalNode<T, N, M>* newParent = new BPlusTreeInternalNode<T, N, M>();
                newParent->InsertInternal(new BPlusTreeDataNodePointer<T, N, M>(leafNode));
                newParent->InsertInternal(new BPlusTreeDataNodePointer<T, N, M>(newSibling));

                // Update the parent pointers for the split nodes
                leafNode->SetParent(newParent);
                newSibling->SetParent(newParent);

                // Update the root if necessary
                if (root == leafNode) {
                    root = newParent;
                }
            }
        }

        // Insert the data into the leaf node
        leafNode->InsertExternal(new BPlusTreeDataValue<T, N, M>(data));
        return true; // Insertion was successful
    }




   template <typename T, int N, int M>
    bool BPlusTree<T, N, M>::_Delete(T data)
    {
        // Step 1: Find the leaf node
        BPlusTreeExternalNode<T, N, M>* leafNode = root->FindLeafNode(data);

        if (!leafNode)
        {
            // Data not found, no action required
            return false;
        }

        // Step 2: Check if we need to merge or move
        const int MIN_MERGE_VAL = M / 2;

        // Step 2.1: Can we delete?
        if (leafNode->GetNumValues() > MIN_MERGE_VAL)
        {
            // Step 2.1.1: Yes, delete
            bool deleted = leafNode->DeleteExternal(data);

            // Check if the deletion was successful
            if (!deleted)
            {
            std::cout << "Error: Data couldn't be deleted, leafNode->DeleteExternal(data) returned false" << std::endl;
            return false;
            }
        }
        else
        {
            // Step 2.2: Can we borrow from a sibling? (left or right)
            BPlusTreeNode<T, N, M>* parent = leafNode->parent;
            int indexInParent = parent->FindChildIndex(leafNode);

            // Attempt to borrow from left sibling
            if (indexInParent > 0)
            {
                BPlusTreeExternalNode<T, N, M>* leftSibling = dynamic_cast<BPlusTreeExternalNode<T, N, M>*>(parent->children[indexInParent - 1]);

                if (leftSibling && leftSibling->GetNumValues() > MIN_MERGE_VAL)
                {
                    // Step 2.2.1: Yes, borrow from leftSibling

                    // Get the largest value from the left sibling
                    BPlusTreeDataValue<T, N, M>* borrowedValue = leftSibling->values[leftSibling->GetNumValues() - 1];

                    // Find the position to insert the borrowed value
                    int insertIndex = 0;
                    while (insertIndex < M && leafNode->values[insertIndex] != nullptr && leafNode->values[insertIndex]->GetValue() < borrowedValue->GetValue())
                    {
                        insertIndex++;
                    }

                    // Shift elements to make space for the borrowed value
                    for (int i = M - 1; i > insertIndex; i--)
                    {
                        leafNode->values[i] = leafNode->values[i - 1];
                    }

                    // Insert the borrowed value into the leaf node
                    leafNode->values[insertIndex] = borrowedValue;

                    // Remove the borrowed value from the left sibling
                    leftSibling->values[leftSibling->GetNumValues() - 1] = nullptr;

                    return true;
                }
            }

            // Attempt to borrow from right sibling
            if (indexInParent < parent->ChildCount() - 1)
            {
                BPlusTreeExternalNode<T, N, M>* rightSibling = dynamic_cast<BPlusTreeExternalNode<T, N, M>*>(parent->children[indexInParent + 1]);

                if (rightSibling && rightSibling->GetNumValues() > MIN_MERGE_VAL)
                {
                    // Step 2.2.2: Yes, borrow from rightSibling

                    // Get the smallest value from the right sibling
                    BPlusTreeDataValue<T, N, M>* borrowedValue = rightSibling->values[0];

                    // Find the position to insert the borrowed value
                    int insertIndex = 0;
                    while (insertIndex < M && leafNode->values[insertIndex] != nullptr && leafNode->values[insertIndex]->GetValue() < borrowedValue->GetValue())
                    {
                        insertIndex++;
                    }

                    // Shift elements to make space for the borrowed value
                    for (int i = M - 1; i > insertIndex; i--)
                    {
                        leafNode->values[i] = leafNode->values[i - 1];
                    }

                    // Insert the borrowed value into the leaf node
                    leafNode->values[insertIndex] = borrowedValue;

                    // Remove the borrowed value from the right sibling
                    for (int i = 0; i < rightSibling->GetNumValues() - 1; i++)
                    {
                        rightSibling->values[i] = rightSibling->values[i + 1];
                    }
                    rightSibling->values[rightSibling->GetNumValues() - 1] = nullptr;

                    return true;
                }
            }
            else
            {
                // Step 2.2.3: No, go to Step 2.3
                // Implement the merge logic in Step 2.3
            }
        }

        // Handle the root if needed
        if (root->ChildCount() == 0)
        {
            // The root node is empty, update it accordingly
            // This might involve changing the root node
        }

        return true; // Deletion was successful
    }



template <typename T, int N, int M>
bool BPlusTree<T, N, M>::_Search(T data) {
    // Start the search from the root node
    BPlusTreeNode<T, N, M>* currentNode = root;

    while (currentNode) {
        if (currentNode->GetNodeType() == BPlusTreeNode<T, N, M>::NodeType::External) {
            // If the current node is a leaf (external node), search for the data in it
            BPlusTreeExternalNode<T, N, M>* leafNode = dynamic_cast<BPlusTreeExternalNode<T, N, M>*>(currentNode);
            
            if (leafNode->Contains(data)) {
                // Data found in the leaf node
                return true;
            } else {
                // Data not found
                return false;
            }
        } else if (currentNode->GetNodeType() == BPlusTreeNode<T, N, M>::NodeType::Internal) {
            // If the current node is an internal node, find the child node to traverse
            BPlusTreeInternalNode<T, N, M>* internalNode = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(currentNode);

            // Find the correct child node based on keys
            int childIndex = 0;
            while (childIndex < internalNode->ChildCount() && internalNode->GetKey(childIndex) <= data) {
                childIndex++;
            }

            // Update the current node to the child node for the next iteration
            currentNode = internalNode->GetChild(childIndex);
        }
    }

    // Data not found in the entire tree
    return false;
}



 template <typename T, int N, int M>
    void BPlusTree<T, N, M>::_Print(BPlusTreeNode<T, N, M>* node)
    {
        _InOrderTraversal(node, 0);
    }

    template <typename T, int N, int M>
    void BPlusTree<T, N, M>::_InOrderTraversal(BPlusTreeNode<T, N, M>* node, int depth)
    {
        if (node)
        {
            // Recursively traverse and print the left subtree
            for (int i = 0; i < node->ChildCount(); i++)
            {
                _InOrderTraversal(node->GetChild(i), depth + 1);
            }

            // Print indentation based on depth
            for (int i = 0; i < depth; i++)
            {
                std::cout << "    "; // Use four spaces for each level of depth
            }

            // Print the keys or values in the current node
            if (node->GetNodeType() == BPlusTreeNode<T, N, M>::NodeType::External)
            {
                BPlusTreeExternalNode<T, N, M>* leafNode = dynamic_cast<BPlusTreeExternalNode<T, N, M>*>(node);
                for (int i = 0; i < M; i++)
                {
                    if (leafNode->GetValues()[i].GetDataValue() != 0)
                    {
                        std::cout << leafNode->GetValues()[i].GetDataValue() << " ";
                    }
                }
            }
            else if (node->GetNodeType() == BPlusTreeNode<T, N, M>::NodeType::Internal)
            {
                BPlusTreeInternalNode<T, N, M>* internalNode = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(node);
                for (int i = 0; i < N - 1; i++)
                {
                    if (internalNode->GetChild(i) != nullptr)
                    {
                        std::cout << internalNode->GetKey(i) << " ";
                    }
                }
            }
            std::cout << std::endl; // Add a newline for clarity

            // Recursively traverse and print the right subtree
            if (node->ChildCount() > 0)
            {
                _InOrderTraversal(node->GetChild(node->ChildCount() - 1), depth + 1);
            }
        }
    }

    template <typename T, int N, int M>
    void BPlusTree<T, N, M>::_Clear(BPlusTreeNode<T, N, M>* node)
    {
        if (!node)
        {
            return; // Nothing to clear
        }

        if (node->GetNodeType() == BPlusTreeNode<T, N, M>::NodeType::External)
        {
            // If the node is a leaf node, cast it to an external node and clear its contents
            BPlusTreeExternalNode<T, N, M>* leafNode = dynamic_cast<BPlusTreeExternalNode<T, N, M>*>(node);
            for (int i = 0; i < M; i++)
            {
                if (leafNode->GetValues()[i].GetDataValue() != 0)
                {
                    leafNode->GetValues()[i].setDataValue(0);
                }
            }
        }
        else
        {
            // If the node is an internal node, cast it to an internal node and recursively clear its children
            BPlusTreeInternalNode<T, N, M>* internalNode = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(node);
            for (int i = 0; i < N; i++)
            {
                if (internalNode->GetChild(i) != nullptr)
                {
                    _Clear(internalNode->GetChild(i));
                }
            }
        }

        // After clearing the children, delete the current node
        delete node;
    }
    
    template <typename T, int N, int M>
    void BPlusTree<T, N, M>::UpdateKeys(BPlusTreeNode<T, N, M>* node) {
        if (!node || node->IsRoot()) {
            return;
        }

        // Calculate the new key value for this node based on its children
        T newKey = CalculateNewKey(node);

        // Update the key value in the node if it's an internal node
        if (node->GetNodeType() == BPlusTreeNode<T, N, M>::NodeType::Internal) {
            BPlusTreeInternalNode<T, N, M>* internalNode = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(node);

            // Find the index for inserting the new key
            int index = 0;
            while (index < internalNode->GetKeyCount() && internalNode->GetKey(index) < newKey) {
                index++;
            }

            // Set the new key value at the specified index
            internalNode->SetKey(index, newKey);
        }

        // Recursively update keys for parent nodes
        UpdateKeys(node->GetParent());
    }

    template <typename T, int N, int M>
    T BPlusTree<T, N, M>::CalculateNewKey(BPlusTreeNode<T, N, M>* node) {
        if (!node || node->IsRoot()) {
            // Handle invalid input or base case if needed
            // You might return a default key or throw an exception here.
            // It depends on how you want to handle such cases.
        }

        T newKey;

        if (node->GetNodeType() == BPlusTreeNode<T, N, M>::NodeType::Internal) {
            BPlusTreeInternalNode<T, N, M>* internalNode = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(node);

            // Use the first key as the associated data value for calculating the new key
            newKey = internalNode->GetKey(0);
        } else {
            // For leaf nodes, calculate the new key based on the highest value in the leaf node
            BPlusTreeExternalNode<T, N, M>* associatedLeafNode = _FindAssociatedLeafNode(node, newKey); // Pass 'newKey' here
            newKey = associatedLeafNode->GetMaxValue();
        }

        // Recursively calculate the new key for parent nodes if not the root
        if (!node->IsRoot()) {
            newKey = CalculateNewKey(node->GetParent());
        }

        return newKey;
    }



    template <typename T, int N, int M>
    BPlusTreeExternalNode<T, N, M>* BPlusTree<T, N, M>::_FindAssociatedLeafNode(BPlusTreeNode<T, N, M>* node, T data) { // Pass 'data' as a parameter
        BPlusTreeNode<T, N, M>* currentNode = node;

        while (currentNode->GetNodeType()!= BPlusTreeNode<T, N, M>::NodeType::External) {
            BPlusTreeInternalNode<T, N, M>* internalNode = dynamic_cast<BPlusTreeInternalNode<T, N, M>*>(currentNode);

            // Find the index of the child to follow based on the data
            int index = 0;
            while (index < internalNode->GetKeyCount() && data >= internalNode->GetKey(index)) {
                index++;
            }

            // Move to the appropriate child
            currentNode = internalNode->GetChild(index);
        }

        // At this point, currentNode is a leaf node associated with the given data
        return dynamic_cast<BPlusTreeExternalNode<T, N, M>*>(currentNode);
    }


} // VLIB
#endif // __BPLUS_TREE_H__