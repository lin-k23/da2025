#include <iostream>

/**
 * @class LinkedStack
 * @brief A linked list implementation of a stack.
 * This implementation is based on the "Linked Stack" concept from the slides.
 * The head of the linked list acts as the top of the stack.
 */

class LinkedStack
{
private:
    // Node structure for the linked list
    struct Node
    {
        int data;
        Node *next;
        // Constructor for convenience
        Node(int val, Node *link) : data(val), next(link) {}
    };

    Node *top; // Pointer to the top of the stack (head of the list)
    int count; // Keeps track of the number of elements for an O(1) StackLength

public:
    LinkedStack();
    ~LinkedStack(); // Destructor to prevent memory leaks

    void ClearStack();
    bool IsEmpty();
    int StackLength();
    bool GetTop(int &e);  // Retrieves the top element without popping
    bool Push(int e);     // Pushes an element onto the stack
    bool Pop(int &e);     // Pops an element from the stack
    void StackTraverse(); // Displays the content of the stack for testing
};

// --- Function Implementations ---

// Constructor: Initializes an empty stack.
LinkedStack::LinkedStack()
{
    top = nullptr;
    count = 0;
}

// Destructor: Deallocates all nodes in the stack.
LinkedStack::~LinkedStack()
{
    ClearStack();
}

// ClearStack: Removes all elements from the stack, freeing memory.
// The logic follows slide 21, repeatedly popping until empty.
void LinkedStack::ClearStack()
{
    Node *current = top;
    while (current != nullptr)
    {
        Node *temp = current;
        current = current->next;
        delete temp;
    }
    top = nullptr;
    count = 0;
}

// IsEmpty: Checks if the stack is empty.
// An empty stack is identified by a null head/top pointer.
bool LinkedStack::IsEmpty()
{
    return top == nullptr;
}

// StackLength: Returns the current number of elements. O(1) complexity.
int LinkedStack::StackLength()
{
    return count;
}

// GetTop: Retrieves the top element into 'e' without removing it.
// Returns the item from the head node.
bool LinkedStack::GetTop(int &e)
{
    if (IsEmpty())
    {
        return false; // Stack is empty
    }
    e = top->data;
    return true;
}

// Push: Inserts element 'e' at the top of the stack (head of the list).
// This operation has a time complexity of O(1).
// The logic is equivalent to "head = new node(e, head)" from slide 19.
bool LinkedStack::Push(int e)
{
    Node *newNode = new (std::nothrow) Node(e, top);
    if (newNode == nullptr)
    {
        return false; // Memory allocation failed
    }
    top = newNode;
    count++;
    return true;
}

// Pop: Deletes the top element and returns it in 'e'.
// This operation has a time complexity of O(1).
// The logic follows the diagram and code on slide 18.
bool LinkedStack::Pop(int &e)
{
    if (IsEmpty())
    {
        return false; // Stack is empty
    }
    e = top->data;
    Node *temp = top;
    top = top->next;
    delete temp;
    count--;
    return true;
}

// StackTraverse: Prints all elements from top to bottom.
void LinkedStack::StackTraverse()
{
    std::cout << "Stack (top -> bottom): ";
    Node *current = top;
    while (current != nullptr)
    {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

// --- Main function acting as a Testbench ---

int main()
{
    std::cout << "--- Initializing LinkedStack Testbench ---" << std::endl;
    LinkedStack myStack;

    // Test 1: Initial state
    std::cout << "\n[Test 1: Initial State]" << std::endl;
    std::cout << "Is stack empty? " << (myStack.IsEmpty() ? "Yes" : "No") << std::endl;
    std::cout << "Initial length: " << myStack.StackLength() << std::endl;

    // Test 2: Push operation
    std::cout << "\n[Test 2: Push Operation]" << std::endl;
    std::cout << "Pushing 10, 20, 30..." << std::endl;
    myStack.Push(10);
    myStack.Push(20);
    myStack.Push(30);
    myStack.StackTraverse();
    std::cout << "Current length: " << myStack.StackLength() << std::endl;

    // Test 3: GetTop operation
    std::cout << "\n[Test 3: GetTop Operation]" << std::endl;
    int top_val;
    if (myStack.GetTop(top_val))
    {
        std::cout << "Top element is: " << top_val << std::endl;
    }
    std::cout << "Stack state is unchanged: ";
    myStack.StackTraverse();

    // Test 4: Pop operation
    std::cout << "\n[Test 4: Pop Operation]" << std::endl;
    int popped_val;
    if (myStack.Pop(popped_val))
    {
        std::cout << "Popped value: " << popped_val << std::endl;
    }
    myStack.StackTraverse();
    if (myStack.Pop(popped_val))
    {
        std::cout << "Popped value: " << popped_val << std::endl;
    }
    myStack.StackTraverse();
    std::cout << "Current length: " << myStack.StackLength() << std::endl;

    // Test 5: Edge Case - Pop/GetTop from empty stack
    std::cout << "\n[Test 5: Edge Cases on Empty Stack]" << std::endl;
    myStack.Pop(popped_val); // Pop the last element (10)
    std::cout << "After popping the last element:" << std::endl;
    std::cout << "Is stack empty? " << (myStack.IsEmpty() ? "Yes" : "No") << std::endl;

    bool pop_fail = myStack.Pop(popped_val);
    std::cout << "Attempt to Pop from empty stack: " << (!pop_fail ? "Failed (Correct)" : "Succeeded (Incorrect)") << std::endl;

    bool gettop_fail = myStack.GetTop(top_val);
    std::cout << "Attempt to GetTop from empty stack: " << (!gettop_fail ? "Failed (Correct)" : "Succeeded (Incorrect)") << std::endl;

    // Test 6: Clear Stack
    std::cout << "\n[Test 6: Clear Stack]" << std::endl;
    std::cout << "Pushing 55, 66, 77..." << std::endl;
    myStack.Push(55);
    myStack.Push(66);
    myStack.Push(77);
    myStack.StackTraverse();
    std::cout << "Clearing the stack..." << std::endl;
    myStack.ClearStack();
    std::cout << "Is stack empty after clearing? " << (myStack.IsEmpty() ? "Yes" : "No") << std::endl;
    myStack.StackTraverse();

    std::cout << "\n--- Testbench Complete ---" << std::endl;

    return 0;
}