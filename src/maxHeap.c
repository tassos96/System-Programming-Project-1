/**
 * This file contains methods for Max Heap used in topK commands.
 * Contains method for: 
 *          - Max Heap Tree
 *          - Queue implemented as linked list
 * 
 * Idea for complete tree implementation algorithm taken from:
 * "https://www.geeksforgeeks.org/linked-complete-binary-tree-its-creation/"
 * 
 * Idea for max heapify algorithm taken from:
 * "https://stackoverflow.com/questions/24801613/max-heapify-a-binary-tree"
**/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/maxHeap.h"

#define TRUE 1
#define FALSE 0

//------------- MAX_HEAP_TREE -------------------//

//Function to create a complete tree node
MaxHeapTree newMaxHeapTreeNode(MaxHeapTree treeNode, char* name, int score)
{
    treeNode = malloc(sizeof(MaxHeapTreeStruct));

    treeNode->score = score;
    treeNode->name = malloc(sizeof(char*) * strlen(name) + 1);
    strcpy(treeNode->name, name);

    treeNode->left = NULL;
    treeNode->right = NULL;

    return treeNode;
}

//Function to insert into the tree and keep it complete
void insertToMaxHeapTree(MaxHeapTree* treeRoot, MaxHeapTree temporaryNode, Queue queue)
{
    MaxHeapTree temporaryTreeNode = temporaryNode;
    
    if((*treeRoot) == NULL) 
    {
        *treeRoot = temporaryTreeNode;
    }
    else
    {
        MaxHeapTree front = getFront(queue);

        if (front->left == NULL) //if left child not exist's
        {
            front->left = temporaryTreeNode;
        }
        else if(front->right == NULL) //if right child not exist's
        {
            front->right = temporaryTreeNode;
        }
        
        if(hasBothChild(front)) //if both childs exists
        {
            deQueue(queue);    
        }
    }

    enQueue(queue, temporaryTreeNode);
}

//Function for levelOrder tree traversal. And get last node and last node's parent
//result: Pointer to the last node of the tree
//resultsParent: Pointer to the last node parent
//childSide: Variable that contains the information on which side of the parent node is the child node (Left 1/Right 0)
void levelOrder(MaxHeapTree root, MaxHeapTree* result, MaxHeapTree* resultsParent, int* childSide)  
{  
    Queue queue = createQueue();

    MaxHeapTree temp = root;

    while (temp != NULL)  
    {    
        *result = temp;  

        if (temp->left != NULL)
        {  
            enQueue(queue, temp->left);
            *resultsParent = temp;
            *childSide = 1;
        }
        if (temp->right != NULL)
        {  
            enQueue(queue, temp->right);
           *resultsParent = temp;
           *childSide = 0;
        }
        temp = deQueue(queue);  

    }
    
    freeQueue(queue);
    free(queue);
}

//Function to print the tree in PreOrder traversal
void printPreOrderMaxHeap(MaxHeapTree treeRoot)
{
    if (treeRoot != NULL)
    {
        printf("%s: %d\n", treeRoot->name, treeRoot->score);
        printPreOrderMaxHeap(treeRoot->left);
        printPreOrderMaxHeap(treeRoot->right);
    }
    
}

//Function to free memory allocated for the tree
void freeMaxHeapTree(MaxHeapTree treeNode)
{
    MaxHeapTree temporary = treeNode;
    if(temporary != NULL)
    {
        freeMaxHeapTree(temporary->left);
        freeMaxHeapTree(temporary->right);
        free(temporary->name);
        free(temporary);
    }
}

//Function to deside if a tree node has both left and right child
int hasBothChild(MaxHeapTree treeNode)
{
    return treeNode && treeNode->left && treeNode->right;
}

//Function to heapify the tree in PreOrder traversal
void preorder(MaxHeapTree treeRoot)
{
    if(treeRoot == NULL)
    {
        return;
    }
    maxHeapify(treeRoot, NULL);
    preorder(treeRoot->left);
    preorder(treeRoot->right);
}

//Function to pop the tree root and do preperation for node's deletion
MaxHeapTree popTreeRoot(MaxHeapTree treeRoot, MaxHeapTree lastNode, MaxHeapTree lastNodeParent, int side)
{

    swapNodes(treeRoot, lastNode);
    if(side == 1) //Left side
    {
        lastNodeParent->left = NULL;
    }
    else    //Right side
    {
        lastNodeParent->right = NULL;
    }

    return lastNode;
}

