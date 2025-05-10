#ifndef PATRICIATREE_H
#define PATRICIATREE_H 8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int comp20003_code;
    int official_code_suburb;
    char official_name_suburb[100];
    int year;
    char official_code_state[10];
    char official_name_state[100];
    char official_code_lga[256];
    char official_name_lga[256];
    double latitude;
    double longitude;
} Record;

// Patricia tree
typedef struct PatriciaTreeNode {
    char *keyFragment; 
    Record *record;  
    int bitIndex; 
    struct PatriciaTreeNode *left;
    struct PatriciaTreeNode *right;
    int isLeaf;  
} PatriciaTreeNode;


//Function declaration
int getBit(char *s, unsigned int bitIndex);
char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits);
void Read_inputCSV(char *line, Record *record);
int editDistance(char *str1, char *str2, int n, int m);
PatriciaTreeNode *insertPatriciaNode(PatriciaTreeNode *root, Record *record, char *key, int bitIndex);
Record *searchPatriciaTree(PatriciaTreeNode *root, char *key, int *b, int *n, int *s);
void printRecord(Record *record, const char *searchKey, int record_count, FILE *output);
void Free_tree(PatriciaTreeNode *root);
#endif
