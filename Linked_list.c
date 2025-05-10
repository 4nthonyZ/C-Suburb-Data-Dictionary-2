#include "Linked_list.h"

Record* Insert_Head(Record* head, Record new_record_data) {
    Record *new_node = (Record *)malloc(sizeof(Record));
    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        return head;
    }
    *new_node = new_record_data;
    new_node->next = head;
    return new_node;
}
void Free_list(Record *head) {
    Record *current = head;
    Record *next;

    while (current != NULL) {
        next = current->next;
        free(current);  // 释放当前节点的内存
        current = next;
    }
}


Record* Find_records(Record *head, FILE *output) {
    char suburb_to_search[100];

    // 对链表进行排序（如果需要排序）
    head = SortRecordsByCode(head);

    while (1) {
        if (fgets(suburb_to_search, sizeof(suburb_to_search), stdin) == NULL) {
            break;  // if EOF, break
        }

        suburb_to_search[strcspn(suburb_to_search, "\n")] = 0;

        if (strlen(suburb_to_search) == 0) {
            break;
        }

        Record *current = head;
        int found = 0;
        int n_comparisons = 0;  // 节点访问次数
        int s_comparisons = 0;  // 字符串比较次数
        int b_comparisons = 0;  // 字符比较次数初始化为0

        // 打印子名标题
        fprintf(output, "%s -->\n", suburb_to_search);

        // 遍历链表进行查找
        while (current != NULL) {
            n_comparisons++;  // 每访问一个节点，节点比较次数加1

            s_comparisons++;  // 每次进行字符串比较时，增加1

            // 遍历字符并进行逐个字符比较计数
            for (int i = 0; i < strlen(suburb_to_search) + 1; i++) {
                b_comparisons += 8;  // 每次比较一个字符，加8位
                if (suburb_to_search[i] != current->official_name_suburb[i]) {
                    break;  // 一旦遇到不匹配字符，停止比较
                }
            }

            // 比较当前节点的名称和输入的子名
            if (strcmp(current->official_name_suburb, suburb_to_search) == 0) {
                // 找到了匹配的记录，输出记录内容
                fprintf(output, "COMP20003 Code: %d, Official Code Suburb: %d, Official Name Suburb: %s, Year: %d, Official Code State: %s, Official Name State: %s, Official Code Local Government Area: %s, Official Name Local Government Area: %s, Latitude: %.7f, Longitude: %.7f\n",
                        current->comp20003_code,
                        current->official_code_suburb,
                        current->official_name_suburb,
                        current->year,
                        current->official_code_state,
                        current->official_name_state,
                        current->official_code_lga,
                        current->official_name_lga,
                        current->latitude,
                        current->longitude);
                found++;
            }

            current = current->next;
        }

        if (found > 0) {
            // 找到记录时的输出
            printf("%s --> %d records - comparisons: b%d n%d s%d\n", 
                    suburb_to_search, found, b_comparisons, n_comparisons, s_comparisons);
        } else {
            // 未找到记录时的输出
            printf("%s --> NOTFOUND\n", suburb_to_search);
        }
    }
    return head;
}






void Read_inputCSV(char *line, Record *record) {
    // 初始化 lga_count
    memset(record->official_code_lga, 0, sizeof(record->official_code_lga));
    memset(record->official_name_lga, 0, sizeof(record->official_name_lga));

    // 使用 sscanf 读取前几个字段，但将 LGA 和经纬度部分留给手动处理
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
        lga_code_start++; // 跳过第一个引号

        // 找到第二个引号，表示LGA代码部分的结束
        lga_code_end = strchr(lga_code_start, '"');
        if (lga_code_end) {
            *lga_code_end = '\0'; // 将结束引号替换为字符串结束符
            strcpy(record->official_code_lga, lga_code_start);
        }

        // 找到第三个引号，表示LGA名称部分的开始
        lga_name_start = strchr(lga_code_end + 1, '"');
        if (lga_name_start) {
            lga_name_start++; // 跳过引号

            // 找到第四个引号，表示LGA名称部分的结束
            lga_name_end = strchr(lga_name_start, '"');
            if (lga_name_end) {
                *lga_name_end = '\0'; // 将结束引号替换为字符串结束符
                strcpy(record->official_name_lga, lga_name_start);
            }
        }

        // 提取纬度和经度
        sscanf(lga_name_end + 2, "%lf,%lf", &record->latitude, &record->longitude);
    } else {
        // 没有双引号时的处理：从正确位置开始读取 LGA 代码、名称、纬度和经度
        char *rest_of_line = strchr(line, ',');
        for (int i = 0; i < 5; i++) {
            rest_of_line = strchr(rest_of_line + 1, ',');  // 前进到第6个逗号位置
        }
        sscanf(rest_of_line + 1, "%[^,],%[^,],%lf,%lf",
               record->official_code_lga,
               record->official_name_lga,
               &record->latitude,
               &record->longitude);
    }

}




