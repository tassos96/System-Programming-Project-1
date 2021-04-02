/**	
 * Avl's Tree methods implementation.
 * Algorithm idea taken from: "https://www.thecrazyprogrammer.com/2014/03/c-program-for-avl-tree-implementation.html."
 * 
 * This file contains Avl tree methods (Insertion, Rotation, Get Height, Get Balance Factory, Printing nodes and Freeing memory).
 * Also contains functions used to calculate results that displayed on interface commands.
 * 
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/avlTree.h"

//Function to insert a node at Avl Tree
//treeNode: A tree node pointer
//dateKey: The tree's node key for insertion (Date struct)
//recordNode: Pointer to the recordsList node that tree node have to point
AvlTreeNode insertNode(AvlTreeNode treeNode, DateStructure dateKey, RecordListNode recordNode)
{
	if(treeNode==NULL)	//if its the first node (tree's root)
	{
		treeNode = malloc(sizeof(TreeNode));

		treeNode->dateKey = dateKey;
		treeNode->recordNode = recordNode;

		treeNode->left=NULL;
		treeNode->right=NULL;
	}
	else
	{
		if(compareDateKey(dateKey, treeNode->dateKey) == 1)		// insert in right subtree, if datekey > treeNode's dateKey
		{
			treeNode->right = insertNode(treeNode->right, dateKey, recordNode);
			if(balanceFactor(treeNode) == -2)	//Re-balance tree
			{
				if(compareDateKey(dateKey, treeNode->right->dateKey) == 1)
				{
					treeNode = RR(treeNode);
				}
				else
				{
					treeNode = RL(treeNode);
				}
			}
		}
		else
		{
			if(compareDateKey(dateKey, treeNode->dateKey) == -1 ||compareDateKey(dateKey, treeNode->dateKey) ==  0) //insert in left subtree, if dateKey <= treeNode's dateKey
			{
				treeNode->left = insertNode(treeNode->left, dateKey, recordNode);
				if(balanceFactor(treeNode) == 2) //Re-Balance tree
				{
					if(compareDateKey(dateKey, treeNode->left->dateKey) == -1 ||compareDateKey(dateKey, treeNode->left->dateKey) == 0)
					{
						treeNode = LL(treeNode);
					}
					else
					{
						treeNode = LR(treeNode);
					}
				}
			}
		}
	}	
	
	treeNode->height = height(treeNode);	
	return(treeNode);
}
 
//Function to get tree's height
//treeNode: A tree node pointer 
int height(AvlTreeNode treeNode)
{
	int leftHeight;
	int rightHeight;

	if(treeNode == NULL)
		return(0);
	
	if(treeNode->left == NULL)
		leftHeight = 0;
	else
		leftHeight = 1 + treeNode->left->height;
		
	if(treeNode->right == NULL)
		rightHeight = 0;
	else
		rightHeight = 1 + treeNode->right->height;
	
	if(leftHeight > rightHeight)
		return(leftHeight);
	
	return(rightHeight);
}

//Function for right rotation 
AvlTreeNode rotateRight(AvlTreeNode x)
{
	AvlTreeNode y;

	y = x->left;
	x->left = y->right;
	y->right = x;
	
	x->height = height(x);
	y->height = height(y);
	
	return(y);
}

//Function for left rotation 
AvlTreeNode rotateLeft(AvlTreeNode x)
{
	AvlTreeNode y;
	
	y = x->right;
	x->right = y->left;
	y->left = x;
	
	x->height = height(x);
	y->height = height(y);

	return(y);
}

 
// All possible rotation compinations following
AvlTreeNode RR(AvlTreeNode treeNode)
{
	treeNode = rotateLeft(treeNode);
	return(treeNode);
}
 
AvlTreeNode LL(AvlTreeNode treeNode)
{
	treeNode = rotateRight(treeNode);
	return(treeNode);
}
 
AvlTreeNode LR(AvlTreeNode treeNode)
{
	treeNode->left = rotateLeft(treeNode->left);
	treeNode = rotateRight(treeNode);
	
	return(treeNode);
}
 
AvlTreeNode RL(AvlTreeNode treeNode)
{
	treeNode->right = rotateRight(treeNode->right);
	treeNode = rotateLeft(treeNode);
	
	return(treeNode);
}

//Function to get balance factor 
int balanceFactor(AvlTreeNode treeNode)
{
	int leftHeight;
	int rightHeight;

	if(treeNode == NULL)
		return(0);
 
	if(treeNode->left == NULL)
		leftHeight = 0;
	else
		leftHeight = 1 + treeNode->left->height;
 
	if(treeNode->right == NULL)
		rightHeight = 0;
	else
		rightHeight = 1 + treeNode->right->height;
 
	return(leftHeight - rightHeight);
}

//Fuction to print AVL tree in PreOrder traversal 
void printPreOrder(AvlTreeNode treeNode)
{
	if(treeNode != NULL)
	{
		printf("\t\t\t  BF=%d: %d-%d-%d\n",balanceFactor(treeNode),treeNode->dateKey->day, treeNode->dateKey->month, treeNode->dateKey->year);
		printPreOrder(treeNode->left);
		printPreOrder(treeNode->right);
	}
}

//Function to print AVL tree in InOrder traversal 
void printInOrder(AvlTreeNode treeNode)
{
	if(treeNode != NULL)
	{
		printInOrder(treeNode->left);
		printf("\t\t\t  BF=%d: %d-%d-%d\n",balanceFactor(treeNode),treeNode->dateKey->day, treeNode->dateKey->month, treeNode->dateKey->year);
		printInOrder(treeNode->right);
	}
}

//Function to free all AVL tree memory allocated
void freeAvlTree(AvlTreeNode treeNode)
{
	AvlTreeNode temporary = treeNode;
	if(temporary != NULL)
	{
		freeAvlTree(temporary->left);
		freeAvlTree(temporary->right);
		free(temporary);
	}
}

//--------------------    Functions for interface commands    --------------------//

void countNodesInOrder(AvlTreeNode treeNode, int* counter)
{
    if(treeNode != NULL)
	{
        countNodesInOrder(treeNode->left, counter);
        (*counter)++;
        countNodesInOrder(treeNode->right, counter);
    }
}

void countNodesInOrderWithDates(AvlTreeNode treeNode, int* counter, DateStructure date1, DateStructure date2)
{
    if (treeNode != NULL)
    {
        countNodesInOrderWithDates(treeNode->left, counter, date1, date2);
        if (compareDateKey(date1, treeNode->dateKey) == -1 || compareDateKey(date1, treeNode->dateKey) == 0)
        {
            if (compareDateKey(date2, treeNode->dateKey) == 1 || compareDateKey(date2, treeNode->dateKey) == 0)
            {
                (*counter)++;
            }
        }
        countNodesInOrderWithDates(treeNode->right, counter, date1, date2);
    }
}

void countNodesInOrderWithDatesAndCountry(AvlTreeNode treeNode, int* counter, DateStructure date1, DateStructure date2, char* country)
{
	if (treeNode != NULL)
    {
		countNodesInOrderWithDatesAndCountry(treeNode->left, counter, date1, date2, country);
		if (compareDateKey(date1, treeNode->dateKey) == -1 || compareDateKey(date1, treeNode->dateKey) == 0)
        {
            if (compareDateKey(date2, treeNode->dateKey) == 1 || compareDateKey(date2, treeNode->dateKey) == 0)
            {
				if (strcmp(treeNode->recordNode->country, country) == 0)
				{
					(*counter)++;
				}
            }
		}
		countNodesInOrderWithDatesAndCountry(treeNode->right, counter, date1, date2, country);
	}
}

void countNodesCurrentPatients(AvlTreeNode treeNode, int* counter)
{
	if (treeNode != NULL)
	{
		countNodesCurrentPatients(treeNode->left, counter);
		if (treeNode->recordNode->exitDate->year == 0)
		{
			(*counter)++;
		}
		countNodesCurrentPatients(treeNode->right, counter);
	}
}

//Function to count tree nodes for topK-Diseases command
void countNodesForTopKDiseases(AvlTreeNode treeNode, int* counter, char* country)
{
	if(treeNode != NULL)
	{
		countNodesForTopKDiseases(treeNode->left, counter, country);
		if (strcmp(treeNode->recordNode->country, country) == 0)
		{
			(*counter)++;
		}
		countNodesForTopKDiseases(treeNode->right, counter, country);
	}
}

//Function to count tree nodes for topK-Diseases command with dates addition
void countNodesForTopKDiseasesWithDates(AvlTreeNode treeNode, int* counter, char* country, DateStructure date1, DateStructure date2)
{
	if(treeNode != NULL)
	{
		countNodesForTopKDiseasesWithDates(treeNode->left, counter, country, date1, date2);
		if (compareDateKey(date1, treeNode->dateKey) == -1 || compareDateKey(date1, treeNode->dateKey) == 0)
        {
            if (compareDateKey(date2, treeNode->dateKey) == 1 || compareDateKey(date2, treeNode->dateKey) == 0)
            {
				if (strcmp(treeNode->recordNode->country, country) == 0)
				{
					(*counter)++;
				}
            }
		}
		countNodesForTopKDiseasesWithDates(treeNode->right, counter, country, date1, date2);
	}
}

//Function to count tree nodes for topK-Countries command
void countNodesForTopKCountries(AvlTreeNode treeNode, int* counter, char* disease)
{
	if(treeNode != NULL)
	{
		countNodesForTopKCountries(treeNode->left, counter, disease);
		if (strcmp(treeNode->recordNode->diseaseID, disease) == 0)
		{
			(*counter)++;
		}
		countNodesForTopKCountries(treeNode->right, counter, disease);
	}
}

//Function to count tree nodes for topK-Countries command with dates addition
void countNodesForTopKCountriesWithDates(AvlTreeNode treeNode, int* counter, char* disease, DateStructure date1, DateStructure date2)
{
	if(treeNode != NULL)
	{
		countNodesForTopKCountriesWithDates(treeNode->left, counter, disease, date1, date2);
		if (compareDateKey(date1, treeNode->dateKey) == -1 || compareDateKey(date1, treeNode->dateKey) == 0)
        {
            if (compareDateKey(date2, treeNode->dateKey) == 1 || compareDateKey(date2, treeNode->dateKey) == 0)
            {
				if (strcmp(treeNode->recordNode->diseaseID, disease) == 0)
				{
					(*counter)++;
				}
            }
		}
		countNodesForTopKCountriesWithDates(treeNode->right, counter, disease, date1, date2);
	}
}