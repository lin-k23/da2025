#include <iostream>

// Node structure for the Singly Linked List
struct NODE
{
    int data;
    NODE *next;
};

class LinkList
{
private:
    NODE *head; // Points to the first node in the list

public:
    // Constructor
    LinkList();
    // Destructor (crucial for freeing memory)
    ~LinkList();

    // --- Basic Operations ---

    void ClearList();
    bool IsEmpty();
    int Length();
    void Traverse(); // A helper to print the list

    // --- Core Data Operations ---

    bool GetElem(int i, int &e);
    int LocateElem(int e);
    bool Insert(int i, int e);
    bool Delete(int i, int &e);
};

// --- Function Implementations ---

// Constructor: Initializes an empty list.
LinkList::LinkList()
{
    head = nullptr;
}

// Destructor: Cleans up all dynamically allocated nodes.
LinkList::~LinkList()
{
    std::cout << "\n--- LinkList Destructor Called: Cleaning up memory... ---" << std::endl;
    ClearList();
}

// ClearList: Deletes all nodes, freeing their memory.
void LinkList::ClearList()
{
    NODE *current = head;
    while (current != nullptr)
    {
        NODE *node_to_delete = current;
        current = current->next;
        delete node_to_delete;
    }
    head = nullptr; // Reset the list to an empty state
}

// IsEmpty: Checks if the list is empty.
bool LinkList::IsEmpty()
{
    return head == nullptr;
}

// Length: Returns the number of elements by traversing the list.
int LinkList::Length()
{
    int count = 0;
    NODE *current = head;
    while (current != nullptr)
    {
        count++;
        current = current->next;
    }
    return count;
}

