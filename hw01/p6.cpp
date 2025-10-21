#include <queue>
#include <stack>
#include <iostream>

void f(std::queue<int> &q)
{
    std::stack<int> s; // Initialize a stack
    int d;             // Temporary variable to hold the front element of the queue
    while (!q.empty())
    {
        d = q.front(); // Get the front element of the queue
        q.pop();       // Remove the front element from the queue
        s.push(d);     // Push the element onto the stack
    }
    while (!s.empty())
    {
        d = s.top(); // Get the top element of the stack
        s.pop();     // Remove the top element from the stack
        q.push(d);   // Push the element back onto the queue
    }
}

// test for f
int main()
{
    std::queue<int> q;
    for (int i = 1; i <= 5; ++i)
        q.push(i);
    f(q);
    while (!q.empty())
    {
        std::cout << q.front() << " ";
        q.pop();
    }
    return 0;
}