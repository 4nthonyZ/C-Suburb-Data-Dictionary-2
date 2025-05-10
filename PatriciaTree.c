#include "PatriciaTree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define BITS_PER_BYTE 8

char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits){
    assert(numBits > 0 && startBit >= 0 && oldKey);
    int extraBytes = 0;
    if((numBits % BITS_PER_BYTE) > 0){
        extraBytes = 1;
    }
    int totalBytes = (numBits / BITS_PER_BYTE) + extraBytes;
    char *newStem = calloc(totalBytes, sizeof(char));
    assert(newStem);
    for(unsigned int i = 0; i < totalBytes; i++){
        newStem[i] = 0;
    }
    for(unsigned int i = 0; i < numBits; i++){
        unsigned int indexFromLeft = i % BITS_PER_BYTE;
        unsigned int offset = (BITS_PER_BYTE - indexFromLeft - 1) % BITS_PER_BYTE;
        unsigned int bitMaskForPosition = 1 << offset;
        unsigned int bitValueAtPosition = getBit(oldKey, startBit + i);
        unsigned int byteInNewStem = i / BITS_PER_BYTE;
        newStem[byteInNewStem] |= bitMaskForPosition * bitValueAtPosition;
    }
    return newStem;
}

int getBit(char *s, unsigned int bitIndex){
    assert(s && bitIndex >= 0);
    unsigned int byte = bitIndex / BITS_PER_BYTE;
    unsigned int indexFromLeft = bitIndex % BITS_PER_BYTE;
    unsigned int offset = (BITS_PER_BYTE - (indexFromLeft) - 1) % BITS_PER_BYTE;
    unsigned char byteOfInterest = s[byte];
    unsigned int offsetMask = (1 << offset);
    unsigned int maskedByte = (byteOfInterest & offsetMask);
    unsigned int bitOnly = maskedByte >> offset;
    return bitOnly;
}

// CSV reading function is the same as ass1
void Read_inputCSV(char *line, Record *record) {
    memset(record->official_code_lga, 0, sizeof(record->official_code_lga));
    memset(record->official_name_lga, 0, sizeof(record->official_name_lga));

    sscanf(line, "%d,%d,%[^,],%d,%[^,],%[^,]",  
           &record->comp20003_code,
           &record->official_code_suburb,
           record->official_name_suburb,
           &record->year,
           record->official_code_state,
           record->official_name_state);

    char *lga_code_start = strchr(line, '"');
    char *lga_code_end = NULL;
    char *lga_name_start = NULL;
    char *lga_name_end = NULL;

    if (lga_code_start) {
        lga_code_start++;
        lga_code_end = strchr(lga_code_start, '"');
        if (lga_code_end) {
            *lga_code_end = '\0';
            strcpy(record->official_code_lga, lga_code_start);
        }

        lga_name_start = strchr(lga_code_end + 1, '"');
        if (lga_name_start) {
            lga_name_start++;
            lga_name_end = strchr(lga_name_start, '"');
            if (lga_name_end) {
                *lga_name_end = '\0';
                strcpy(record->official_name_lga, lga_name_start);
            }
        }

        sscanf(lga_name_end + 2, "%lf,%lf", &record->latitude, &record->longitude);
    } else {
        char *rest_of_line = strchr(line, ',');
        for (int i = 0; i < 5; i++) {
            rest_of_line = strchr(rest_of_line + 1, ',');
        }
        sscanf(rest_of_line + 1, "%[^,],%[^,],%lf,%lf",
               record->official_code_lga,
               record->official_name_lga,
               &record->latitude,
               &record->longitude);
    }
}


