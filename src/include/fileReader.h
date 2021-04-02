#ifndef FILE_READER_H
#define FILE_READER_H

#include "recordsList.h"
#include "hashTables.h"

//Function to read line by line the given records file
int readRecordsFromFile(FILE** filePointer, HashTable diseaseHashTable, HashTable countryHashTable, RecordsList recordsList, char* patientRecordFileName);

//Function to tokenize each line and then add this record at each structure(RecordsList and the hashTables)
int getFileLineTokens(char* line, RecordsList recordsList, HashTable diseaseHashTable, HashTable countryHashTable);

//Function used to free our string variables
void freeFileReaderVariables(char* patientFirstName, char* patientLastName, char* diseaseId, char* country, char* entryDateString, char* exitDateString, char* copyOfLine);

#endif