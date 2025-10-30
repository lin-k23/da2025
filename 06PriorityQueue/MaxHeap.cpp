#include <iostream>
#include <vector>
#include <stdexcept> // For exceptions
#include <algorithm> // For std::swap

class MaxHeap
{
private:
    std::vector<int> heap;

    // Helper functions for indices
    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    /**
     * Corresponds to FixUp or Heapify-up[cite: 1550, 1683].
     * Moves a node up the tree to maintain the heap property.
     */
    void heapifyUp(int i)
    {
        // While node i is not the root and is greater than its parent [cite: 1686]
        while (i > 0 && heap[i] > heap[parent(i)])
        {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    /**
     * Corresponds to FixDown or Heapify-down[cite: 1439, 1531].
     * Moves a node down the tree to maintain the heap property.
     */
    void heapifyDown(int i)
    {
        int maxIndex = i;
        int l = left(i);
        int r = right(i);

        // Find the largest among the node, its left child, and its right child
        if (l < heap.size() && heap[l] > heap[maxIndex])
        {
            maxIndex = l;
        }
        if (r < heap.size() && heap[r] > heap[maxIndex])
        {
            maxIndex = r;
        }

        // If the largest is not the current node, swap and recurse [cite: 1537-1541]
        if (i != maxIndex)
        {
            std::swap(heap[i], heap[maxIndex]);
            heapifyDown(maxIndex);
        }
    }

    /**
     * Builds the heap from an arbitrary array in O(N) time.
     * This is the "Bottom-up Construction" method[cite: 2015, 2412].
     */
    void buildHeap()
    {
        // Start from the last non-leaf node and heapify down [cite: 2042-2044]
        for (int i = (heap.size() / 2) - 1; i >= 0; i--)
        {
            heapifyDown(i);
        }
    }

public:
    // Constructor for building heap from an existing vector
    MaxHeap(const std::vector<int> &elements)
    {
        heap = elements; // Copy elements
        buildHeap();     // Build the heap
    }

    // Default constructor for an empty heap
    MaxHeap() {}

    /**
     * Inserts a new element into the heap[cite: 1702].
     */
    void insert(int e)
    {
        heap.push_back(e);          // Add element to the end [cite: 1707]
        heapifyUp(heap.size() - 1); // Fix the heap property [cite: 1709]
    }

    /**
     * Gets the max element (root) without removing it[cite: 2461].
     */
    int getMax() const
    {
        if (isEmpty())
        {
            throw std::out_of_range("Heap is empty");
        }
        return heap[0]; // Root element has highest priority [cite: 2464]
    }

    /**
     * Removes and returns the max element (root)[cite: 1765].
     */
    int extractMax()
    {
        if (isEmpty())
        {
            throw std::out_of_range("Heap is empty");
        }

        int maxElement = heap[0];
        // Move the last element to the root [cite: 1767, 1799]
        heap[0] = heap.back();
        heap.pop_back();

        // Restore the heap property from the root [cite: 1767, 1801]
        heapifyDown(0);

        return maxElement;
    }

    bool isEmpty() const
    {
        return heap.empty();
    }

    int getSize() const
    {
        return heap.size();
    }

    void printHeap() const
    {
        for (int val : heap)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

int main()
{
    // Test 1: Bottom-up construction (from an array)
    // Uses the O(N) buildHeap() constructor
    std::cout << "## Test 1: Bottom-up Heap Construction ##" << std::endl;
    std::vector<int> elements = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    // This example array is from the slides [cite: 1869]
    MaxHeap heap1(elements);

    std::cout << "Initial heap array (after buildHeap): ";
    heap1.printHeap(); // Should be {16, 14, 10, 8, 7, 9, 3, 2, 4, 1} [cite: 2363, 2372]

    std::cout << "Extracting elements (Heap Sort):" << std::endl;
    while (!heap1.isEmpty())
    {
        std::cout << heap1.extractMax() << " ";
    }
    std::cout << "\n-----------------------------------" << std::endl;

    // Test 2: Top-down construction (inserting one by one)
    // Uses the O(N log N) insert() method
    std::cout << "## Test 2: Top-down Heap Construction (via insert) ##" << std::endl;
    MaxHeap heap2;
    std::vector<int> elements_to_insert = {10, 20, 15, 5, 50, 2};

    std::cout << "Inserting elements: 10 20 15 5 50 2" << std::endl;
    for (int el : elements_to_insert)
    {
        heap2.insert(el);
    }

    std::cout << "Heap array after insertions: ";
    heap2.printHeap();

    std::cout << "Max element is: " << heap2.getMax() << std::endl;

    std::cout << "Extracting elements:" << std::endl;
    while (!heap2.isEmpty())
    {
        std::cout << heap2.extractMax() << " ";
    }
    std::cout << "\n-----------------------------------" << std::endl;

    return 0;
}