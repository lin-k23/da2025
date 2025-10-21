#include <iostream>

const int LIST_MAX_SIZE = 100;

class SqList
{
private:
    int elem[LIST_MAX_SIZE]; // Storage for elements
    int length;              // Current number of elements

public:
    SqList();
    void ClearList();
    bool IsEmpty();
    int ListLength();
    bool GetElem(int i, int &e);    // i: 0-based
    int LocateElem(int e);          // 返回0-based下标，未找到返回-1
    bool ListInsert(int i, int e);  // i: 0-based
    bool ListDelete(int i, int &e); // i: 0-based
    void ListTraverse();
};

// --- Function Implementations ---

// Constructor: Initializes an empty list.
SqList::SqList()
{
    length = 0;
}

// ClearList: Resets the list to be empty.
void SqList::ClearList()
{
    length = 0;
}

// IsEmpty: Checks if the list is empty.
bool SqList::IsEmpty()
{
    return length == 0;
}

// ListLength: Returns the current number of elements in the list.
int SqList::ListLength()
{
    return length;
}

// GetElem: Retrieves the element at index i (0-based).
bool SqList::GetElem(int i, int &e)
{
    if (i < 0 || i >= length)
    {
        return false; // Invalid index
    }
    e = elem[i];
    return true;
}

// LocateElem: Finds the index of the first occurrence of element e (0-based).
int SqList::LocateElem(int e)
{
    for (int i = 0; i < length; ++i)
    {
        if (elem[i] == e)
        {
            return i; // 0-based index
        }
    }
    return -1; // Not found
}

// ListInsert: Inserts element e at index i (0-based).
bool SqList::ListInsert(int i, int e)
{
    if (i < 0 || i > length || length >= LIST_MAX_SIZE)
    {
        return false; // Invalid index or list is full
    }
    for (int j = length; j > i; --j)
    {
        elem[j] = elem[j - 1]; // Shift elements to the right
    }
    elem[i] = e; // Insert new element
    length++;
    return true;
}

// ListDelete: Deletes the element at index i (0-based) and returns it in e.
bool SqList::ListDelete(int i, int &e)
{
    if (i < 0 || i >= length)
    {
        return false; // Invalid index
    }
    e = elem[i];
    for (int j = i; j < length - 1; ++j)
    {
        elem[j] = elem[j + 1]; // Shift elements to the left
    }
    length--;
    return true;
}

// ListTraverse: Prints all elements in the list.
void SqList::ListTraverse()
{
    for (int i = 0; i < length; ++i)
    {
        std::cout << elem[i] << " ";
    }
    std::cout << std::endl;
}

// --- Main function acting as a Testbench ---

int main()
{
    std::cout << "--- Initializing SqList Testbench ---" << std::endl;
    SqList myList;

    // Test 1: Initial state
    std::cout << "\n[Test 1: Initial State]" << std::endl;
    std::cout << "Is list empty? " << (myList.IsEmpty() ? "Yes" : "No") << std::endl;
    std::cout << "Initial length: " << myList.ListLength() << std::endl;
    myList.ListTraverse();

    // Test 2: Insertion
    std::cout << "\n[Test 2: Insertion]" << std::endl;
    myList.ListInsert(0, 10); // Insert at beginning
    myList.ListInsert(1, 30); // Insert at end
    myList.ListInsert(1, 20); // Insert in middle
    myList.ListInsert(3, 40); // Insert at end again
    std::cout << "After inserting 10, 20, 30, 40:" << std::endl;
    myList.ListTraverse();
    std::cout << "Current length: " << myList.ListLength() << std::endl;

    // Test 3: Edge Case Insertion
    std::cout << "\n[Test 3: Edge Case Insertion]" << std::endl;
    bool insert_fail = myList.ListInsert(10, 99); // Invalid index
    std::cout << "Attempt to insert at invalid index 10: " << (insert_fail ? "Success" : "Failed (Correct)") << std::endl;
    myList.ListTraverse();

    // Test 4: Get Element
    std::cout << "\n[Test 4: Get Element]" << std::endl;
    int val;
    if (myList.GetElem(1, val))
    {
        std::cout << "Element at index 1 is: " << val << std::endl;
    }
    if (!myList.GetElem(5, val))
    {
        std::cout << "Attempt to get element at invalid index 5 failed (Correct)." << std::endl;
    }

    // Test 5: Locate Element
    std::cout << "\n[Test 5: Locate Element]" << std::endl;
    std::cout << "Locating element 30. Index: " << myList.LocateElem(30) << std::endl;
    std::cout << "Locating element 99 (not in list). Index: " << myList.LocateElem(99) << std::endl;

    // Test 6: Deletion
    std::cout << "\n[Test 6: Deletion]" << std::endl;
    int deleted_val;
    myList.ListDelete(0, deleted_val); // Delete from beginning
    std::cout << "Deleted element at index 0. Value was: " << deleted_val << std::endl;
    myList.ListTraverse();
    myList.ListDelete(2, deleted_val); // Delete from end
    std::cout << "Deleted element at index 2. Value was: " << deleted_val << std::endl;
    myList.ListTraverse();
    myList.ListInsert(0, 10);
    myList.ListInsert(3, 40);
    myList.ListTraverse();
    myList.ListDelete(1, deleted_val); // Delete from middle
    std::cout << "Deleted element at index 1. Value was: " << deleted_val << std::endl;
    myList.ListTraverse();

    // Test 7: Edge Case Deletion
    std::cout << "\n[Test 7: Edge Case Deletion]" << std::endl;
    bool delete_fail = myList.ListDelete(5, deleted_val); // Invalid index
    std::cout << "Attempt to delete at invalid index 5: " << (delete_fail ? "Success" : "Failed (Correct)") << std::endl;

    // Test 8: Clear List
    std::cout << "\n[Test 8: Clear List]" << std::endl;
    myList.ClearList();
    std::cout << "Is list empty after clearing? " << (myList.IsEmpty() ? "Yes" : "No") << std::endl;
    myList.ListTraverse();

    // Test 9: Deletion from an empty list
    std::cout << "\n[Test 9: Deletion from empty list]" << std::endl;
    delete_fail = myList.ListDelete(0, deleted_val);
    std::cout << "Attempt to delete from empty list: " << (delete_fail ? "Success" : "Failed (Correct)") << std::endl;

    std::cout << "\n--- Testbench Complete ---" << std::endl;

    return 0;
}