// Traverse: A utility function to print the contents of the list.
void LinkList::Traverse()
{
    std::cout << "List contents: head -> ";
    NODE *current = head;
    while (current != nullptr)
    {
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << "nullptr | Length: " << this->Length() << std::endl;
}

// GetElem: Retrieves the element at a specific position 'i' (0-based).
bool LinkList::GetElem(int i, int &e)
{
    if (i < 0)
        return false;

    NODE *p = head;
    int j = 0;
    while (p != nullptr && j < i)
    { // Traverse to the ith node
        p = p->next;
        j++;
    }

    if (p == nullptr)
    { // Index is out of bounds
        return false;
    }

    e = p->data;
    // Retrieve the data
    return true;
}

// LocateElem: Returns the index of the first occurrence of element 'e'.
int LinkList::LocateElem(int e)
{
    NODE *p = head;
    int index = 0;
    while (p != nullptr)
    {
        if (p->data == e)
        {
            return index;
        }
        p = p->next;
        index++;
    }
    return -1; // Element not found
}

// Insert: Inserts element 'e' at position 'i'.
bool LinkList::Insert(int i, int e)
{
    if (i < 0)
        return false;

    NODE *new_node = new NODE;
    new_node->data = e;

    // Case 1: Insert at the head (i = 0)
    if (i == 0)
    {
        new_node->next = head;
        head = new_node;
        return true;
    }

    // Case 2: Insert at a position other than the head
    NODE *p = head;
    int j = 0;
    // Traverse to the node *before* the insertion point (at index i-1)
    while (p != nullptr && j < i - 1)
    {
        p = p->next;
        j++;
    }

    if (p == nullptr)
    {                    // The list is shorter than i-1, invalid index
        delete new_node; // Clean up the allocated node
        return false;
    }

    // Perform the insertion
    new_node->next = p->next;
    p->next = new_node;
    return true;
}

// Delete: Deletes the element at position 'i'.
bool LinkList::Delete(int i, int &e)
{
    if (i < 0 || IsEmpty())
        return false;

    NODE *node_to_delete = nullptr;

    // Case 1: Delete the head node (i = 0)
    if (i == 0)
    {
        node_to_delete = head;
        e = head->data;
        head = head->next;
        delete node_to_delete;
        return true;
    }

    // Case 2: Delete a node other than the head
    NODE *p = head;
    int j = 0;
    // Traverse to the node *before* the one to be deleted (at index i-1)
    while (p != nullptr && j < i - 1)
    {
        p = p->next;
        j++;
    }

    // Check if predecessor or the target node is valid
    if (p == nullptr || p->next == nullptr)
    {
        return false; // Invalid index
    }

    // Perform the deletion
    node_to_delete = p->next;
    e = node_to_delete->data;
    p->next = node_to_delete->next;
    // Bypass the deleted node
    delete node_to_delete;
    // Free memory

    return true;
}

// --- Main function acting as a Testbench ---

int main()
{
    std::cout << "--- Initializing LinkList Testbench ---" << std::endl;
    LinkList myList;

    // Test 1: Initial state
    std::cout << "\n[Test 1: Initial State]" << std::endl;
    std::cout << "Is list empty? " << (myList.IsEmpty() ? "Yes" : "No") << std::endl;
    std::cout << "Initial length: " << myList.Length() << std::endl;
    myList.Traverse();

    // Test 2: Insertion
    std::cout << "\n[Test 2: Insertion]" << std::endl;
    myList.Insert(0, 10); // Insert at beginning of empty list
    myList.Traverse();
    myList.Insert(1, 30); // Insert at end
    myList.Traverse();
    myList.Insert(1, 20); // Insert in middle
    myList.Traverse();
    myList.Insert(0, 5); // Insert at head of non-empty list
    myList.Traverse();

    // Test 3: Edge Case Insertion
    std::cout << "\n[Test 3: Edge Case Insertion]" << std::endl;
    bool insert_fail = myList.Insert(10, 99); // Invalid index
    std::cout << "Attempt to insert at invalid index 10: " << (insert_fail ? "Success" : "Failed (Correct)") << std::endl;
    myList.Traverse();

    // Test 4: Get and Locate Element
    std::cout << "\n[Test 4: Get and Locate Element]" << std::endl;
    int val;
    if (myList.GetElem(2, val))
    {
        std::cout << "Element at index 2 is: " << val << std::endl;
    }
    if (!myList.GetElem(5, val))
    {
        std::cout << "Attempt to get element at invalid index 5 failed (Correct)." << std::endl;
    }
    std::cout << "Locating element 30. Index: " << myList.LocateElem(30) << std::endl;
    std::cout << "Locating element 99 (not in list). Index: " << myList.LocateElem(99) << std::endl;

    // Test 5: Deletion
    std::cout << "\n[Test 5: Deletion]" << std::endl;
    int deleted_val;
    myList.Delete(0, deleted_val); // Delete from beginning
    std::cout << "Deleted element at index 0. Value was: " << deleted_val << std::endl;
    myList.Traverse();
    myList.Delete(2, deleted_val); // Delete from end
    std::cout << "Deleted element at index 2. Value was: " << deleted_val << std::endl;
    myList.Traverse();
    myList.Delete(0, deleted_val); // Delete from middle (by index)
    std::cout << "Deleted element at index 0. Value was: " << deleted_val << std::endl;
    myList.Traverse();

    // Test 6: Clear List
    std::cout << "\n[Test 6: Clear List]" << std::endl;
    myList.Insert(0, 100);
    myList.Insert(1, 200);
    myList.Traverse();
    myList.ClearList();
    std::cout << "Is list empty after clearing? " << (myList.IsEmpty() ? "Yes" : "No") << std::endl;
    myList.Traverse();

    // Test 7: Operations on an empty list
    std::cout << "\n[Test 7: Operations on an empty list]" << std::endl;
    bool delete_fail = myList.Delete(0, deleted_val);
    std::cout << "Attempt to delete from empty list: " << (delete_fail ? "Success" : "Failed (Correct)") << std::endl;

    std::cout << "\n--- Testbench Complete ---" << std::endl;
    // The destructor will be automatically called here when main() ends.
    return 0;
}