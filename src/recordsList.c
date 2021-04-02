/**
 * This file contains methods for records list.
 * Creation, Insertion, Check Id Duplication, Freeing Memory and Printing.
**/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/recordsList.h"
#include "include/record.h"

//Function to initialize records list
RecordListNode initializeRecordsList() 
{
    RecordListNode head;
    head = malloc(sizeof(Record));

    head->recordID = NULL;
    head->patientFirstName = NULL;
    head->patientLastName = NULL;
    head->diseaseID = NULL;
    head->country = NULL;
    head->entryDate = NULL;
    head->exitDate = NULL;

    head->next = NULL;

    return head;
}

//Function to insert a record into list
void addRecordToList(RecordsList recordsList, char* recordID, char* patientFirstName, char* patientLastName, char* diseaseID, char* country, DateStructure entryDate, DateStructure exitDate)
{
    RecordListNode currentNode = recordsList->tailNode;

    if (currentNode->recordID == NULL) //first record
    {
        initializeRecord(currentNode, recordID, patientFirstName, patientLastName, diseaseID, country, entryDate, exitDate);
        currentNode->next = NULL;
        return;
    }
    
    RecordListNode newRecord;
        
    newRecord = malloc(sizeof(Record));
    initializeRecord(newRecord, recordID, patientFirstName, patientLastName, diseaseID, country, entryDate, exitDate);
    
    newRecord->next = NULL;
    currentNode->next = newRecord;
    recordsList->tailNode = newRecord;

    return;
}

//Function to check id duplication error
int checkId(RecordsList recordsList, char* recordId)
{
    RecordListNode currentNode = recordsList->headNode;
    while (currentNode != NULL && currentNode->recordID != NULL)
    {
        if(strcmp(currentNode->recordID, recordId) == 0)
        {
            return -1;
        }
        currentNode = currentNode->next;
    }
    
    return 1; 
}

//Function to print records list
void printRecordsListIds(RecordsList recordsList)
{
    RecordListNode current = recordsList->headNode;
    while (current != NULL)
    {
        if (current->exitDate->day != 0)
        {
            printf("%s %s %s %s %s %d-%d-%d %d-%d-%d\n", current->recordID, current->patientFirstName, current->patientLastName, current->diseaseID, current->country, current->entryDate->day, current->entryDate->month, current->entryDate->year, current->exitDate->day, current->exitDate->month, current->exitDate->year);
        }
        else
        {
            printf("%s %s %s %s %s %d-%d-%d\n", current->recordID, current->patientFirstName, current->patientLastName, current->diseaseID, current->country, current->entryDate->day, current->entryDate->month, current->entryDate->year);
        }
        current = current->next;
    }
}

//Function to free records list memory allocated
void freeRecordsList(RecordsList recordsList)
{
    RecordListNode current = recordsList->headNode;
    RecordListNode temporary_next;

    while (current != NULL)
    {
        free(current->recordID);
        free(current->patientFirstName);
        free(current->patientLastName);
        free(current->diseaseID);
        free(current->country);
        destroyDate(current->entryDate);
        destroyDate(current->exitDate);
        temporary_next = current;
        current = current->next;
        free(temporary_next);
    }
}