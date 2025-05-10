#include "PatriciaTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//According to my ass1 program, only a slight change is needed
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <stage> <input_csv_file> <output_file>\n", argv[0]);
        return 1;
    }

    int stage = atoi(argv[1]);
    const char *input_csv_file = argv[2];
    const char *output_file = argv[3];

    PatriciaTreeNode *root = NULL;
    FILE *file = fopen(input_csv_file, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", input_csv_file);
        return 1;
    }

    char line[1024];
    fgets(line, sizeof(line), file);  
    while (fgets(line, sizeof(line), file)) {
        Record *new_record = calloc(1, sizeof(Record));
        Read_inputCSV(line, new_record);

        root = insertPatriciaNode(root, new_record, new_record->official_name_suburb, 0);
    }
    fclose(file);

    FILE *output = fopen(output_file, "w");
    if (output == NULL) {
        printf("Error opening output file %s.\n", output_file);
        return 1;
    }

    if (stage == 4) {
        char searchKey[100];
        while (fgets(searchKey, sizeof(searchKey), stdin)) {
            searchKey[strcspn(searchKey, "\n")] = '\0';

            int b = 0, n = 0, s = 0;

            Record *result = searchPatriciaTree(root, searchKey, &b, &n, &s);

            if (result) {
                printf("%s --> 1 records - comparisons: b%d n%d s%d\n", searchKey, b, n, s);
                printRecord(result, searchKey, 1, output);
            } else {

                printf("No exact match found for: %s\n", searchKey);
            }
        }
    }

    fclose(output);
    Free_tree(root); 

    return 0;
}



