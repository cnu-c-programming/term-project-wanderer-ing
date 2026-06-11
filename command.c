#include "student.h"
#include <stdio.h>
#include <string.h>
#include "command.h"
#include <stdlib.h>

void handle_list(Student* head) {
    if (head == NULL) {
        printf("No students found.\n");
        return;
    }

    Student* current = head;
    while (current != NULL) {
        printf("%d %s %d\n", current->id, current->name, current->score);
        current = current->next;
    }
}

void handle_add(Student** head) {
    char* id_str = strtok(NULL, " ");
    char* name = strtok(NULL, " ");
    char* score_str = strtok(NULL, " ");

    if (id_str == NULL || name == NULL || score_str == NULL) {
        printf("Error: Invalid input.\n");
        return;
    }

    int id = atoi(id_str);
    int score = atoi(score_str);

    Student* current = *head;
    while (current != NULL) {
        if (current->id == id) {
            printf("Error: Student ID %d already exists.\n", id);
            return;
        }
        current = current->next;
    }

    *head = append_student(*head, id, name, score);
}