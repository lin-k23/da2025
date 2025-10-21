#include <iostream>

const int STACK_MAX_SIZE = 100;

class SqStack
{
private:
    int data[STACK_MAX_SIZE]; // Storage for stack elements
    int top_index;            // Represents the current length and next empty slot

public:
    SqStack();
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
SqStack::SqStack()
{
    top_index = 0;
}

// ClearStack: Resets the stack to be empty.
void SqStack::ClearStack()
{
    top_index = 0;
}

// IsEmpty: Checks if the stack is empty.
bool SqStack::IsEmpty()
{
    return top_index == 0;
}

// StackLength: Returns the current number of elements.
int SqStack::StackLength()
{
    return top_index;
}

// GetTop: Retrieves the top element of the stack into 'e'.
// Returns false if the stack is empty, true otherwise.
bool SqStack::GetTop(int &e)
{
    if (IsEmpty())
    {
        return false; // Stack is empty
    }
    e = data[top_index - 1];
    return true;
}

// Push: Inserts element 'e' at the top of the stack.
// This operation has a time complexity of O(1)
// Returns false if the stack is full (overflow), true otherwise.
bool SqStack::Push(int e)
{
    if (top_index >= STACK_MAX_SIZE)
    {
        return false; // Stack overflow
    }
    data[top_index] = e;
    top_index++;
    return true;
}

// Pop: Deletes the top element of the stack and returns it in 'e'.
// This operation has a time complexity of O(1)
// Returns false if the stack is empty, true otherwise.
bool SqStack::Pop(int &e)
{
    if (IsEmpty())
    {
        return false; // Stack is empty
    }
    top_index--;
    e = data[top_index];
    return true;
}

// StackTraverse: Prints all elements in the stack from bottom to top.
void SqStack::StackTraverse()
{
    std::cout << "Stack (bottom -> top): ";
    for (int i = 0; i < top_index; ++i)
    {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

// --- Main function acting as a Testbench ---

int main()
{
    std::cout << "--- Initializing SequentialStack Testbench ---" << std::endl;
    SqStack myStack;

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
    myStack.StackTraverse();
    std::cout << "Length after GetTop: " << myStack.StackLength() << std::endl;

    // Test 4: Pop operation
    std::cout << "\n[Test 4: Pop Operation]" << std::endl;
    int popped_val;
    if (myStack.Pop(popped_val))
    {
        std::cout << "Popped value: " << popped_val << std::endl;
    }
    myStack.StackTraverse();
    std::cout << "Current length: " << myStack.StackLength() << std::endl;
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
    std::cout << "Pushing 55, 66..." << std::endl;
    myStack.Push(55);
    myStack.Push(66);
    myStack.StackTraverse();
    std::cout << "Clearing the stack..." << std::endl;
    myStack.ClearStack();
    std::cout << "Is stack empty after clearing? " << (myStack.IsEmpty() ? "Yes" : "No") << std::endl;
    myStack.StackTraverse();

    std::cout << "\n--- Testbench Complete ---" << std::endl;

    return 0;
}