#ifndef INTERFACE_COMMANDS_H
#define INTERFACE_COMMANDS_H

#include "hashTables.h"
#include "myDateStructure.h"
#include "maxHeap.h"


//Function to print wrong command error
void printWrongCommandError();
//Function to save command's arguments to a string array
char** getCommandArguments(char*** arguments, char** copyOfCommand, int argumentsNum);
//Run /globalDiseaseStats [date1 date2] 
int runGlobalDiseaseStats(char*** arguments, int argumentsNum, HashTable diseaseHashTable); 
//Run /diseaseFrequency virusName [country] date1 date2
int runDiseaseFrequency(char*** arguments, int argumentsNum, HashTable diseaseHashTable);
//Run /topK-Diseases k country [date1 date2]
int runTopKDiseases(char*** arguments, int argumentsNum, HashTable diseaseHashTable);
//Run /topK-Countries k disease [date1 date2]
int runTopKCountries(char*** arguments, int argumentsNum, HashTable countyHashTable);
//Run /insertPatientRecord recordID patientFirstName patientLastName diseaseID entryDate [exitDate]
int runInsertPatientRecord(char*** arguments, int argumentsNum, RecordsList recordsList, HashTable diseaseHashTable, HashTable countryHashTable);
//Run /recordPatientExit recordID exitDate
int runRecordPatientExit(char*** arguments, int argumentsNum, RecordsList recordsList);
//Run /numCurrentPatients [disease]
int runNumCurrentPatients(char*** arguments, int argumentsNum, HashTable diseaseHashTable);

#endif