//Function to delete node's memory allocated
void deleteRoot(MaxHeapTree treeNode)
{
    free(treeNode->name);
    free(treeNode);
}

//Function to max heapify a complete tree
void maxHeapify(MaxHeapTree treeRoot, MaxHeapTree previous)
{
    if(treeRoot == NULL)
    {
        return;
    }
    MaxHeapTree leftChild;
    MaxHeapTree rightChild;

    leftChild = treeRoot->left;
    rightChild = treeRoot->right;

    maxHeapify(leftChild, treeRoot);
    maxHeapify(rightChild, treeRoot);

    if (previous != NULL && treeRoot->score >= previous->score) //TODO: CHECK =
    {
        swapNodes(treeRoot, previous);
    }
}

//Function used to swap tree's two nodes
void swapNodes(MaxHeapTree p1, MaxHeapTree p2)
{
    char* tempName = NULL;
    char* p1_name = NULL;
    char* p2_name = NULL;
    p1_name = malloc(sizeof(char*) * strlen(p1->name)+ 1);
    strcpy(p1_name, p1->name);

    p2_name = malloc(sizeof(char*) * strlen(p2->name) + 1);
    strcpy(p2_name, p2->name);

    free(p1->name);
    p1->name = malloc(sizeof(char*) * strlen(p2_name) +1);

    free(p2->name);
    p2->name = malloc(sizeof(char*) * strlen(p1_name) + 1);

    int tempScore = -1;

    tempScore = p1->score;
    tempName = malloc(sizeof(char*) * strlen(p1_name) + 1);
    strcpy(tempName, p1_name);

    p1->score = p2->score;
    strcpy(p1->name, p2_name);

    p2->score = tempScore;
    strcpy(p2->name, tempName);

    free(tempName);
    free(p1_name);
    free(p2_name);
}
// -------------    QUEUE   -----------------------//

//Function to initalize a Queue List new node
QueueNode newNode(MaxHeapTree treeNode)
{
    QueueNode temporaryNode = malloc(sizeof(QueueNodeStruct));
    temporaryNode->treeNode = NULL;
    temporaryNode->next = NULL;

    initializeQueueNode(temporaryNode, treeNode);
    
    return temporaryNode;
}

//Function to initialize a Queue List
Queue createQueue()
{
    Queue queue = malloc(sizeof(QueueStruct));
    queue->head = NULL;
    queue->first = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

//Function to check if queue is empty
int isEmpty(Queue queue)
{
    return (queue->size == 0);
}

//Function to add a node into queue
void enQueue(Queue queue, MaxHeapTree treeNode)
{
    QueueNode temporaryNode = newNode(treeNode);

    if(isEmpty(queue) == TRUE)
    {
        queue->head = temporaryNode;
        queue->first = temporaryNode; 
        queue->rear = temporaryNode;
        queue->size++;
        return;
    }

    queue->rear->next = temporaryNode;
    queue->rear = temporaryNode;
    queue->size++;
}

//Function to pop a node from queue
MaxHeapTree deQueue(Queue queue)
{
    if(isEmpty(queue) == TRUE)
    {
        return NULL;
    }
    
    MaxHeapTree treeNodeToReturn;   
    treeNodeToReturn = queue->first->treeNode;
    
    if (queue->size == 1)
    {
        queue->first = queue->rear;
        queue->size--;
    }
    else
    {
        queue->first = queue->first->next;
        queue->size--;    
    }
    
    return (treeNodeToReturn);
}

//Function to print the queue
void printQueue(QueueNode queueFront)
{
    while (queueFront != NULL)
    {
        printf("Name: %s Score: %d\n", queueFront->treeNode->name, queueFront->treeNode->score);
        queueFront = queueFront->next;
    }
}

//Function to free memory allocated from queue
void freeQueue(Queue queue)
{
    QueueNode tempNode;
    while (queue->head != NULL)
    {
        tempNode = queue->head;
        queue->head = queue->head->next;
        free(tempNode);
    }    
}

//Check if queue has only one item
int hasOnlyOneItem(Queue queue)
{
    return(queue->first == queue->rear);
}

//Get the tree node that first node of queue contains
MaxHeapTree getFront(Queue queue)
{
    return (queue->first->treeNode);    
}

//-------------------  QUEUE_NODE -----------------//
//Function to initialize a queue node
void initializeQueueNode(QueueNode queueNode, MaxHeapTree treeNode)
{
    queueNode->treeNode = treeNode;
    queueNode->next = NULL;
}