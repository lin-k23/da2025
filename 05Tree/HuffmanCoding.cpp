#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <memory> // For std::shared_ptr

// A node in the Huffman tree
struct MinHeapNode
{
    char data;                                // Character
    unsigned freq;                            // Frequency of the character
    std::shared_ptr<MinHeapNode> left, right; // Left and right children

    // Constructor
    MinHeapNode(char data, unsigned freq)
        : data(data), freq(freq), left(nullptr), right(nullptr) {}

    // Constructor for internal nodes
    MinHeapNode(unsigned freq, std::shared_ptr<MinHeapNode> l, std::shared_ptr<MinHeapNode> r)
        : data('$'), freq(freq), left(l), right(r) {} // '$' as a special value for internal nodes
};

// Comparison structure for the min-heap (priority queue)
// We want to extract the node with the *minimum* frequency
struct compare
{
    bool operator()(std::shared_ptr<MinHeapNode> l, std::shared_ptr<MinHeapNode> r)
    {
        return (l->freq > r->freq);
    }
};

// Prints Huffman codes from the root of the built tree.
void printCodes(std::shared_ptr<MinHeapNode> root, std::string str)
{
    if (!root)
        return;

    // If this is a leaf node, it contains one of the input characters.
    // Print the character and its code.
    if (root->data != '$')
    {
        std::cout << root->data << ": " << str << "\n";
    }

    // Recursively call for left and right subtrees
    // Append '0' for left branch
    printCodes(root->left, str + "0");
    // Append '1' for right branch
    printCodes(root->right, str + "1");
}

// The main function that builds a Huffman Tree and prints codes.
void buildAndPrintHuffmanTree(std::vector<char> &data, std::vector<unsigned> &freq)
{
    if (data.size() != freq.size() || data.empty())
    {
        std::cerr << "Error: Data and frequency arrays mismatch or are empty.\n";
        return;
    }

    std::shared_ptr<MinHeapNode> left, right, top;

    // Create a min-heap (priority queue)
    // std::priority_queue<Type, Container, Compare>
    std::priority_queue<std::shared_ptr<MinHeapNode>,
                        std::vector<std::shared_ptr<MinHeapNode>>,
                        compare>
        minHeap;

    // Create a leaf node for each character and add it to the min-heap.
    for (size_t i = 0; i < data.size(); ++i)
    {
        minHeap.push(std::make_shared<MinHeapNode>(data[i], freq[i]));
    }

    // Iterate while size of heap is not 1
    while (minHeap.size() > 1)
    {
        // Extract the two minimum frequency items from min heap
        left = minHeap.top();
        minHeap.pop();

        right = minHeap.top();
        minHeap.pop();

        // Create a new internal node with frequency equal to the
        // sum of the two nodes' frequencies. Make the two extracted
        // nodes as left and right children of this new node.
        // Add this node to the min heap.
        // Use '$' as a special value for internal nodes (not a character)
        top = std::make_shared<MinHeapNode>(left->freq + right->freq, left, right);
        minHeap.push(top);
    }

    // The remaining node is the root node.
    // Print the Huffman codes starting from this root.
    std::cout << "Huffman Codes:\n";
    printCodes(minHeap.top(), "");
}

// Main function to drive the program
int main()
{
    std::vector<char> arr = {'a', 'b', 'c', 'd', 'e', 'f'};
    std::vector<unsigned> freq = {5, 9, 12, 13, 16, 45};

    buildAndPrintHuffmanTree(arr, freq);

    return 0;
}
