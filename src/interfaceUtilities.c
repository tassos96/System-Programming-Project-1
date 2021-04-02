/**
 * This file contains methods for reading command line arguments and 
 * commands writen to program interface and call the correct command.
**/
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "include/interfaceUtilities.h"
#include "include/interfaceCommands.h"

#define ARGS_SIZE 9 //Correct number of command line total arguments

/**
 *  Function for reading arguments from command line and initalize variables on main.c 
 **/
void readCommandLineArguments(char** filename, int* diseaseHashtableNumOfEntries, int* countryHashtableNumOfEntries, int* bucketSize, int argc, const char* argv[])
{
    //Check if all needed arguments passed
    if (argc < ARGS_SIZE || argc > ARGS_SIZE)
    {
        printArgsErrorMsg();    //Print error -> Arguments are missing
        exit(1);                //Terminate program
    }
    else
    {
        for (int i = 0; i < argc - 1; i++)
        {
            if (strcmp(argv[i], "-p") == 0)          //File name
            {
                *filename = malloc(strlen(argv[i+1])*sizeof(char)+1);
                strcpy(*filename, argv[i+1]);
            }
            else if (strcmp(argv[i], "-h1") == 0)    //Disease Hashtable size
            {
                *diseaseHashtableNumOfEntries = atoi(argv[i+1]);
            }
            else if (strcmp(argv[i], "-h2") == 0)    //Country Hashtable size
            {
                *countryHashtableNumOfEntries = atoi(argv[i+1]);
            }
            else if (strcmp(argv[i], "-b") == 0)     //Bucket size
            {
                *bucketSize = atoi(argv[i+1]);
            }
        }   
    }
}

/**
 * Function to make sure every argument needed passed from command line 
 **/
void checkForArgsError(char* filename, int diseaseHashtableNumOfEntries, int countryHashtableNumOfEntries, int bucketSize)
{
    if ((filename == NULL) || (diseaseHashtableNumOfEntries == -1) || (countryHashtableNumOfEntries == -1) || (bucketSize == -1))
    {
        printArgsErrorMsg();    //Print error -> Arguments are missing
        exit(2);                //Terminate program
    }  
}

/**
 * Function to print error messages for command line arguments
 **/
void printArgsErrorMsg()
{
    printf("Error occured: Something is wrong with arguments. \n");
    printf("Please try again.\n");
}

/**
 * Function to read interface command from user
 **/
int readCommandFromInterface(RecordsList recordsList, HashTable diseaseHashTable, HashTable countryHashTable) 
{
    char* inputLine = NULL; //string input from prompt
    size_t inputLength = 0; //buffer
    size_t lineSize = -1;   //total bytes readed

    lineSize = getline(&inputLine, &inputLength, stdin);    //read the line
    if (lineSize == -1)
    {
        printf("Error occured: Check again your command.\n");
        free(inputLine);
        return -1;
    }
    else
    {
        inputLine[strlen(inputLine)-1] = '\0';  //replace '\n' character with '\0' character
        
        //Run commands here
        int commandCode = runCommands(&inputLine, recordsList, diseaseHashTable, countryHashTable);
        free(inputLine);
        return commandCode;
    }
}

/**
 * Function to run user's commands 
**/
int runCommands(char** inputLine, RecordsList recordsList, HashTable diseaseHashTable, HashTable countryHashTable)
{
    int functionReturnCode = -1;
    char* command = NULL;       //command string
    char* copyOfCommand = NULL;
    char** arguments = NULL;    //command's arguments string array
    int argumentsNum = 0;       //command arguments number (contains command name)

    copyOfCommand = malloc(strlen(*inputLine)*sizeof(char)+1);
    strcpy(copyOfCommand, *inputLine);

    command = strtok(*inputLine, " ");

    argumentsNum = countCommandArguments(&command);     //get command's arguments number
    
    arguments = getCommandArguments(&arguments, &copyOfCommand, argumentsNum);  //save to string array the command's arguments
    
    free(copyOfCommand);
    
    if (strcmp(arguments[0], "/globalDiseaseStats") == 0)         //globalDiseaseStats [date1 date2]
    {
        functionReturnCode = runGlobalDiseaseStats(&arguments, argumentsNum, diseaseHashTable);
        printf("\n");   
    }
    else if (strcmp(arguments[0], "/diseaseFrequency") == 0)      //diseaseFrequency virusName [country] date1 date2
    {
        functionReturnCode = runDiseaseFrequency(&arguments, argumentsNum, diseaseHashTable);
        printf("\n");
    }
    else if (strcmp(arguments[0], "/topk-Diseases") == 0)         //topK-Diseases k country [date1 date2]
    {
        functionReturnCode = runTopKDiseases(&arguments, argumentsNum, diseaseHashTable);
        printf("\n");
    }
    else if (strcmp(arguments[0], "/topk-Countries") == 0)        //topK-Countries k disease [date1 date2]
    {
        functionReturnCode = runTopKCountries(&arguments, argumentsNum, countryHashTable);
        printf("\n");
    }
    else if (strcmp(arguments[0], "/insertPatientRecord") == 0)   //insertPatientRecord recordID patientFirstName patientLastName diseaseID country entryDate [exitDate]
    {
        functionReturnCode = runInsertPatientRecord(&arguments, argumentsNum, recordsList, diseaseHashTable, countryHashTable);
        printf("\n");
    }
    else if (strcmp(arguments[0], "/recordPatientExit") == 0)     //recordPatientExit recordID exitDate
    {
        functionReturnCode = runRecordPatientExit(&arguments, argumentsNum, recordsList);
        printf("\n");
    }
    else if (strcmp(arguments[0], "/numCurrentPatients") == 0)    //numCurrentPatients [disease]
    {
        functionReturnCode = runNumCurrentPatients(&arguments, argumentsNum, diseaseHashTable);
        printf("\n");
    }
    else if (strcmp(arguments[0], "/exit") == 0)                  //exit
    {
        functionReturnCode = 8;
    }

    freeArgumentsStringArray(&arguments, argumentsNum);
    
    if (functionReturnCode == -1)
    {
        printWrongCommandError();
    }
    return functionReturnCode;
}

/**
 * Function to count command arguments 
 **/
int countCommandArguments(char** command)
{
    int argumentsCounter = 0;   //includes command name
    while( *command != NULL ) 
    {
        argumentsCounter++;
        *command = strtok(NULL, " ");
    }
    return argumentsCounter;
}

/**
 * Function to save command's arguments to a string array
 **/
char** getCommandArguments(char*** arguments, char** copyOfCommand, int argumentsNum) 
{
    char* command = NULL;

    arguments[0] = malloc(sizeof(char*) * (argumentsNum));
    int i = 0;
    command = strtok(*copyOfCommand, " ");
    while(command != NULL) 
    {
        arguments[0][i] = malloc(sizeof(char) * strlen(command) + 1);
        strcpy(arguments[0][i], command);
        command = strtok(NULL, " ");
        i++;
    }
    return arguments[0];
}

/**
 * Function to free our string array
 **/
void freeArgumentsStringArray(char*** arguments, int argumentsNum) 
{
    for(int i = 0; i < argumentsNum; i++)
    {
        free(arguments[0][i]);
    }
    free(arguments[0]);
}