#include <iostream>
#include <string>

struct D_NODE
{
    int data;
    D_NODE *next;
    D_NODE *prev;
};

class C2LinkList
{
private:
    D_NODE *head; // Points to an arbitrary node in the circle, often the "first"

public:
    // Constructor
    C2LinkList();
    // Destructor
    ~C2LinkList();

    // --- Basic Operations ---
    void ClearList();
    bool IsEmpty();
    int Length();
    void Traverse(); // A helper to print the list

    // --- Core Data Operations ---

    D_NODE *GetElemP(int i); // Helper to get a pointer to the ith node
    bool Insert(int i, int e);
    bool Delete(int i, int &e);
};

// --- Function Implementations ---

// Constructor: Initializes an empty list.
C2LinkList::C2LinkList()
{
    head = nullptr;
}

// Destructor: Cleans up all dynamically allocated nodes.
C2LinkList::~C2LinkList()
{
    std::cout << "\n--- C2LinkList Destructor Called: Cleaning up memory... ---" << std::endl;
    ClearList();
}

// ClearList: Deletes all nodes, freeing their memory.
void C2LinkList::ClearList()
{
    if (IsEmpty())
    {
        return;
    }

    D_NODE *current = head->next;
    while (current != head)
    {
        D_NODE *node_to_delete = current;
        current = current->next;
        delete node_to_delete;
    }
    delete head; // Delete the last remaining node
    head = nullptr;
}

// IsEmpty: Checks if the list is empty.
bool C2LinkList::IsEmpty()
{
    return head == nullptr;
}

// Length: Returns the number of elements by traversing the list.
int C2LinkList::Length()
{
    if (IsEmpty())
    {
        return 0;
    }
    int count = 0;
    D_NODE *current = head;
    do
    {
        count++;
        current = current->next;
    } while (current != head);
    return count;
}

// Traverse: A utility function to print the contents of the list.
void C2LinkList::Traverse()
{
    if (IsEmpty())
    {
        std::cout << "List is empty. | Length: 0" << std::endl;
        return;
    }
    std::cout << "List contents: head -> ";
    D_NODE *current = head;
    do
    {
        std::cout << current->data << " <-> ";
        current = current->next;
    } while (current != head);
    std::cout << "(head:" << head->data << ") | Length: " << this->Length() << std::endl;
}

// GetElemP: Helper function to get a pointer to the ith node.
D_NODE *C2LinkList::GetElemP(int i)
{
    if (i < 0 || IsEmpty())
        return nullptr;

    D_NODE *p = head;
    int j = 0;
    while (j < i)
    {
        p = p->next;
        j++;
    }
    return p;
}

// Insert: Inserts element 'e' at position 'i'.
bool C2LinkList::Insert(int i, int e)
{
    if (i < 0)
        return false;

    D_NODE *new_node = new D_NODE;
    new_node->data = e;

    // Case 1: The list is empty
    if (IsEmpty())
    {
        head = new_node;
        head->next = head;
        head->prev = head;
        return true;
    }

    // Find the node `p` that will be at position `i` *after* insertion
    D_NODE *p = GetElemP(i);
    if (p == nullptr)
    { // This can happen if i >= length
        // Insert at the end, which is before the head
        p = head;
    }

    // Get the predecessor of p
    D_NODE *p_prev = p->prev;

    // Perform the pointer updates as per the slide
    new_node->prev = p_prev; // Step 1
    p_prev->next = new_node; // Step 2
    new_node->next = p;      // Step 3
    p->prev = new_node;      // Step 4

    // If we inserted before the old head, the new node becomes the head
    if (i == 0)
    {
        head = new_node;
    }

    return true;
}

// Delete: Deletes the element at position 'i'.
bool C2LinkList::Delete(int i, int &e)
{
    if (i < 0 || IsEmpty())
        return false;

    // Find the node to be deleted
    D_NODE *p = GetElemP(i);
    if (p == nullptr)
        return false; // Index out of bounds

    // Case 1: Deleting the only node in the list
    if (p->next == p)
    {
        e = p->data;
        delete p;
        head = nullptr;
        return true;
    }

    // Case 2: Deleting from a list with more than one node
    D_NODE *p_prev = p->prev;
    D_NODE *p_next = p->next;

    // Perform pointer updates as per the slide
    p_prev->next = p_next; // Step 1
    p_next->prev = p_prev; // Step 2

    e = p->data;

    // If we deleted the head, update head to the next node
    if (p == head)
    {
        head = p_next;
    }

    delete p; // Step 3
    return true;
}

// --- Main function acting as a Testbench ---

int main()
{
    std::cout << "--- Initializing C2LinkList Testbench ---" << std::endl;
    C2LinkList myList;

    // Test 1: Initial state
    std::cout << "\n[Test 1: Initial State]" << std::endl;
    std::cout << "Is list empty? " << (myList.IsEmpty() ? "Yes" : "No") << std::endl;
    myList.Traverse();

    // Test 2: Insertion
    std::cout << "\n[Test 2: Insertion]" << std::endl;
    myList.Insert(0, 10); // Insert into empty list
    myList.Traverse();
    myList.Insert(1, 30); // Insert at the end
    myList.Traverse();
    myList.Insert(1, 20); // Insert in the middle
    myList.Traverse();
    myList.Insert(0, 5); // Insert at the new head
    myList.Traverse();
    // Verify circular property by checking head's predecessor
    D_NODE *head_node = myList.GetElemP(0);
    if (head_node)
    {
        std::cout << "Head is " << head_node->data << ". Head's predecessor is " << head_node->prev->data << " (should be last element)." << std::endl;
    }

    // Test 3: Deletion
    std::cout << "\n[Test 3: Deletion]" << std::endl;
    int deleted_val;
    myList.Delete(0, deleted_val); // Delete the head
    std::cout << "Deleted element at index 0. Value was: " << deleted_val << std::endl;
    myList.Traverse();

    myList.Delete(2, deleted_val); // Delete the tail
    std::cout << "Deleted element at index 2. Value was: " << deleted_val << std::endl;
    myList.Traverse();

    myList.Insert(1, 15);
    myList.Traverse();
    myList.Delete(1, deleted_val); // Delete from the middle
    std::cout << "Deleted element at index 1. Value was: " << deleted_val << std::endl;
    myList.Traverse();

    // Verify circular property again
    head_node = myList.GetElemP(0);
    if (head_node)
    {
        std::cout << "Head is " << head_node->data << ". Head's predecessor is " << head_node->prev->data << "." << std::endl;
    }

    // Test 4: Clear List
    std::cout << "\n[Test 4: Clear List]" << std::endl;
    myList.Traverse();
    myList.ClearList();
    std::cout << "Is list empty after clearing? " << (myList.IsEmpty() ? "Yes" : "No") << std::endl;
    myList.Traverse();

    // Test 5: Delete the only node
    std::cout << "\n[Test 5: Delete the only node]" << std::endl;
    myList.Insert(0, 100);
    myList.Traverse();
    myList.Delete(0, deleted_val);
    std::cout << "Deleted the only node. Value was: " << deleted_val << std::endl;
    myList.Traverse();

    std::cout << "\n--- Testbench Complete ---" << std::endl;
    // The destructor will be automatically called here when main() ends.
    return 0;
}