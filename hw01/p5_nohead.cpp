#include <iostream>

// Define the structure for a singly linked list node
struct SNode
{
    int data;
    SNode *next;
    SNode() : data(0), next(NULL) {} // Constructor
};

/**
 * @brief Merges two sorted (non-decreasing) linked lists, HA and HB, which do not have head nodes.
 * @param HA A reference to the head pointer of the first list. Becomes NULL after the merge.
 * @param HB A reference to the head pointer of the second list. Becomes NULL after the merge.
 * @return SNode* The head pointer of the newly merged sorted list.
 */
SNode *mergeList_noHead(SNode *&HA, SNode *&HB)
{
    // 1. Handle edge cases where one of the lists is empty.
    if (HA == NULL)
    {
        SNode *result = HB;
        HB = NULL; // Set the original pointer to NULL
        return result;
    }
    if (HB == NULL)
    {
        SNode *result = HA;
        HA = NULL; // Set the original pointer to NULL
        return result;
    }

    // 2. Determine the head of the new list (HC) and initialize the tail pointer.
    SNode *HC = NULL;
    SNode *tail = NULL;

    if (HA->data <= HB->data)
    {
        HC = HA;
        tail = HA;
        HA = HA->next; // Move the head pointer of HA forward
    }
    else
    {
        HC = HB;
        tail = HB;
        HB = HB->next; // Move the head pointer of HB forward
    }

    // 3. Loop through both lists and append the smaller node to the tail.
    while (HA != NULL && HB != NULL)
    {
        if (HA->data <= HB->data)
        {
            tail->next = HA; // Append the current node of HA to the tail
            tail = HA;       // Update the tail pointer
            HA = HA->next;   // Move HA's pointer to the next node
        }
        else
        {
            tail->next = HB; // Append the current node of HB to the tail
            tail = HB;       // Update the tail pointer
            HB = HB->next;   // Move HB's pointer to the next node
        }
    }

    // 4. Append the remaining nodes from the non-empty list.
    if (HA != NULL)
    {
        tail->next = HA;
    }
    else
    {
        tail->next = HB;
    }

    // 5. Nullify the original head pointers to make them empty.
    // Since the parameters are references, HA and HB have been modified throughout
    // the process. By the end, all nodes have been moved, so we set them to NULL.
    HA = NULL;
    HB = NULL;

    return HC;
}

// Helper function: Prints a list (without a head node).
void printList_noHead(SNode *head)
{
    if (head == NULL)
    {
        std::cout << "List is empty." << std::endl;
        return;
    }
    SNode *p = head;
    while (p)
    {
        std::cout << p->data << " -> ";
        p = p->next;
    }
    std::cout << "NULL" << std::endl;
}

// Helper function: Creates a list from an array (without a head node).
SNode *createList_noHead(const int arr[], int n)
{
    if (n == 0)
        return NULL;
    SNode *head = new SNode();
    head->data = arr[0];
    SNode *tail = head;
    for (int i = 1; i < n; ++i)
    {
        SNode *newNode = new SNode();
        newNode->data = arr[i];
        tail->next = newNode;
        tail = newNode;
    }
    return head;
}

// --- Main function for testing ---
int main()
{
    // Create and initialize list HA: 1 -> 3 -> 5 -> 7
    int arrA[] = {1, 3, 5, 7};
    SNode *HA = createList_noHead(arrA, sizeof(arrA) / sizeof(int));
    std::cout << "Original List HA: ";
    printList_noHead(HA);

    // Create and initialize list HB: 2 -> 4 -> 6 -> 8 -> 10
    int arrB[] = {2, 4, 6, 8, 10};
    SNode *HB = createList_noHead(arrB, sizeof(arrB) / sizeof(int));
    std::cout << "Original List HB: ";
    printList_noHead(HB);

    // Merge the lists
    SNode *HC = mergeList_noHead(HA, HB);

    std::cout << "\nAfter merging:" << std::endl;
    std::cout << "Merged List HC:   ";
    printList_noHead(HC);

    std::cout << "List HA status:   ";
    printList_noHead(HA); // Should be empty

    std::cout << "List HB status:   ";
    printList_noHead(HB); // Should be empty

    // Free the memory allocated for the merged list HC
    SNode *current = HC;
    while (current != NULL)
    {
        SNode *next = current->next;
        delete current;
        current = next;
    }

    return 0;
}