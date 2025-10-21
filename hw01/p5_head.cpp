#include <iostream>

// Define linked list node struct
struct SNode
{
    int data;
    SNode *next;
    SNode() : data(0), next(NULL) {} // constructor
};

// For convenience, define LinkList struct
struct LinkList
{
    SNode *head;
    LinkList()
    {
        head = new SNode(); // create head node
    }
};

/**
 * @brief Merge two non-decreasing ordered linked lists HA and HB (both with head nodes) into HC
 * @param HA Ordered linked list A to be merged (will be considered empty after merge)
 * @param HB Ordered linked list B to be merged (will become empty after merge)
 * @param HC Target ordered linked list to hold the merged result
 */
void mergeList(LinkList &HA, LinkList &HB, LinkList &HC)
{
    // pa and pb are working pointers for lists A and B, initially point to first data node
    SNode *pa = HA.head->next;
    SNode *pb = HB.head->next;

    // 1. Reuse HA's head node as HC's head node
    HC.head = HA.head;

    // pc is the tail pointer of HC, initially points to the head node
    SNode *pc = HC.head;

    // Ensure the merged list is initially empty
    pc->next = NULL;

    // 2. Loop to compare nodes from both lists, linking the smaller node to HC tail
    while (pa != NULL && pb != NULL)
    {
        if (pa->data <= pb->data)
        {
            // Link the node pointed by pa after pc
            pc->next = pa;
            // pc moves to new tail node
            pc = pa;
            // advance pa
            pa = pa->next;
        }
        else
        {
            // Link the node pointed by pb after pc
            pc->next = pb;
            // pc moves to new tail node
            pc = pb;
            // advance pb
            pb = pb->next;
        }
    }

    // 3. Attach the remaining part of the non-empty list to pc
    // If pa is not NULL, attach remaining A; otherwise attach remaining B
    pc->next = (pa != NULL) ? pa : pb;

    // 4. Set HB's head next to NULL to make it an empty list, and free its head node
    HB.head->next = NULL;
    delete HB.head;
    HB.head = NULL; // prevent dangling pointer
}

// Helper: print linked list (with head node)
void printList(const LinkList &L)
{
    SNode *p = L.head->next;
    if (!p)
    {
        std::cout << "List is empty." << std::endl;
        return;
    }
    while (p)
    {
        std::cout << p->data << " -> ";
        p = p->next;
    }
    std::cout << "NULL" << std::endl;
}

// Helper: create linked list (with head node) using tail insertion
void createList(LinkList &L, const int arr[], int n)
{
    SNode *tail = L.head;
    for (int i = 0; i < n; ++i)
    {
        SNode *newNode = new SNode();
        newNode->data = arr[i];
        tail->next = newNode;
        tail = newNode;
    }
}

// --- main: test ---
int main()
{
    // Create and initialize list HA: 1 -> 3 -> 5 -> 7
    LinkList HA;
    int arrA[] = {1, 3, 5, 7};
    createList(HA, arrA, sizeof(arrA) / sizeof(int));
    std::cout << "Original List HA: ";
    printList(HA);

    // Create and initialize list HB: 2 -> 4 -> 6 -> 8 -> 10
    LinkList HB;
    int arrB[] = {2, 4, 6, 8, 10};
    createList(HB, arrB, sizeof(arrB) / sizeof(int));
    std::cout << "Original List HB: ";
    printList(HB);

    // Create empty HC to receive the result
    LinkList HC;

    // Merge
    mergeList(HA, HB, HC);

    std::cout << "\nAfter merging:" << std::endl;
    std::cout << "Merged List HC:   ";
    printList(HC);

    std::cout << "List HA status:   ";
    // HA's head node was given to HC, so HA itself is no longer a valid list
    // But we can verify using HC
    if (HA.head == HC.head)
    {
        std::cout << "HA's head was reused by HC." << std::endl;
    }

    std::cout << "List HB status:   ";
    if (HB.head == NULL)
    {
        std::cout << "List is empty and head node is freed." << std::endl;
    }
    else
    {
        printList(HB);
    }

    // Free memory of HC list
    SNode *current = HC.head;
    while (current != NULL)
    {
        SNode *next = current->next;
        delete current;
        current = next;
    }

    return 0;
}