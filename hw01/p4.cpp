#include <iostream>

struct SNode
{
    int data;
    SNode *next;
    SNode() : data(0), next(nullptr) {}
};

struct SNode *FindNode(SNode *&HA)
{
    // no node or only one node
    if (HA == nullptr || HA->next == nullptr)
        return nullptr;

    SNode *first = HA;
    SNode *second = HA;

    // Move first pointer to the end
    // second pointer stays at the second last node
    while (first->next != nullptr)
    {
        first = first->next;
        if (first->next != nullptr)
        {
            second = second->next;
        }
    }
    return second;
}

// test
int main()
{
    SNode *head = new SNode();
    head->data = 1;
    head->next = new SNode();
    head->next->data = 2;
    head->next->next = new SNode();
    head->next->next->data = 3;
    head->next->next->next = new SNode();
    head->next->next->next->data = 4;

    SNode *result = FindNode(head);
    if (result != nullptr)
    {
        std::cout << "The second last node's value is: " << result->data << std::endl;
    }
    else
    {
        std::cout << "The second last node does not exist." << std::endl;
    }

    // Clean up memory
    SNode *current = head;
    SNode *temp_next;
    while (current != nullptr)
    {
        temp_next = current->next;
        delete current;
        current = temp_next;
    }
    head = nullptr;
    std::cout << "Memory cleanup complete." << std::endl;

    // main 的总体复杂度：时间 O(n)，空间 O(1)（不计程序输入输出与栈帧）。
    return 0;
}
