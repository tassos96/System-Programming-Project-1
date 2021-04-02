#ifndef INTERFACE_UTILITIES_H
#define INTERFACE_UTILITIES_H

#include "hashTables.h"

//Function for reading arguments from command line and initalize variables on main.c
void readCommandLineArguments(char** filename, int* diseaseHashtableNumOfEntries, int* countryHashtableNumOfEntries, int* bucketSize, int argc, const char* argv[]);
//Function to make sure every argument needed passed from command line
void checkForArgsError(char* filename, int diseaseHashtableNumOfEntries, int countryHashtableNumOfEntries, int bucketSize);
//Function to print error messages for command line arguments
void printArgsErrorMsg();
//Function to read commands from program's interface
int readCommandFromInterface(RecordsList recordsList, HashTable diseaseHashTable, HashTable countryHashTable);
//Function to run commands
int runCommands(char** inputLine, RecordsList recordsList, HashTable diseaseHashTable, HashTable countryHashTable);
//Function to count command's arguments number
int countCommandArguments(char** command);
//Function to free our string array
void freeArgumentsStringArray(char*** arguments, int argumentsNum);

#endif