Record* SortRecordsByCode(Record* head) {
    if (!head || !head->next) {
        return head; // 如果链表为空或只有一个元素，直接返回
    }

    Record* sorted = NULL; // 排序后的链表头部

    // 遍历原链表，将节点插入到新的排序链表中
    Record* current = head;
    while (current) {
        Record* next = current->next; // 保存下一个节点
        if (!sorted || sorted->comp20003_code > current->comp20003_code) {
            current->next = sorted;
            sorted = current;
        } else {
            Record* temp = sorted;
            while (temp->next && temp->next->comp20003_code <= current->comp20003_code) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }

    return sorted; // 返回排序后的链表头部
}











Record* delete_record(Record *head, FILE *output) {
    char suburb_to_delete[100];

    head = SortRecordsByCode(head);

    while (1) {
        if (fgets(suburb_to_delete, sizeof(suburb_to_delete), stdin) == NULL) {
            break;  // 如果是EOF，结束循环
        }

        suburb_to_delete[strcspn(suburb_to_delete, "\n")] = 0;  // 去掉换行符

        if (strlen(suburb_to_delete) == 0) {
            break;  // 如果输入为空，结束循环
        }

        Record *current = head;
        Record *prev = NULL;
        int found = 0;

        while (current != NULL) {
            if (strcmp(current->official_name_suburb, suburb_to_delete) == 0) {
                if (prev == NULL) {
                    head = current->next;  // 更新头节点
                } else {
                    prev->next = current->next;
                }

                Record *to_free = current;
                current = current->next;
                free(to_free);
                found++;
            } else {
                prev = current;
                current = current->next;
            }
        }

        if (found == 0) {
            printf("%s --> NOTFOUND\n", suburb_to_delete);
        } else {
            printf("%s --> %d records deleted\n", suburb_to_delete, found);
        }
    }

    Record *current = head;
    fprintf(output, "COMP20003 Code,Official Code Suburb,Official Name Suburb,Year,Official Code State,Official Name State,Official Code Local Government Area,Official Name Local Government Area,Latitude,Longitude\n");

    while (current != NULL) {
        char official_code_lga[256];
        char official_name_lga[256];

        if (strchr(current->official_code_lga, ',') != NULL) {
            snprintf(official_code_lga, sizeof(official_code_lga), "\"%s\"", current->official_code_lga);
        } else {
            strncpy(official_code_lga, current->official_code_lga, sizeof(official_code_lga));
        }

        if (strchr(current->official_name_lga, ',') != NULL) {
            snprintf(official_name_lga, sizeof(official_name_lga), "\"%s\"", current->official_name_lga);
        } else {
            strncpy(official_name_lga, current->official_name_lga, sizeof(official_name_lga));
        }

        fprintf(output, "%d,%d,%s,%d,%s,%s,%s,%s,%.7f,%.7f\n",
                current->comp20003_code,
                current->official_code_suburb,
                current->official_name_suburb,
                current->year,
                current->official_code_state,
                current->official_name_state,
                official_code_lga,
                official_name_lga,
                current->latitude,
                current->longitude);
        current = current->next;
    }

    return head;  // 返回新的头节点
}

