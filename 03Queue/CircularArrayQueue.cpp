#include <iostream>
#include <stdexcept> // For throwing exceptions

// A generic circular queue template class
template <typename T>
class CircularArrayQueue
{
private:
    T *arr;       // Pointer to the array storing queue elements
    int capacity; // Maximum capacity of the queue
    int front;    // Index of the front element
    int rear;     // Index for the next element at the rear
    int count;    // Current number of elements in the queue

public:
    // Constructor: initializes a queue of a given size
    CircularArrayQueue(int size)
    {
        capacity = size;
        arr = new T[capacity];
        front = 0;
        rear = 0;
        count = 0; // Initial element count is 0
        std::cout << "Circular queue created with capacity " << capacity << "." << std::endl;
    }

    // Destructor: frees the allocated memory
    ~CircularArrayQueue()
    {
        delete[] arr;
        std::cout << "Circular queue destroyed." << std::endl;
    }

    // Checks if the queue is empty
    bool isEmpty() const
    {
        return count == 0;
    }

    // Checks if the queue is full
    bool isFull() const
    {
        return count == capacity;
    }

    // Gets the current size of the queue
    int size() const
    {
        return count;
    }

    // Adds an element to the rear of the queue (enqueue)
    void enqueue(const T &item)
    {
        if (isFull())
        {
            std::cerr << "Error: Queue is full. Cannot enqueue " << item << "." << std::endl;
            return;
        }
        arr[rear] = item;
        // Use modulo operator for circular behavior
        rear = (rear + 1) % capacity;
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
        T item = arr[front];
        // Use modulo operator for circular behavior
        front = (front + 1) % capacity;
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
        return arr[front];
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
        for (int i = 0; i < count; i++)
        {
            // Starting from front, circularly access count elements
            int index = (front + i) % capacity;
            std::cout << arr[index] << " ";
        }
        std::cout << std::endl;
    }
};

// main function to test the CircularArrayQueue class
int main()
{
    std::cout << "--- Creating a circular queue with capacity 5 ---" << std::endl;
    CircularArrayQueue<int> q(5);

    std::cout << "\n--- Testing enqueue operation until the queue is full ---" << std::endl;
    q.enqueue(10);
    q.display();
    q.enqueue(20);
    q.display();
    q.enqueue(30);
    q.display();
    q.enqueue(40);
    q.display();
    q.enqueue(50);
    q.display();

    std::cout << "Is the queue full? " << (q.isFull() ? "Yes" : "No") << std::endl;
    q.enqueue(60); // This should fail because the queue is full

    std::cout << "\n--- Testing dequeue operation ---" << std::endl;
    q.dequeue();
    q.display();
    q.dequeue();
    q.display();

    std::cout << "\n--- Solving the 'fake overflow' issue: Enqueue again ---" << std::endl;
    std::cout << "The queue now has empty space at the front. We can enqueue again and the pointers will wrap around." << std::endl;
    q.enqueue(60);
    q.display();
    q.enqueue(70);
    q.display();

    std::cout << "Is the queue full? " << (q.isFull() ? "Yes" : "No") << std::endl;
    q.enqueue(80); // This should fail

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
