#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Record {
    int comp20003_code;
    int official_code_suburb;
    char official_name_suburb[128];
    int year;
    char official_code_state[128];
    char official_name_state[128];
    char official_code_lga[128]; // 修改为单个字符串
    char official_name_lga[128]; // 修改为单个字符串
    double latitude;
    double longitude;
    struct Record *next;
} Record;




Record* Insert_Head(Record *head, Record new_record);
void Free_list(Record *head);
void Read_inputCSV(char *line, Record *record);
Record* Find_records(Record *head, FILE *output);
Record* delete_record(Record *head, FILE *output);
Record* SortRecordsByCode(Record* head);

#endif