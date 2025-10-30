#include <iostream>
#include <vector>
#include <stack>
#include <algorithm> // For std::reverse
#include <string>

// Use standard namespace for conciseness, as requested
using namespace std;

/**
 * @brief Defines the structure for a binary tree node.
 */
struct Node
{
    int data;
    Node *left;
    Node *right;

    // Constructor for easy node creation
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

/**
 * @brief Performs an iterative pre-order traversal (Node, Left, Right) using a stack.
 */
vector<int> preorderTraversal(Node *root)
{
    vector<int> result;
    if (root == nullptr)
    {
        return result;
    }

    stack<Node *> s;
    s.push(root);

    while (!s.empty())
    {
        Node *curr = s.top();
        s.pop();

        // 1. Visit the node
        result.push_back(curr->data);

        // 2. Push the right child first (so it's processed after the left)
        if (curr->right)
        {
            s.push(curr->right);
        }
        // 3. Push the left child last (so it's processed first - LIFO)
        if (curr->left)
        {
            s.push(curr->left);
        }
    }
    return result;
}

/**
 * @brief Performs an iterative in-order traversal (Left, Node, Right) using a stack.
 */
vector<int> inorderTraversal(Node *root)
{
    vector<int> result;
    stack<Node *> s;
    Node *curr = root;

    // Loop continues as long as we have nodes to process OR
    // nodes stored on the stack from a previous left-path.
    while (curr != nullptr || !s.empty())
    {

        // 1. Go all the way left, pushing nodes onto the stack
        while (curr != nullptr)
        {
            s.push(curr);
            curr = curr->left;
        }

        // At this point, curr is null. We've gone as far left as possible.

        // 2. Pop the last (leftmost) node from the stack
        curr = s.top();
        s.pop();

        // 3. Visit the node
        result.push_back(curr->data);

        // 4. Move to the right subtree to repeat the process
        curr = curr->right;
    }
    return result;
}

/**
 * @brief Performs an iterative post-order traversal (Left, Right, Node) using one stack.
 *
 * This implementation is a "trick". It performs a modified pre-order
 * (Node, Right, Left) and then reverses the result.
 */
vector<int> postorderTraversal(Node *root)
{
    vector<int> result;
    if (root == nullptr)
    {
        return result;
    }

    stack<Node *> s;
    s.push(root);

    while (!s.empty())
    {
        Node *curr = s.top();
        s.pop();

        // 1. Visit the node (add to result)
        result.push_back(curr->data);

        // 2. Push LEFT child first (so it's processed after right)
        if (curr->left)
        {
            s.push(curr->left);
        }
        // 3. Push RIGHT child last (so it's processed first - LIFO)
        if (curr->right)
        {
            s.push(curr->right);
        }
    }

    // 4. Reverse the "Node, Right, Left" traversal to get "Left, Right, Node"
    reverse(result.begin(), result.end());
    return result;
}

/**
 * @brief Helper function to print a vector with a title.
 */
void printVector(const string &title, const vector<int> &vec)
{
    cout << title << ": ";
    for (int val : vec)
    {
        cout << val << " ";
    }
    cout << endl;
}

/**
 * @brief Main function to build a tree and test the traversals.
 */
int main()
{
    /*
     * We will create the following tree:
     *
     * 1
     * / \
     * 2   3
     * / \ / \
     * 4  5 6  7
     */
    Node *root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->left = new Node(6);
    root->right->right = new Node(7);

    cout << "Tree Traversals (Iterative / Stack-based):" << endl;

    // 1. Pre-order (Node, Left, Right)
    // Expected: 1 2 4 5 3 6 7
    vector<int> pre_order = preorderTraversal(root);
    printVector("Pre-order", pre_order);

    // 2. In-order (Left, Node, Right)
    // Expected: 4 2 5 1 6 3 7
    vector<int> in_order = inorderTraversal(root);
    printVector("In-order ", in_order);

    // 3. Post-order (Left, Right, Node)
    // Expected: 4 5 2 6 7 3 1
    vector<int> post_order = postorderTraversal(root);
    printVector("Post-order", post_order);

    // Simple cleanup (this isn't robust, but fine for this example)
    delete root->left->left;
    delete root->left->right;
    delete root->right->left;
    delete root->right->right;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}
