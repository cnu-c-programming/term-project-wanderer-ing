#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "student.h"

Student* load_csv(const char* filename){
    FILE* file = fopen(filename, "r");
    if(file==NULL){
        return NULL;
    }
    Student* head = NULL;
    Student* tail = NULL;
    char line[100];

    if(fgets(line, sizeof(line), file)==NULL){
        fclose(file);
        return NULL;
    }

    while(fgets(line, sizeof(line), file)!=NULL){
        int id;
        int score;
        char name[32];

        if(sscanf(line, "%d,%31[^,],%d" , &id, name, &score)==3){
            Student* new_student = (Student*)malloc(sizeof(Student));
            if(new_student==NULL){
                continue;
            }
            new_student->id = id;
            strcpy(new_student->name, name);
            new_student->score = score;
            new_student->next = NULL;

            if(head==NULL){
                head=new_student;
            } else{
                tail->next = new_student;
            }
            tail=new_student;
        }
    }
    fclose(file);
    return head;
}

void save_csv(const char* filename, Student* head) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not save to file.\n");
        return;
    }

    fprintf(file, "id,name,score\n");

    Student* current = head;
    while (current != NULL) {
        fprintf(file, "%d,%s,%d\n", current->id, current->name, current->score);
        current = current->next;
    }

    fclose(file);
}