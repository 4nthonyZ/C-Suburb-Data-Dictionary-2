
#include "Linked_list.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <stage> <input_csv_file> <output_file>\n", argv[0]);
        return 1;
    }

    int stage = atoi(argv[1]);
    const char *input_csv_file = argv[2];
    const char *output_file = argv[3];

    Record *head = NULL;
    FILE *file = fopen(input_csv_file, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", input_csv_file);
        return 1;
    }

    char line[1024];
    fgets(line, sizeof(line), file);  // Skip header line
    while (fgets(line, sizeof(line), file)) {
        Record new_record;
        Read_inputCSV(line, &new_record);
        head = Insert_Head(head, new_record);
    }
    fclose(file);

    FILE *output = fopen(output_file, "w");
    if (output == NULL) {
        printf("Error opening output file %s.\n", output_file);
        return 1;
    }

    if (stage == 3) {
        head = Find_records(head, output);
        
    }
    fclose(output);
    Free_list(head);

    return 0;
}