//Insert Patricia tree node
PatriciaTreeNode *insertPatriciaNode(PatriciaTreeNode *root, Record *record, char *key, int bitIndex) {
    if (!root) {
        // When the root node is empty, create a new node
        PatriciaTreeNode *newNode = calloc(1, sizeof(PatriciaTreeNode));
        newNode->keyFragment = createStem(key, bitIndex, strlen(key) * BITS_PER_BYTE);
        newNode->record = record;
        newNode->bitIndex = bitIndex;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->isLeaf = 1;

        
        return newNode;
    }

    // If the current node is a leaf node
    if (root->isLeaf) {
        int diffBit = 0;
        // Find the first bit that is different
        while (getBit(root->keyFragment, diffBit) == getBit(key, diffBit)) {
            diffBit++;
        }


        PatriciaTreeNode *newNode = calloc(1, sizeof(PatriciaTreeNode));

        newNode->keyFragment = createStem(key, bitIndex, strlen(key) * BITS_PER_BYTE);
        newNode->record = record;
        newNode->bitIndex = diffBit;
        newNode->isLeaf = 1;


        PatriciaTreeNode *internalNode = calloc(1, sizeof(PatriciaTreeNode));
        internalNode->bitIndex = diffBit;
        internalNode->isLeaf = 0;

        if (getBit(key, diffBit) == 0) {
            internalNode->left = newNode;
            internalNode->right = root;
        } else {
            internalNode->left = root;
            internalNode->right = newNode;
        }
        return internalNode;
    }


    if (getBit(key, root->bitIndex) == 0) {
        root->left = insertPatriciaNode(root->left, record, key, bitIndex);
    } else {
        root->right = insertPatriciaNode(root->right, record, key, bitIndex);
    }

    return root;
}


//Very similar to ass1's free, only slightly modified traversal order
void Free_tree(PatriciaTreeNode *root) {
    if (root == NULL) {
        return;
    }


    Free_tree(root->left);


    Free_tree(root->right);


    if (root->record != NULL) {
        free(root->record);
    }
    if (root->keyFragment != NULL) {
        free(root->keyFragment);  // 释放 createStem 中分配的内存
    }

    free(root);
}


// same as ass1
void printRecord(Record *record, const char *searchKey, int record_count, FILE *output) {
    if (record != NULL) {

        fprintf(output, "%s --> \n", searchKey);

        fprintf(output, "COMP20003 Code: %d, Official Code Suburb: %d, Official Name Suburb: %s, Year: %d, Official Code State: %s, Official Name State: %s, Official Code Local Government Area: %s, Official Name Local Government Area: %s, Latitude: %.7f, Longitude: %.7f\n",
                record->comp20003_code,
                record->official_code_suburb,
                record->official_name_suburb,
                record->year,
                record->official_code_state,
                record->official_name_state,
                record->official_code_lga,
                record->official_name_lga,
                record->latitude,
                record->longitude);
    } else {
        fprintf(output, "Record is NULL.\n");
    }
}





Record *searchPatriciaTree(PatriciaTreeNode *root, char *key, int *b, int *n, int *s) {
    if (!root) {
        return NULL;  // If the tree is empty, return NULL
    }

    PatriciaTreeNode *currentNode = root;
    PatriciaTreeNode *closestNode = NULL;
    int minDistance = INT_MAX;
    int foundExactMatch = 0;
    int prevBitIndex = -1;  

    while (currentNode != NULL) {
        (*n)++;  // Increase the number of node visits


        if (prevBitIndex != -1) {
            *b += (currentNode->bitIndex - prevBitIndex);  // Accumulate the number of bits
        } else {
            *b += currentNode->bitIndex;  
        }
        prevBitIndex = currentNode->bitIndex;  

        // If it is a leaf node, check whether it is an exact match
        if (currentNode->isLeaf) {
            int distance = editDistance(key, currentNode->record->official_name_suburb, 
                                        strlen(key), strlen(currentNode->record->official_name_suburb));
            (*s)++;  

            
            if (distance == 0) {
                foundExactMatch = 1;
                return currentNode->record; 
            }

            // If there is no exact match, update the closest node
            if (distance < minDistance) {
                minDistance = distance;
                closestNode = currentNode;
            }

            break;  
        }


        if (getBit(key, currentNode->bitIndex) == 0) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }


    if (!foundExactMatch && closestNode != NULL) {
        return closestNode->record;  // Returns the most recent matching record
    }

    return NULL;  
}







int editDistance(char *str1, char *str2, int n, int m) {
    int dp[n + 1][m + 1];

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            if (i == 0) {
                dp[i][j] = j;
            }
            else if (j == 0) {
                dp[i][j] = i;
            }
            else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else {
                dp[i][j] = 1 + ((dp[i - 1][j] < dp[i][j - 1]) 
                                ? (dp[i - 1][j] < dp[i - 1][j - 1] ? dp[i - 1][j] : dp[i - 1][j - 1])
                                : (dp[i][j - 1] < dp[i - 1][j - 1] ? dp[i][j - 1] : dp[i - 1][j - 1]));
            }
        }
    }

    return dp[n][m];
}



