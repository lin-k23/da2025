#include <iostream>
#include <list>
#include <string>
#include <iterator> // For std::advance

// 辅助函数，用于打印 std::list 的内容
void print_list(const std::string &message, const std::list<int> &l)
{
    std::cout << message << " head <-> ";
    for (int val : l)
    {
        std::cout << val << " <-> ";
    }
    std::cout << "tail | Size: " << l.size() << std::endl;
}

int main()
{
    std::cout << "--- Initializing std::list Testbench ---" << std::endl;
    std::list<int> myList;

    // Test 1: Initial state
    std::cout << "\n[Test 1: Initial State]" << std::endl;
    std::cout << "Is list empty? " << (myList.empty() ? "Yes" : "No") << std::endl;
    print_list("Initial contents:", myList);

    // Test 2: Insertion
    std::cout << "\n[Test 2: Insertion]" << std::endl;
    // Insert into empty list (equivalent to Insert(0, 10))
    myList.push_front(10);
    print_list("Inserted 10:", myList);

    // Insert at the end (equivalent to Insert(1, 30))
    myList.push_back(30);
    print_list("Inserted 30 at the end:", myList);

    // Insert in the middle (equivalent to Insert(1, 20))
    auto it = myList.begin();
    std::advance(it, 1); // Move iterator to the 1st position
    myList.insert(it, 20);
    print_list("Inserted 20 at index 1:", myList);

    // Insert at the new head (equivalent to Insert(0, 5))
    myList.push_front(5);
    print_list("Inserted 5 at the front:", myList);

    // Verify boundary property
    std::cout << "Head is " << myList.front() << ". Tail is " << myList.back() << "." << std::endl;
    std::cout << "(Note: In std::list, the head's predecessor is not the tail)" << std::endl;

    // Test 3: Deletion
    std::cout << "\n[Test 3: Deletion]" << std::endl;
    int deleted_val = myList.front();
    myList.pop_front(); // Delete the head
    std::cout << "Deleted element at front. Value was: " << deleted_val << std::endl;
    print_list("After deletion:", myList);

    deleted_val = myList.back();
    myList.pop_back(); // Delete the tail
    std::cout << "Deleted element at back. Value was: " << deleted_val << std::endl;
    print_list("After deletion:", myList);

    // Re-insert an element to test middle deletion
    it = myList.begin();
    std::advance(it, 1);
    myList.insert(it, 15);
    print_list("Re-inserted 15:", myList);

    // Delete from the middle
    it = myList.begin();
    std::advance(it, 1);
    deleted_val = *it;
    myList.erase(it);
    std::cout << "Deleted element at index 1. Value was: " << deleted_val << std::endl;
    print_list("After deletion:", myList);

    // Test 4: Clear List
    std::cout << "\n[Test 4: Clear List]" << std::endl;
    myList.clear();
    std::cout << "Is list empty after clearing? " << (myList.empty() ? "Yes" : "No") << std::endl;
    print_list("After clearing:", myList);

    // Test 5: Delete the only node
    std::cout << "\n[Test 5: Delete the only node]" << std::endl;
    myList.push_front(100);
    print_list("List with one element:", myList);
    deleted_val = myList.front();
    myList.pop_front();
    std::cout << "Deleted the only node. Value was: " << deleted_val << std::endl;
    print_list("After deletion:", myList);

    std::cout << "\n--- Testbench Complete ---" << std::endl;
    return 0;
}