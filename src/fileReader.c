/**
 * This file contains file reader utility for my program.
 * Also adds records to structures (Records List, diseaseHashTable and countryHashable)
**/ 
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/fileReader.h"
#include "include/myDateStructure.h"
#include "include/recordsList.h"

#define SUCCESS 1
#define ERROR -1

//Function to read line by line the given records file
int readRecordsFromFile(FILE** filePointer, HashTable diseaseHashTable, HashTable countryHashTable, RecordsList recordsList, char* patientRecordFileName)
{
    char* line = NULL;
    size_t length = 0;
    ssize_t read;

    *filePointer = fopen(patientRecordFileName, "r");   //Open file with read access rights
    if (*filePointer == NULL)   //Error: File with this name doen's exist
    {
        printf("Error: File doesn't exist\n");
        return ERROR;
    }

    while ((read = getline(&line, &length, *filePointer)) != -1)    //Get a line from file
    {
        line[strlen(line)-1] = '\0';
        int returnCode = getFileLineTokens(line, recordsList, diseaseHashTable, countryHashTable);
        if(returnCode == ERROR) //If return code is ERROR then either we have a duplicate ID or we have a wrong dates order
        {
            fclose(*filePointer);
            free(line);
            return ERROR;   //So return ERROR code and exit programm.
        }
    }
    
    fclose(*filePointer);
    if (line)
    {
        free(line);
    }
    return SUCCESS;
}

//Function to tokenize each line and then add this record at each structure(RecordsList and the hashTables)
int getFileLineTokens(char* line, RecordsList recordsList, HashTable diseaseHashTable, HashTable countryHashTable)
{    
    //Copy of each line to get tokens from
    char* copyOfLine = malloc(sizeof(char*) * strlen(line)+1);
    strcpy(copyOfLine, line);

    char* record;   //contains a token from strtok

    char* recordId = NULL;
    char* patientFirstName = NULL;
    char* patientLastName = NULL;
    char* diseaseId = NULL;
    char* country = NULL;
    char* entryDateString = NULL;
    char* exitDateString = NULL;

    record = strtok(copyOfLine, " ");   //get first token
    

    recordId = malloc(sizeof(char*) * strlen(record) + 1);
    strcpy(recordId, record);   //copy it to recordId

    record = strtok(NULL, " ");
    patientFirstName = malloc(sizeof(char*) * strlen(record) + 1);
    strcpy(patientFirstName, record);

    record = strtok(NULL, " ");
    patientLastName = malloc(sizeof(char*) * strlen(record) + 1);
    strcpy(patientLastName, record);

    record = strtok(NULL, " ");
    diseaseId = malloc(sizeof(char*) * strlen(record) + 1);
    strcpy(diseaseId, record);    

    record = strtok(NULL, " ");
    country = malloc(sizeof(char*) * strlen(record) + 1);
    strcpy(country, record);

    record = strtok(NULL, " ");
    entryDateString = malloc(sizeof(char*) * strlen(record) + 1);
    strcpy(entryDateString, record);

    record = strtok(NULL, " ");
    exitDateString = malloc(sizeof(char*) * strlen(record) + 1);
    strcpy(exitDateString, record);

    record = strtok(NULL, " ");

    DateStructure entryDate = setDate(entryDateString); //Convert date string to date structure
    DateStructure exitDate = setDate(exitDateString);   //Convert date string to date structure
    if(exitDate->day != 0 && datesOrderValidation(entryDate, exitDate) == ERROR)    //Check if dates order is correct, if not then exit
    {
        printf("Record Ignored !\n");
        destroyDate(entryDate);
        destroyDate(exitDate);
        free(recordId);
        freeFileReaderVariables(patientFirstName, patientLastName, diseaseId, country, entryDateString, exitDateString, copyOfLine);
        return ERROR;
    }

    if(checkId(recordsList, recordId) == -1) //check if this id already exist in our list, if its in our list then exit program
    {
        printf("Error: Record with Id: %s already exists!\n", recordId);
        destroyDate(entryDate);
        destroyDate(exitDate);
        free(recordId);
        freeFileReaderVariables(patientFirstName, patientLastName, diseaseId, country, entryDateString, exitDateString, copyOfLine);
        return ERROR;
    }
    else
    {
        //Add to records list
        addRecordToList(recordsList, recordId, patientFirstName, patientLastName, diseaseId, country, entryDate, exitDate);
        
        //Insert to Hash Tables
        addToHashTable(diseaseHashTable, diseaseId, recordsList->tailNode);
        addToHashTable(countryHashTable, country, recordsList->tailNode);
    }
    
    free(recordId);
    freeFileReaderVariables(patientFirstName, patientLastName, diseaseId, country, entryDateString, exitDateString, copyOfLine);
    return SUCCESS;
}

//Function used to free our string variables
void freeFileReaderVariables(char* patientFirstName, char* patientLastName, char* diseaseId, char* country, char* entryDateString, char* exitDateString, char* copyOfLine)
{
    free(patientFirstName);
    free(patientLastName);
    free(diseaseId);
    free(country);
    free(entryDateString);
    free(exitDateString);
    free(copyOfLine);
}