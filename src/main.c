#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "include/interfaceUtilities.h"
#include "include/fileReader.h"
#include "include/recordsList.h"
#include "include/hashTables.h"

#define SUCCESS 0
#define ERROR -1
#define EXIT 8

int main(int argc, char const *argv[]) 
{
    //Arguments Variables  
    char* patientRecordFileName = NULL;         //File Name
    int diseaseHashtableNumOfEntries = -1;      //Disease Hashtable size
    int countryHashtableNumOfEntries = -1;      //Country Hashtable size
    int bucketSize = -1;                        //Buckets size (Bytes)

    //Read all arguments from command line and initialize the variables
    readCommandLineArguments(&patientRecordFileName, &diseaseHashtableNumOfEntries, &countryHashtableNumOfEntries, &bucketSize, argc, argv);
    //Check for error (If some of args have not initialized, return error)
    checkForArgsError(patientRecordFileName, diseaseHashtableNumOfEntries, countryHashtableNumOfEntries, bucketSize);

    //Print the variables value for check
    //printf("File name: %s\n", patientRecordFileName);
    //printf("Disease Hashtable size: %d\n", diseaseHashtableNumOfEntries);
    //printf("Country Hashtable size: %d\n", countryHashtableNumOfEntries);
    //printf("Bucket Size: %d\n", bucketSize);

    //Read records from file
    FILE* filePointer;
    
    RecordsList recordsList = malloc(sizeof(List));
    recordsList->headNode = initializeRecordsList();
    recordsList->tailNode = recordsList->headNode;

    HashTable diseaseHashTable;
    diseaseHashTable = createHashTable(diseaseHashtableNumOfEntries, bucketSize);
    if (diseaseHashTable == NULL)
    {
        free(diseaseHashTable);
        freeRecordsList(recordsList);
        free(recordsList);
        free(patientRecordFileName);
        return ERROR;
    }

    HashTable countryHashTable;
    countryHashTable = createHashTable(countryHashtableNumOfEntries, bucketSize);

    if (countryHashTable == NULL)
    {
        freeHashTable(diseaseHashTable);
        free(countryHashTable);
        freeRecordsList(recordsList);
        free(recordsList);
        free(patientRecordFileName);
        return ERROR;
    }
    
    //printf("Creating Data Structures... \n");
    if(readRecordsFromFile(&filePointer, diseaseHashTable, countryHashTable, recordsList, patientRecordFileName) == ERROR)
    {
        freeHashTable(diseaseHashTable);
        free(diseaseHashTable);
        freeHashTable(countryHashTable);
        free(countryHashTable);
        freeRecordsList(recordsList);
        free(recordsList);
        free(patientRecordFileName);
        return ERROR;
    }
    
    //printf("Print records list\n");
    //printRecordsListIds(recordsList);
    //printf("Print Disease HashTable\n");
    //printHashTable(diseaseHashTable);
    //printf("Print Country HashTable\n");
    //printHashTable(countryHashTable);

    //printf("Please enter your command: \n");
    while(1)
    {
        int commandCode = readCommandFromInterface(recordsList, diseaseHashTable, countryHashTable);
        if (commandCode == EXIT) //exit
        {
            break;
        }
    }

    printf("exiting\n");
    
    freeHashTable(diseaseHashTable);
    free(diseaseHashTable);
    freeHashTable(countryHashTable);
    free(countryHashTable);
    freeRecordsList(recordsList);
    free(recordsList);
    free(patientRecordFileName); //free filename memory

    return SUCCESS;
}