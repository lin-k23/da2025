#include <iostream>
#include <stdexcept> // Used for throwing exceptions

// A template class for a generic Queue
template <typename T>
class ArrayQueue
{
private:
    T *arr;       // Pointer to the array that stores queue elements
    int capacity; // The maximum capacity of the queue
    int front;    // Index of the front element
    int rear;     // Index for the next element to be inserted at the rear

public:
    // Constructor: Initializes the queue with a given size
    ArrayQueue(int size)
    {
        capacity = size;
        arr = new T[capacity];
        front = 0;
        rear = 0;
        std::cout << "Queue of size " << capacity << " created." << std::endl;
    }

    // Destructor: Frees the allocated memory
    ~ArrayQueue()
    {
        delete[] arr;
        std::cout << "Queue destroyed." << std::endl;
    }

    // Function to check if the queue is empty
    // The queue is empty when front and rear pointers are at the same position.
    bool isEmpty() const
    {
        return front == rear;
    }

    // Function to check if the queue is full
    // In this simple implementation, the queue is full when the rear pointer reaches the capacity.
    // This can lead to a "fake overflow" issue.
    bool isFull() const
    {
        return rear == capacity;
    }

    // Function to add an element to the rear of the queue
    void enqueue(const T &item)
    {
        if (isFull())
        {
            std::cerr << "Error: Queue is full. Cannot enqueue " << item << "." << std::endl;
            return;
        }
        arr[rear] = item; // Add item to the rear
        rear++;           // Increment rear pointer
        std::cout << item << " enqueued to queue." << std::endl;
    }

    // Function to remove an element from the front of the queue
    T dequeue()
    {
        if (isEmpty())
        {
            // Throwing an exception is a good way to handle errors like this.
            throw std::runtime_error("Error: Queue is empty. Cannot dequeue.");
        }
        T item = arr[front]; // Get the front element
        front++;             // Increment front pointer
        std::cout << item << " dequeued from queue." << std::endl;
        return item;
    }

    // Function to get the front element without removing it
    T getFront() const
    {
        if (isEmpty())
        {
            throw std::runtime_error("Error: Queue is empty.");
        }
        return arr[front];
    }

    // Function to display the current elements in the queue
    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Queue is empty." << std::endl;
            return;
        }
        std::cout << "Queue elements (front to rear): ";
        for (int i = front; i < rear; i++)
        {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }
};

// Main function to test the ArrayQueue class
int main()
{
    std::cout << "--- Creating a queue of size 5 ---" << std::endl;
    ArrayQueue<int> q(5);

    std::cout << "\n--- Testing Basic Operations ---" << std::endl;
    std::cout << "Is queue empty? " << (q.isEmpty() ? "Yes" : "No") << std::endl;

    q.enqueue(10);
    q.display();
    q.enqueue(20);
    q.display();
    q.enqueue(30);
    q.display();

    std::cout << "\nIs queue empty? " << (q.isEmpty() ? "Yes" : "No") << std::endl;

    try
    {
        std::cout << "Front element is: " << q.getFront() << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }

    q.dequeue();
    q.display();

    try
    {
        std::cout << "New front element is: " << q.getFront() << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "\n--- Testing Full and Fake Overflow Condition ---" << std::endl;
    q.enqueue(40);
    q.enqueue(50);
    q.enqueue(60); // This should fail as the queue is full
    q.display();
    std::cout << "Is queue full? " << (q.isFull() ? "Yes" : "No") << std::endl;

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

    std::cout << "\n--- Demonstrating Fake Overflow ---" << std::endl;
    std::cout << "Even though the queue is empty, we cannot add new elements because the rear pointer is at the end." << std::endl;
    q.enqueue(70); // This will fail, demonstrating the limitation.

    std::cout << "\n--- Testing Dequeue on Empty Queue ---" << std::endl;
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
