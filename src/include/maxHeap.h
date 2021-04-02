#ifndef MAX_HEAP_H
#define MAX_HEAP_H

typedef struct MaxHeapTreeStruct
{
    int score;
    char* name;
    struct MaxHeapTreeStruct* right;
    struct MaxHeapTreeStruct* left;
}MaxHeapTreeStruct;

typedef struct MaxHeapTreeStruct* MaxHeapTree;

typedef struct QueueNodeStruct
{
    MaxHeapTree treeNode;
    struct QueueNodeStruct* next;
    
}QueueNodeStruct;

typedef QueueNodeStruct* QueueNode;

typedef struct QueueStruct
{
    QueueNode head;
    QueueNode first;
    QueueNode rear;
    int size;

}QueueStruct;

typedef QueueStruct* Queue;

//------------- MAX_HEAP_TREE -------------------//
//Function to create a complete tree node
MaxHeapTree newMaxHeapTreeNode(MaxHeapTree treeNode, char* name, int score);
//Function to insert into the tree and keep it complete
void insertToMaxHeapTree(MaxHeapTree* treeNode, MaxHeapTree temporaryNode, Queue queue);
//Function to print the tree in PreOrder traversal
void printPreOrderMaxHeap(MaxHeapTree treeRoot);
//Function to free memory allocated for the tree
void freeMaxHeapTree(MaxHeapTree treeNode);
//Function to deside if a tree node has both left and right child
int hasBothChild(MaxHeapTree treeNode);
//Function for levelOrder tree traversal. And get last node and last node's parent
void levelOrder(MaxHeapTree root, MaxHeapTree* result, MaxHeapTree* resultsParent, int* childSide);
//Function to heapify the tree in PreOrder traversal
void preorder(MaxHeapTree treeRoot);
//Function to max heapify a complete tree
void maxHeapify(MaxHeapTree treeRoot, MaxHeapTree previous);
//Function used to swap tree's two nodes
void swapNodes(MaxHeapTree p1, MaxHeapTree p2);
//Function to pop the tree root and do preperation for node's deletion
MaxHeapTree popTreeRoot(MaxHeapTree treeRoot, MaxHeapTree lastNode, MaxHeapTree lastNodeParent, int side);
//Function to delete node's memory allocated
void deleteRoot(MaxHeapTree treeNode);


// -------------    QUEUE   -----------------------//
//Function to initalize a Queue List new node
QueueNode newNode(MaxHeapTree treeNode);
//Function to initialize a Queue List
Queue createQueue();
//Function to check if queue is empty
int isEmpty(Queue queue);
//Function to add a node into queue
void enQueue(Queue queue, MaxHeapTree treeNode);
//Function to pop a node from queue
MaxHeapTree deQueue(Queue queue);
//Function to print the queue
void printQueue(QueueNode queueFront);
//Function to free memory allocated from queue
void freeQueue(Queue queue);
//Check if queue has only one item
int hasOnlyOneItem(Queue queue);
//Get the tree node that first node of queue contains
MaxHeapTree getFront(Queue queue);

//-------------------  QUEUE_NODE -----------------//
//Function to initialize a queue node
void initializeQueueNode(QueueNode queueNode, MaxHeapTree treeNode);

#endif