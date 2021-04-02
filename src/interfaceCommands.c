/**
 * This file contains implementation of each interface command. 
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/interfaceCommands.h"

#define GLOBAL_DISEASE_STATS 1
#define DISEASE_FREQUENCY 2
#define TOP_K_DISEASES 3
#define TOP_K_COUNTRIES 4
#define INSERT_PATIENT_RECORD 5
#define RECORD_PATIENT_EXIT 6
#define NUM_CURRENT_PATIENTS 7
#define ERROR -1

//Function to print wrong command error
void printWrongCommandError()
{
    printf("Error occured: Something goes wrong with command.\n");
    printf("Please try again\n");
}

// Run /globalDiseaseStats [date1 date2]
int runGlobalDiseaseStats(char*** arguments, int argumentsNum, HashTable diseaseHashTable)
{
    //printf("Running globalDiseaseStats...\n");
    if (argumentsNum == 1)  //Without dates arguments
    {
        Bucket temporary;
        for(int i = 0; i<diseaseHashTable->hashTableSize; i++)
        {
            temporary = diseaseHashTable->bucketList[i];
            while (temporary != NULL)
            {
                for(int j = 0; j < temporary->maxBucketEntries; j++)
                {
                    int counter = 0;
                    if(temporary->entries[j] != NULL && temporary->entries[j]->treeRoot != NULL)
                    {
                        printf("%s ", temporary->entries[j]->entryString);
                        countNodesInOrder(temporary->entries[j]->treeRoot, &counter);
                        printf("%d\n", counter);
                    }
                }
                temporary = temporary->nextBucket;
            }
        }
        return GLOBAL_DISEASE_STATS;
    }
    else if (argumentsNum == 3) //With dates arguments
    {
        char* date1_string = NULL;
        char* date2_string = NULL; 

        date1_string = malloc(sizeof(char*) * strlen(arguments[0][1])+1);
        strcpy(date1_string, arguments[0][1]);

        date2_string = malloc(sizeof(char*) * strlen(arguments[0][2])+1);
        strcpy(date2_string, arguments[0][2]);

        DateStructure date1 = setDate(date1_string);
        if (date1->day == -1 || date1->month == -1 || date1->year == -1)
        {
            destroyDate(date1);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        DateStructure date2 = setDate(date2_string);
        if (date2->day == -1 || date2->month == -1 || date2->year == -1)
        {
            destroyDate(date1);
            destroyDate(date2);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        if(datesOrderValidation(date1, date2) == -1)
        {
            destroyDate(date1);
            destroyDate(date2);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        //------------------------------------------
        Bucket temporary;
        for(int i = 0; i<diseaseHashTable->hashTableSize; i++)
        {
            temporary = diseaseHashTable->bucketList[i];
            while (temporary != NULL)
            {
                for(int j = 0; j < temporary->maxBucketEntries; j++)
                {
                    int counter = 0;
                    if(temporary->entries[j] != NULL && temporary->entries[j]->treeRoot != NULL)
                    {
                        printf("%s ", temporary->entries[j]->entryString);
                        countNodesInOrderWithDates(temporary->entries[j]->treeRoot, &counter, date1, date2);
                        printf("%d\n", counter);
                    }
                }
                temporary = temporary->nextBucket;
            }
        }
        //------------------------------------------

        destroyDate(date1);
        destroyDate(date2);
        free(date1_string);
        free(date2_string);
        return GLOBAL_DISEASE_STATS;
    }
    else
    {
        return ERROR;
    }
}

//Run /diseaseFrequency virusName date1 date2 [country] 
int runDiseaseFrequency(char*** arguments, int argumentsNum, HashTable diseaseHashTable)
{
    //printf("Running diseaseFrequency...\n");
    char* virusName = NULL;
    char* date1_string = NULL;
    char* date2_string = NULL;
    char* country = NULL;

    
    if(argumentsNum == 4)   //Without country argument
    {
        virusName = malloc(sizeof(char*) * strlen(arguments[0][1]) +1);
        strcpy(virusName, arguments[0][1]);

        date1_string = malloc(sizeof(char*) * strlen(arguments[0][2]) + 1);
        strcpy(date1_string, arguments[0][2]);

        date2_string = malloc(sizeof(char*) * strlen(arguments[0][3]) + 1);
        strcpy(date2_string, arguments[0][3]);

        DateStructure date1 = setDate(date1_string);
        if (date1->day == -1 || date1->month == -1 || date1->year == -1)
        {
            destroyDate(date1);
            free(virusName);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        DateStructure date2 = setDate(date2_string);
        if (date2->day == -1 || date2->month == -1 || date2->year == -1)
        {
            destroyDate(date1);
            destroyDate(date2);
            free(virusName);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        if(datesOrderValidation(date1, date2) == -1)
        {
            destroyDate(date1);
            destroyDate(date2);
            free(virusName);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        Bucket temporary;
        
        int index = hashFunction(diseaseHashTable, virusName);
        temporary = diseaseHashTable->bucketList[index];

        //-------------------------
        while (temporary != NULL)
        {
            for(int j = 0; j < temporary->maxBucketEntries; j++)
            {
                int counter = 0;
                if(temporary->entries[j] != NULL && temporary->entries[j]->treeRoot != NULL)
                {
                    if(strcmp(temporary->entries[j]->entryString, virusName) == 0)
                    {
                        printf("%s ", temporary->entries[j]->entryString);
                        countNodesInOrderWithDates(temporary->entries[j]->treeRoot, &counter, date1, date2);
                        printf("%d\n", counter);
                    }
                }
            }
            temporary = temporary->nextBucket;
        }
        //-------------------------

        free(virusName);
        destroyDate(date1);
        destroyDate(date2);
        free(date1_string);
        free(date2_string);
        return DISEASE_FREQUENCY;
    }
    else if(argumentsNum == 5)  //With country argument
    {
        virusName = malloc(sizeof(char*) * strlen(arguments[0][1]) +1);
        strcpy(virusName, arguments[0][1]);

        date1_string = malloc(sizeof(char*) * strlen(arguments[0][2]) + 1);
        strcpy(date1_string, arguments[0][2]);

        date2_string = malloc(sizeof(char*) * strlen(arguments[0][3]) + 1);
        strcpy(date2_string, arguments[0][3]);

        DateStructure date1 = setDate(date1_string);
        if (date1->day == -1 || date1->month == -1 || date1->year == -1)
        {
            destroyDate(date1);
            free(virusName);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        DateStructure date2 = setDate(date2_string);
        if (date2->day == -1 || date2->month == -1 || date2->year == -1)
        {
            destroyDate(date1);
            destroyDate(date2);
            free(virusName);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        if(datesOrderValidation(date1, date2) == -1)
        {
            destroyDate(date1);
            destroyDate(date2);
            free(virusName);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        country = malloc(sizeof(char*) * strlen(arguments[0][4]) +1);
        strcpy(country, arguments[0][4]);


        Bucket temporary;
        
        int index = hashFunction(diseaseHashTable, virusName);
        temporary = diseaseHashTable->bucketList[index];

        //-------------------------
        while (temporary != NULL)
        {
            for(int j = 0; j < temporary->maxBucketEntries; j++)
            {
                int counter = 0;
                if(temporary->entries[j] != NULL && temporary->entries[j]->treeRoot != NULL)
                {
                    if(strcmp(temporary->entries[j]->entryString, virusName) == 0)
                    {
                        printf("%s ", temporary->entries[j]->entryString);
                        countNodesInOrderWithDatesAndCountry(temporary->entries[j]->treeRoot, &counter, date1, date2, country);
                        printf("%d\n", counter);
                    }
                }
            }
            temporary = temporary->nextBucket;
        }
        //-------------------------

        free(virusName);
        free(date1_string);
        free(date2_string);
        destroyDate(date1);
        destroyDate(date2);
        free(country);
        return DISEASE_FREQUENCY;
    }
    else
    {
        return ERROR;
    }
}

// Run /topK-Diseases k country [date1 date2]
int runTopKDiseases(char*** arguments, int argumentsNum, HashTable diseaseHashTable)
{
    int k = 0;
    char* country = NULL;
    char* date1_string = NULL;
    char* date2_string = NULL;

    char* endptr;

    if (argumentsNum == 3)  //Without dates argument
    {
        
        k = strtol(arguments[0][1], &endptr, 10);
        if (*endptr) //if the result is 0, error
        {
            printf("\nConversion error occurred: Wrong K insertion !\n");
            k = -1;
            return ERROR;
        }

        country = malloc(sizeof(char*) * strlen(arguments[0][2]) + 1);
        strcpy(country, arguments[0][2]);   //assign country

        //printf("topK-Diseases %d %s\n", k, country);

        //-------------------------------
        Queue queue = createQueue();
        MaxHeapTree treeRoot = NULL;
        MaxHeapTree tempMaxHeapTreeNode = NULL;

        Bucket temporary;
        for (int i = 0; i < diseaseHashTable->hashTableSize; i++)
        {
            temporary = diseaseHashTable->bucketList[i];
            while (temporary != NULL)
            {
                for(int j = 0; j < temporary->maxBucketEntries; j++)
                {
                    int counter = 0;
                    if (temporary->entries[j] != NULL && temporary->entries[j]->treeRoot != NULL)
                    {
                        countNodesForTopKDiseases(temporary->entries[j]->treeRoot, &counter, country);
                        tempMaxHeapTreeNode = newMaxHeapTreeNode(tempMaxHeapTreeNode, temporary->entries[j]->entryString, counter);
                        insertToMaxHeapTree(&treeRoot, tempMaxHeapTreeNode, queue);
                    }
                }
                temporary = temporary->nextBucket;
            }
        }
        MaxHeapTree lastNode; //Max heap last node inserted (This node will be deleted to keep tree's balance)
        MaxHeapTree lastNodeParent; //Last nodes parent (To update left or right child pointer to NULL)
        int childSide = -1; // If is 1 then its left side, if is 0 then its right side (Which side set to NULL at parent node)
        
        //Root to be deleted
        MaxHeapTree popRoot = NULL;

        int previousScore = -1;
        while (k != 0)
        {
            preorder(treeRoot);//Heapify Complete Tree (Again)
            levelOrder(treeRoot, &lastNode, &lastNodeParent, &childSide); //find last node of tree, keep it for deletion
            popRoot = popTreeRoot(treeRoot, lastNode, lastNodeParent, childSide); //pop root, get ready for deletion
            if(popRoot->score == previousScore)
            {
                deleteRoot(popRoot);
                continue;
            }
            if(popRoot->score != 0 && popRoot->score != previousScore)  //check if same score has already displayed
            {
                printf("%s %d\n", popRoot->name, popRoot->score);   //print result name and score
                previousScore = popRoot->score; //Update score flag for if condition
                
            }
            deleteRoot(popRoot);    //free memory of poped treeRoot
            k--;
        }

        freeMaxHeapTree(treeRoot);  //free max heap tree nodes lefted
        freeQueue(queue);   //freeQueue
        free(queue);    
        
        free(country);  //free country name string
        return TOP_K_DISEASES; //Return success code
    }
    else if (argumentsNum == 5) //With dates arguments
    {
        k = strtol(arguments[0][1], &endptr, 10);
        if (*endptr) //if the result is 0, error
        {
            printf("\nConversion error occurred: Wrong K insertion !\n");
            k = -1;
            return ERROR;
        }

        country = malloc(sizeof(char*) * strlen(arguments[0][2]) + 1);
        strcpy(country, arguments[0][2]);   //assign country

        date1_string = malloc(sizeof(char*) * strlen(arguments[0][3]) + 1);
        strcpy(date1_string, arguments[0][3]); //assign date1

        date2_string = malloc(sizeof(char*) * strlen(arguments[0][4]) + 1);
        strcpy(date2_string, arguments[0][4]); //assign date2

        DateStructure date1 = setDate(date1_string);
        if (date1->day == -1 || date1->month == -1 || date1->year == -1)
        {
            destroyDate(date1);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        DateStructure date2 = setDate(date2_string);
        if (date2->day == -1 || date2->month == -1 || date2->year == -1)
        {
            destroyDate(date1);
            destroyDate(date2);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        if(datesOrderValidation(date1, date2) == -1)
        {
            destroyDate(date1);
            destroyDate(date2);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        //printf("topK-Diseases %d %s %s %s\n", k, country, date1_string, date2_string);

        //---------------------------------------------
        Queue queue = createQueue();
        MaxHeapTree treeRoot = NULL;
        MaxHeapTree tempMaxHeapTreeNode = NULL;

        Bucket temporary;
        for (int i = 0; i < diseaseHashTable->hashTableSize; i++)
        {
            temporary = diseaseHashTable->bucketList[i];
            while (temporary != NULL)
            {
                for(int j = 0; j < temporary->maxBucketEntries; j++)
                {
                    int counter = 0;
                    if (temporary->entries[j] != NULL && temporary->entries[j]->treeRoot != NULL)
                    {
                        countNodesForTopKDiseasesWithDates(temporary->entries[j]->treeRoot, &counter, country, date1, date2);
                        tempMaxHeapTreeNode = newMaxHeapTreeNode(tempMaxHeapTreeNode, temporary->entries[j]->entryString, counter);
                        insertToMaxHeapTree(&treeRoot, tempMaxHeapTreeNode, queue);
                    }
                }
                temporary = temporary->nextBucket;
            }
        }
        //---------------------------------------------

        MaxHeapTree lastNode; //Max heap last node inserted (This node will be deleted to keep tree's balance)
        MaxHeapTree lastNodeParent; //Last nodes parent (To update left or right child pointer to NULL)
        int childSide = -1; // If is 1 then its left side, if is 0 then its right side (Which side set to NULL at parent node)
        
        //Root to be deleted
        MaxHeapTree popRoot = NULL;

        int previousScore = -1;
        while (k != 0)
        {
            preorder(treeRoot);//Heapify Complete Tree (Again)
            levelOrder(treeRoot, &lastNode, &lastNodeParent, &childSide); //find last node of tree, keep it for deletion
            popRoot = popTreeRoot(treeRoot, lastNode, lastNodeParent, childSide); //pop root, get ready for deletion
            if(popRoot->score == previousScore)
            {
                deleteRoot(popRoot);
                continue;
            }
            if(popRoot->score != 0 && popRoot->score != previousScore)  //check if same score has already displayed
            {
                printf("%s %d\n", popRoot->name, popRoot->score);   //print result name and score
                previousScore = popRoot->score; //Update score flag for if condition
                
            }
            deleteRoot(popRoot);    //free memory of poped treeRoot
            k--;
        }

        freeMaxHeapTree(treeRoot);  //free max heap tree nodes lefted
        freeQueue(queue);   //freeQueue
        free(queue);    

        free(country);
        free(date1_string);
        free(date2_string);
        destroyDate(date1);
        destroyDate(date2);
        return TOP_K_DISEASES;
    }
    else
    {
        free(country);
        return ERROR;
    }
}

// Run /topK-Countries k disease [date1 date2]
int runTopKCountries(char*** arguments, int argumentsNum, HashTable countryHashTable)
{
    int k = 0;
    char* disease = NULL;
    char* date1_string = NULL;
    char* date2_string = NULL;

    char* endptr;

    k = strtol(arguments[0][1], &endptr, 10);
    if (*endptr) //if the result is 0, error
    {
        printf("\nConversion error occurred: Wrong K insertion !\n");
        k = -1;
        return ERROR;
    }

    if (argumentsNum == 3)  //Without dates arguments
    {
        disease = malloc(sizeof(char*) * strlen(arguments[0][2]) + 1);
        strcpy(disease, arguments[0][2]);   //assign country
        
        //printf("topK-Countries %d %s\n", k, disease);

        //-------------------------------
        Queue queue = createQueue();
        MaxHeapTree treeRoot = NULL;
        MaxHeapTree tempMaxHeapTreeNode = NULL;

        Bucket temporary;
        for(int i = 0; i < countryHashTable->hashTableSize; i++)
        {
            temporary = countryHashTable->bucketList[i];
            while (temporary != NULL)
            {
                for(int j = 0; j < temporary->maxBucketEntries; j++)
                {
                    int counter = 0;
                    if(temporary->entries[j] != NULL && temporary->entries[j]->treeRoot != NULL)
                    {
                        //printf("%s ", temporary->entries[j]->entryString);
                        countNodesForTopKCountries(temporary->entries[j]->treeRoot, &counter, disease);
                        //printf("%d\n", counter);
                        tempMaxHeapTreeNode = newMaxHeapTreeNode(tempMaxHeapTreeNode, temporary->entries[j]->entryString, counter);
                        insertToMaxHeapTree(&treeRoot, tempMaxHeapTreeNode, queue);
                    }
                }
                temporary = temporary->nextBucket;
            }
        }
        MaxHeapTree lastNode; //Max heap last node inserted (This node will be deleted to keep tree's balance)
        MaxHeapTree lastNodeParent; //Last nodes parent (To update left or right child pointer to NULL)
        int childSide = -1; // If is 1 then its left side, if is 0 then its right side (Which side set to NULL at parent node)
        
        //Root to be deleted
        MaxHeapTree popRoot = NULL;

        int previousScore = -1;
        while (k != 0)
        {
            preorder(treeRoot);//Heapify Complete Tree (Again)
            levelOrder(treeRoot, &lastNode, &lastNodeParent, &childSide); //find last node of tree, keep it for deletion
            popRoot = popTreeRoot(treeRoot, lastNode, lastNodeParent, childSide); //pop root, get ready for deletion
            if(popRoot->score == previousScore)
            {
                deleteRoot(popRoot);
                continue;
            }
            if(popRoot->score != 0 && popRoot->score != previousScore)  //check if same score has already displayed
            {
                printf("%s %d\n", popRoot->name, popRoot->score);   //print result name and score
                previousScore = popRoot->score; //Update score flag for if condition
                
            }
            deleteRoot(popRoot);    //free memory of poped treeRoot
            k--;
        }

        freeMaxHeapTree(treeRoot);  //free max heap tree nodes lefted
        freeQueue(queue);   //freeQueue
        free(queue);

        free(disease);
        return TOP_K_COUNTRIES;
    }
    else if (argumentsNum == 5) //With dates arguments
    {
        disease = malloc(sizeof(char*) * strlen(arguments[0][2]) + 1);
        strcpy(disease, arguments[0][2]);   //assign country

        date1_string = malloc(sizeof(char*) * strlen(arguments[0][3]) + 1);
        strcpy(date1_string, arguments[0][3]); //assign date1

        date2_string = malloc(sizeof(char*) * strlen(arguments[0][4]) + 1);
        strcpy(date2_string, arguments[0][4]); //assign date2

        DateStructure date1 = setDate(date1_string);
        if (date1->day == -1 || date1->month == -1 || date1->year == -1)
        {
            destroyDate(date1);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        DateStructure date2 = setDate(date2_string);
        if (date2->day == -1 || date2->month == -1 || date2->year == -1)
        {
            destroyDate(date1);
            destroyDate(date2);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        if(datesOrderValidation(date1, date2) == -1)
        {
            destroyDate(date1);
            destroyDate(date2);
            free(date1_string);
            free(date2_string);
            return ERROR;
        }

        //printf("topK-Countries %d %s %s %s\n", k, disease, date1_string, date2_string);

        //---------------------------------------------
        Queue queue = createQueue();
        MaxHeapTree treeRoot = NULL;
        MaxHeapTree tempMaxHeapTreeNode = NULL;

        Bucket temporary;
        for (int i = 0; i < countryHashTable->hashTableSize; i++)
        {
            temporary = countryHashTable->bucketList[i];
            while (temporary != NULL)
            {
                for(int j = 0; j < temporary->maxBucketEntries; j++)
                {
                    int counter = 0;
                    if (temporary->entries[j] != NULL && temporary->entries[j]->treeRoot != NULL)
                    {
                        countNodesForTopKCountriesWithDates(temporary->entries[j]->treeRoot, &counter, disease, date1, date2);
                        tempMaxHeapTreeNode = newMaxHeapTreeNode(tempMaxHeapTreeNode, temporary->entries[j]->entryString, counter);
                        insertToMaxHeapTree(&treeRoot, tempMaxHeapTreeNode, queue);
                    }
                }
                temporary = temporary->nextBucket;
            }
        }
        //---------------------------------------------

        MaxHeapTree lastNode; //Max heap last node inserted (This node will be deleted to keep tree's balance)
        MaxHeapTree lastNodeParent; //Last nodes parent (To update left or right child pointer to NULL)
        int childSide = -1; // If is 1 then its left side, if is 0 then its right side (Which side set to NULL at parent node)
        
        //Root to be deleted
        MaxHeapTree popRoot = NULL;

        int previousScore = -1;
        while (k != 0)
        {
            preorder(treeRoot);//Heapify Complete Tree (Again)
            levelOrder(treeRoot, &lastNode, &lastNodeParent, &childSide); //find last node of tree, keep it for deletion
            popRoot = popTreeRoot(treeRoot, lastNode, lastNodeParent, childSide); //pop root, get ready for deletion
            if(popRoot->score == previousScore)
            {
                deleteRoot(popRoot);
                continue;
            }
            if(popRoot->score != 0 && popRoot->score != previousScore)  //check if same score has already displayed
            {
                printf("%s %d\n", popRoot->name, popRoot->score);   //print result name and score
                previousScore = popRoot->score; //Update score flag for if condition
                
            }
            deleteRoot(popRoot);    //free memory of poped treeRoot
            k--;
        }

        freeMaxHeapTree(treeRoot);  //free max heap tree nodes lefted
        freeQueue(queue);   //freeQueue
        free(queue);

        free(disease);
        free(date1_string);
        free(date2_string);
        destroyDate(date1);
        destroyDate(date2);
        return TOP_K_COUNTRIES;
    }
    else
    {
        free(disease);
        return ERROR;
    }
    return TOP_K_COUNTRIES;
}

//Run /insertPatientRecord recordID patientFirstName patientLastName diseaseID country entryDate [exitDate]
int runInsertPatientRecord(char*** arguments, int argumentsNum, RecordsList recordsList, HashTable diseaseHashTable, HashTable countryHashTable)
{
    char* recordId = NULL;
    char* patientFirstName = NULL;
    char* patientLastName = NULL;
    char* diseaseId = NULL;
    char* country = NULL;
    char* entryDateString = NULL;
    char* exitDateString = NULL;
    
    //printf("Running insertPatientRecord...\n");
    
    if (argumentsNum == 7 || argumentsNum == 8)
    {
        recordId = malloc(sizeof(char*) * strlen(arguments[0][1]) + 1);
        strcpy(recordId, arguments[0][1]);

        patientFirstName = malloc(sizeof(char*) * strlen(arguments[0][2])+1);
        strcpy(patientFirstName, arguments[0][2]);

        patientLastName = malloc(sizeof(char*) * strlen(arguments[0][3])+1);
        strcpy(patientLastName, arguments[0][3]);

        diseaseId = malloc(sizeof(char*) * strlen(arguments[0][4]) + 1);
        strcpy(diseaseId, arguments[0][4]);

        country = malloc(sizeof(char*) * strlen(arguments[0][5]) + 1);
        strcpy(country, arguments[0][5]);

        entryDateString = malloc(sizeof(char*) * strlen(arguments[0][6])+1);
        strcpy(entryDateString, arguments[0][6]);
        DateStructure entryDate = setDate(entryDateString);

        DateStructure exitDate;
        if (argumentsNum == 7)
        {
            exitDate = setDate("-");
        }
        else
        {
            exitDateString = malloc(sizeof(char*) * strlen(arguments[0][7])+1);
            strcpy(exitDateString, arguments[0][7]);
            exitDate = setDate(exitDateString);
        }

        if (exitDate->day != 0 && datesOrderValidation(entryDate, exitDate) == ERROR)   //Check dates order
        {
            printf("Can't insert this record\n");
            destroyDate(entryDate);
            destroyDate(exitDate);
            free(recordId);
            free(patientFirstName);
            free(patientLastName);
            free(diseaseId);
            free(country);
            free(entryDateString);
            free(exitDateString);
            return ERROR;
        }
        
        if (checkId(recordsList, recordId) == -1) //check if this id already exists
        {
            printf("Error: Can't insert record because Record Id: %s already exists!\n", recordId);
            destroyDate(entryDate);
            destroyDate(exitDate);
            free(recordId);
            free(patientFirstName);
            free(patientLastName);
            free(diseaseId);
            free(country);
            free(entryDateString);
            free(exitDateString);
            return ERROR;           
        }
        else
        {
            addRecordToList(recordsList, recordId, patientFirstName, patientLastName, diseaseId, country, entryDate, exitDate);
            addToHashTable(diseaseHashTable, diseaseId, recordsList->tailNode);
            addToHashTable(countryHashTable, country, recordsList->tailNode);
            printf("Record added\n");
        }

        free(recordId);
        free(patientFirstName);
        free(patientLastName);
        free(diseaseId);
        free(country);
        free(entryDateString);
        free(exitDateString);

        return INSERT_PATIENT_RECORD;
    }
    else
    {
        return ERROR;
    }
}

// Run /recordPatientExit recordID exitDate
int runRecordPatientExit(char*** arguments, int argumentsNum, RecordsList recordList)
{
    char* recordId = NULL;
    char* exitDateString = NULL;
    
    RecordListNode current = recordList->headNode;

    if(argumentsNum == 3)
    {
        recordId = malloc(sizeof(char*) * strlen(arguments[0][1]) + 1);
        strcpy(recordId, arguments[0][1]);

        exitDateString = malloc(sizeof(char*) * strlen(arguments[0][2])+1);
        strcpy(exitDateString, arguments[0][2]);
        DateStructure exitDate = setDate(exitDateString);

        while (current != NULL)
        {
            if(strcmp(current->recordID, recordId) == 0)
            {
                //printf("Record with RecordId: %s found\n", current->recordID);
                if (current->exitDate->day == 0 || current->exitDate->month == 0 || current->exitDate->year == 0)
                {
                    if (datesOrderValidation(current->entryDate, exitDate) != ERROR)
                    {
                        current->exitDate->day = exitDate->day;
                        current->exitDate->month = exitDate->month;
                        current->exitDate->year = exitDate->year;
                        //printf("Added exit date to patient with id: %s\n", current->recordID);
                        printf("Record updated\n");
                        
                        free(recordId);
                        free(exitDateString);
                        destroyDate(exitDate);
                        return RECORD_PATIENT_EXIT;
                    }
                    else
                    {
                        free(recordId);
                        free(exitDateString);
                        destroyDate(exitDate);
                        return ERROR;
                    }
                }
                else
                {
                    if (datesOrderValidation(current->entryDate, exitDate) != ERROR)
                    {
                        current->exitDate->day = exitDate->day;
                        current->exitDate->month = exitDate->month;
                        current->exitDate->year = exitDate->year;
                        //printf("Updated exit date to patient with id: %s\n", current->recordID);
                        printf("Record updated\n");

                        free(recordId);
                        free(exitDateString);
                        destroyDate(exitDate);
                        return RECORD_PATIENT_EXIT;
                    }
                    else
                    {
                        free(recordId);
                        free(exitDateString);
                        destroyDate(exitDate);
                        return ERROR;
                    }
                }
            }
            current = current->next;
        }
        printf("This record doesn't exist\n");
        free(exitDateString);
        destroyDate(exitDate);
        return ERROR;
    }
    else
    {
        return ERROR;
    }
}

// Run /numCurrentPatients [disease]
int runNumCurrentPatients(char*** arguments, int argumentsNum, HashTable diseaseHashTable)
{
    char* disease = NULL;

    if (argumentsNum == 1)  //without disease argument
    {
        //printf("numCurrentPatients\n");
        Bucket temporary;
        for (int i = 0; i < diseaseHashTable->hashTableSize; i++)
        {
            temporary = diseaseHashTable->bucketList[i];
            while (temporary != NULL)
            {
                for (int j = 0; j < temporary->maxBucketEntries; j++)
                {
                    int counter = 0;
                    if (temporary->entries[j] != NULL && temporary->entries[j]->treeRoot != NULL)
                    {
                        printf("%s ", temporary->entries[j]->entryString);
                        countNodesCurrentPatients(temporary->entries[j]->treeRoot, &counter);
                        printf("%d\n", counter);
                    }
                }
                temporary = temporary->nextBucket;
            }
        }
        return NUM_CURRENT_PATIENTS;
    }
    else if (argumentsNum == 2) //with disease argument
    {
        disease = malloc(sizeof(char*) * strlen(arguments[0][1])+1);
        strcpy(disease, arguments[0][1]);
        
        //printf("NumCurrentPatients %s\n", disease);

        Bucket temporary;
        int index = hashFunction(diseaseHashTable, disease);
        temporary = diseaseHashTable->bucketList[index];
        
        while (temporary != NULL)
        {
            for (int j = 0; j < temporary->maxBucketEntries; j++)
            {
                int counter = 0;
                if (temporary->entries[j] != NULL && temporary->entries[j]->treeRoot != NULL)
                {
                    if(strcmp(temporary->entries[j]->entryString, disease) == 0)
                    {
                        printf("%s ", temporary->entries[j]->entryString);
                        countNodesCurrentPatients(temporary->entries[j]->treeRoot, &counter);
                        printf("%d\n", counter);
                    }
                }
            }
            temporary = temporary->nextBucket;
        }
        
        free(disease);
        return NUM_CURRENT_PATIENTS;
    }
    else
    {
        return ERROR;
    }
}