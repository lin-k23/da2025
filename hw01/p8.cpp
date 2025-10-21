#include <iostream>

using ElemType = int;

struct NODE
{
    ElemType data;
    NODE *next;

    NODE() : data(0), next(NULL) {}
};

NODE *list_find(NODE *current, ElemType x)
{
    // Base Case 1: Reached the end of the list, or the list is empty. Value not found.
    if (current == NULL)
    {
        return NULL;
    }

    // Base Case 2: Found the target value in the current node.
    if (current->data == x)
    {
        return current;
    }

    // Recursive Step: Search in the rest of the list (from the next node).
    return list_find(current->next, x);
}

// Helper function to print the contents of the list
void print_list(NODE *head)
{
    std::cout << "Current list: ";
    NODE *p = head;
    while (p != NULL)
    {
        std::cout << p->data << " -> ";
        p = p->next;
    }
    std::cout << "NULL" << std::endl;
}

// The main function for testing
int main()
{
    // --- Initialize Linked List ---
    std::cout << "--- Initializing Linked List ---" << std::endl;
    NODE *head = new NODE();
    head->data = 10;

    head->next = new NODE();
    head->next->data = 20;

    head->next->next = new NODE();
    head->next->next->data = 30;

    head->next->next->next = new NODE();
    head->next->next->next->data = 40;

    // The list is now: 10 -> 20 -> 30 -> 40 -> NULL
    print_list(head);
    std::cout << std::endl;

    // --- Test the list_find function ---
    std::cout << "--- Testing (list_find) ---" << std::endl;

    // Test 1: Find a value that exists in the list (30)
    ElemType value_to_find = 30;
    std::cout << "Searching for value: " << value_to_find << std::endl;
    NODE *found_node = list_find(head, value_to_find);
    if (found_node != NULL)
    {
        std::cout << "  Success! Found the node with value " << found_node->data << "." << std::endl;
    }
    else
    {
        std::cout << "  Failure! Could not find the node with value " << value_to_find << "." << std::endl;
    }

    // Test 2: Find a value that does not exist in the list (99)
    value_to_find = 99;
    std::cout << "Searching for value: " << value_to_find << std::endl;
    found_node = list_find(head, value_to_find);
    if (found_node != NULL)
    {
        std::cout << "  Success! Found the node with value " << found_node->data << "." << std::endl;
    }
    else
    {
        std::cout << "  Failure! Could not find the node with value " << value_to_find << "." << std::endl;
    }
    std::cout << "--- Test Finished ---\n"
              << std::endl;

    // --- Clean up allocated memory ---
    std::cout << "--- Cleaning Up Memory ---" << std::endl;
    NODE *current = head;
    NODE *temp_next;
    while (current != NULL)
    {
        temp_next = current->next;
        std::cout << "Deleting node: " << current->data << std::endl;
        delete current;
        current = temp_next;
    }
    head = NULL;
    std::cout << "Memory cleanup complete." << std::endl;

    return 0;
}