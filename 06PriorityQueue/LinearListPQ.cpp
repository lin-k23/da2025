#include <iostream>
#include <vector> // For sequential lists
#include <list>   // For linked lists
#include <string>
#include <stdexcept>  // For exceptions
#include <algorithm>  // For std::min_element, std::lower_bound, std::swap
#include <functional> // For std::greater
#include <cassert>    // For testing

// ---
// 1. Unordered Sequential List (std::vector)
// ---
// * insert(val): O(1) - Add to end
// * extractMin(): O(N) - O(N) to find min, O(1) to swap-and-pop
// ---
struct PQ_UnorderedVector
{
    std::vector<int> data;

    bool isEmpty() const
    {
        return data.empty();
    }

    void insert(int value)
    {
        data.push_back(value); // O(1)
    }

    int peekMin()
    {
        if (isEmpty())
            throw std::runtime_error("Queue is empty");
        // O(N) to find
        return *std::min_element(data.begin(), data.end());
    }

    int extractMin()
    {
        if (isEmpty())
            throw std::runtime_error("Queue is empty");

        // O(N) to find
        auto minIt = std::min_element(data.begin(), data.end());
        int minVal = *minIt;

        // Optimized removal: Swap min with last element and pop.
        // This avoids an O(N) shift that data.erase(minIt) would cause.
        std::swap(*minIt, data.back()); // O(1)
        data.pop_back();                // O(1)

        return minVal;
    }
};

// ---
// 2. Ordered Sequential List (std::vector)
// ---
// Stored in *descending* order to make extractMin() O(1).
// * insert(val): O(N) - O(log N) to find, O(N) to shift
// * extractMin(): O(1) - Just pop from the back
// ---
struct PQ_OrderedVector
{
    std::vector<int> data; // Sorted descending

    bool isEmpty() const
    {
        return data.empty();
    }

    void insert(int value)
    {
        // Find first element *less than* value
        // O(log N) binary search
        auto it = std::lower_bound(data.begin(), data.end(), value, std::greater<int>());

        // O(N) shift to insert
        data.insert(it, value);
    }

    int peekMin()
    {
        if (isEmpty())
            throw std::runtime_error("Queue is empty");
        return data.back(); // O(1)
    }

    int extractMin()
    {
        if (isEmpty())
            throw std::runtime_error("Queue is empty");

        int minVal = data.back(); // O(1)
        data.pop_back();          // O(1)
        return minVal;
    }
};

// ---
// 3. Unordered Linked List (std::list)
// ---
// * insert(val): O(1) - Add to front
// * extractMin(): O(N) - O(N) to find, O(1) to erase
// ---
struct PQ_UnorderedList
{
    std::list<int> data;

    bool isEmpty() const
    {
        return data.empty();
    }

    void insert(int value)
    {
        data.push_front(value); // O(1)
    }

    int peekMin()
    {
        if (isEmpty())
            throw std::runtime_error("Queue is empty");
        // O(N) traversal
        return *std::min_element(data.begin(), data.end());
    }

    int extractMin()
    {
        if (isEmpty())
            throw std::runtime_error("Queue is empty");

        // O(N) traversal to find
        auto minIt = std::min_element(data.begin(), data.end());
        int minVal = *minIt;

        // O(1) removal once iterator is found
        data.erase(minIt);
        return minVal;
    }
};

// ---
// 4. Ordered Linked List (std::list)
// ---
// Stored in *ascending* order.
// * insert(val): O(N) - O(N) to find insertion point
// * extractMin(): O(1) - Just pop from the front
// ---
struct PQ_OrderedList
{
    std::list<int> data; // Sorted ascending

    bool isEmpty() const
    {
        return data.empty();
    }

    void insert(int value)
    {
        // O(N) traversal to find insertion point
        auto it = std::lower_bound(data.begin(), data.end(), value);

        // O(1) insertion
        data.insert(it, value);
    }

    int peekMin()
    {
        if (isEmpty())
            throw std::runtime_error("Queue is empty");
        return data.front(); // O(1)
    }

    int extractMin()
    {
        if (isEmpty())
            throw std::runtime_error("Queue is empty");

        int minVal = data.front(); // O(1)
        data.pop_front();          // O(1)
        return minVal;
    }
};

// ---
// Test Harness
// ---
// A generic test function that works with any of the PQ structs.
template <typename PQ>
void testPriorityQueue(const std::string &type)
{
    std::cout << "--- Testing: " << type << " ---" << std::endl;
    PQ pq;

    // Test 1: Initial state
    assert(pq.isEmpty());

    // Test 2: Insertions
    pq.insert(5);
    pq.insert(1);
    pq.insert(3);

    // Test 3: Peek and non-empty
    assert(!pq.isEmpty());
    assert(pq.peekMin() == 1); // Min is 1

    // Test 4: Extraction
    assert(pq.extractMin() == 1);
    assert(pq.peekMin() == 3);
    assert(pq.extractMin() == 3);

    // Test 5: More insertions
    pq.insert(2);
    pq.insert(0);

    // Test 6: Final extraction order
    assert(pq.peekMin() == 0);
    assert(pq.extractMin() == 0);
    assert(pq.extractMin() == 2);
    assert(pq.peekMin() == 5);
    assert(pq.extractMin() == 5);

    // Test 7: Back to empty
    assert(pq.isEmpty());

    // Test 8: Exception handling
    bool caught = false;
    try
    {
        pq.extractMin();
    }
    catch (const std::runtime_error &e)
    {
        caught = true;
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    assert(caught);

    std::cout << "PASS: " << type << std::endl;
}

// ---
// Main Function
// ---
int main()
{
    testPriorityQueue<PQ_UnorderedVector>("Unordered Vector");
    std::cout << std::endl;

    testPriorityQueue<PQ_OrderedVector>("Ordered Vector");
    std::cout << std::endl;

    testPriorityQueue<PQ_UnorderedList>("Unordered List");
    std::cout << std::endl;

    testPriorityQueue<PQ_OrderedList>("Ordered List");
    std::cout << std::endl;

    std::cout << "All priority queue tests passed!" << std::endl;
    return 0;
}