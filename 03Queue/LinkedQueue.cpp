#include <iostream>
#include <stdexcept> // For throwing exceptions

// A generic Node for the linked list
template <typename T>
struct Node
{
    T data;
    Node *next;

    // Node constructor
    Node(const T &val) : data(val), next(nullptr) {}
};

// A generic linked list based queue template class
template <typename T>
class LinkedQueue
{
private:
    Node<T> *front; // Pointer to the front of the queue
    Node<T> *rear;  // Pointer to the rear of the queue
    int count;      // Current number of elements in the queue

public:
    // Constructor: initializes an empty queue
    LinkedQueue() : front(nullptr), rear(nullptr), count(0)
    {
        std::cout << "Linked queue created." << std::endl;
    }

    // Destructor: frees all allocated memory
    ~LinkedQueue()
    {
        while (!isEmpty())
        {
            dequeue(); // Dequeue all elements to free memory
        }
        std::cout << "Linked queue destroyed." << std::endl;
    }

    // Checks if the queue is empty
    bool isEmpty() const
    {
        return front == nullptr;
    }

    // Gets the current size of the queue
    int size() const
    {
        return count;
    }

    // Adds an element to the rear of the queue (enqueue)
    void enqueue(const T &item)
    {
        Node<T> *newNode = new Node<T>(item);
        if (isEmpty())
        {
            front = rear = newNode; // The new node is both front and rear
        }
        else
        {
            rear->next = newNode; // Link the old rear to the new node
            rear = newNode;       // Update the rear pointer
        }
        count++;
        std::cout << item << " has been enqueued." << std::endl;
    }

    // Removes an element from the front of the queue (dequeue)
    T dequeue()
    {
        if (isEmpty())
        {
            throw std::runtime_error("Error: Queue is empty. Cannot dequeue.");
        }

        Node<T> *temp = front; // Temporarily store the front node
        T item = front->data;  // Get the data from the front node
        front = front->next;   // Move front to the next node

        // If the queue becomes empty after dequeue, update rear as well
        if (front == nullptr)
        {
            rear = nullptr;
        }

        delete temp; // Free the memory of the old front node
        count--;
        std::cout << item << " has been dequeued." << std::endl;
        return item;
    }

    // Gets the front element without removing it
    T getFront() const
    {
        if (isEmpty())
        {
            throw std::runtime_error("Error: Queue is empty.");
        }
        return front->data;
    }

    // Displays all elements in the queue
    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Queue is empty." << std::endl;
            return;
        }
        std::cout << "Queue elements (from front to rear): ";
        Node<T> *current = front;
        while (current != nullptr)
        {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

// main function to test the LinkedQueue class
int main()
{
    std::cout << "--- Creating a linked queue ---" << std::endl;
    LinkedQueue<int> q;

    std::cout << "\n--- Testing enqueue operation ---" << std::endl;
    q.enqueue(10);
    q.display();
    q.enqueue(20);
    q.display();
    q.enqueue(30);
    q.display();

    std::cout << "Current queue size: " << q.size() << std::endl;

    std::cout << "\n--- Testing dequeue operation ---" << std::endl;
    q.dequeue();
    q.display();

    try
    {
        std::cout << "Front element is now: " << q.getFront() << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "\n--- Enqueueing more elements ---" << std::endl;
    q.enqueue(40);
    q.display();
    q.enqueue(50);
    q.display();

    std::cout << "Current queue size: " << q.size() << std::endl;

    std::cout << "\n--- Dequeueing all elements ---" << std::endl;
    try
    {
        while (!q.isEmpty())
        {
            q.dequeue();
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    q.display();

    std::cout << "\n--- Testing dequeue on an empty queue ---" << std::endl;
    try
    {
        q.dequeue(); // This will throw an exception
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
