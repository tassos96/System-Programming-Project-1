#ifndef RECORD_H
#define RECORD_H

#include <stdio.h>
#include <stdlib.h>

#include "myDateStructure.h"


typedef struct Record
{
    //Record Info
    char* recordID;
    char* patientFirstName;
    char* patientLastName;
    char* diseaseID;
    char* country;
    // Date entryDate;
    // Date exitDate;
    DateStructure entryDate;
    DateStructure exitDate;

    //List's Next Record
    struct Record* next;

}Record;

typedef Record* RecordListNode;

void initializeRecord(RecordListNode record, char* recordID, char* patientFirstName, char* patientLastName, char* diseaseID, char* country, DateStructure entryDate, DateStructure exitDate);

#endif