#include <iostream>
#include <forward_list>
#include <string>
#include <iterator> // For std::distance

// 辅助函数，用于打印 forward_list 内容
void print_list(const std::string &message, const std::forward_list<int> &flist)
{
    std::cout << message << " head -> ";
    for (int val : flist)
    {
        std::cout << val << " -> ";
    }
    // forward_list 没有 .size() 方法，需要手动计算
    size_t length = std::distance(flist.begin(), flist.end());
    std::cout << "nullptr | Length: " << length << std::endl;
}

int main()
{
    std::cout << "--- Initializing std::forward_list Testbench ---" << std::endl;
    std::forward_list<int> myList;

    // Test 1: Initial state
    std::cout << "\n[Test 1: Initial State]" << std::endl;
    std::cout << "Is list empty? " << (myList.empty() ? "Yes" : "No") << std::endl;
    print_list("Initial contents:", myList);

    // Test 2: Insertion
    std::cout << "\n[Test 2: Insertion]" << std::endl;
    // Insert at beginning of empty list (index 0)
    myList.push_front(10);
    print_list("Inserted 10 at index 0:", myList);

    // Insert at end (index 1) - forward_list 需要找到前一个元素
    auto it = myList.before_begin();
    std::advance(it, 1);
    myList.insert_after(it, 30);
    print_list("Inserted 30 at index 1:", myList);

    // Insert in middle (index 1)
    it = myList.before_begin();
    std::advance(it, 1);
    myList.insert_after(it, 20);
    print_list("Inserted 20 at index 1:", myList);

    // Insert at head of non-empty list (index 0)
    myList.push_front(5);
    print_list("Inserted 5 at index 0:", myList);

    // Test 3: Edge Case Insertion
    std::cout << "\n[Test 3: Edge Case Insertion]" << std::endl;
    std::cout << "std::forward_list does not support random access insertion by index." << std::endl;
    std::cout << "Operations are done via iterators, preventing invalid index access at compile/runtime." << std::endl;

    // Test 4: Get and Locate Element
    std::cout << "\n[Test 4: Get and Locate Element]" << std::endl;
    // Get element at index 2
    it = myList.begin();
    std::advance(it, 2);
    int val = *it;
    std::cout << "Element at index 2 is: " << val << std::endl;

    // Locate element 30
    int index = 0;
    bool found = false;
    for (int current_val : myList)
    {
        if (current_val == 30)
        {
            std::cout << "Locating element 30. Index: " << index << std::endl;
            found = true;
            break;
        }
        index++;
    }
    if (!found)
    {
        std::cout << "Locating element 30. Index: -1" << std::endl;
    }

    // Test 5: Deletion
    std::cout << "\n[Test 5: Deletion]" << std::endl;
    int deleted_val = myList.front();
    myList.pop_front(); // Delete from beginning
    std::cout << "Deleted element at index 0. Value was: " << deleted_val << std::endl;
    print_list("After deletion:", myList);

    // Delete from end (index 2)
    it = myList.before_begin();
    std::advance(it, 1); // it is now before the element at index 1
    deleted_val = *(std::next(it));
    myList.erase_after(it);
    std::cout << "Deleted element at index 2 (original). Value was: " << deleted_val << std::endl;
    print_list("After deletion:", myList);

    // Delete from middle (index 0)
    deleted_val = myList.front();
    myList.pop_front();
    std::cout << "Deleted element at index 0. Value was: " << deleted_val << std::endl;
    print_list("After deletion:", myList);

    // Test 6: Clear List
    std::cout << "\n[Test 6: Clear List]" << std::endl;
    myList.push_front(200);
    myList.push_front(100);
    print_list("Before clearing:", myList);
    myList.clear();
    std::cout << "Is list empty after clearing? " << (myList.empty() ? "Yes" : "No") << std::endl;
    print_list("After clearing:", myList);

    // Test 7: Operations on an empty list
    std::cout << "\n[Test 7: Operations on an empty list]" << std::endl;
    std::cout << "Attempt to delete from empty list... (std::forward_list doesn't throw error, it's undefined behavior if not checked)" << std::endl;
    if (!myList.empty())
    {
        myList.pop_front();
    }
    else
    {
        std::cout << "List is empty, deletion skipped (Correct)." << std::endl;
    }

    std::cout << "\n--- Testbench Complete ---" << std::endl;
    return 0;
}