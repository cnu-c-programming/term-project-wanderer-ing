#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

void add_student(Student** head, int id, const char* name, int score){
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if(newStudent==NULL){
        printf("Error: It's full\n");
        return;
    }

    newStudent->id = id;
    strcpy(newStudent->name, name);
    newStudent->score = score;
    newStudent->next = NULL;

    if(*head == NULL){
        *head = newStudent;
        return;
    }

    Student* current = *head;
    while(current->next!= NULL){
        current = current->next;
    }
    current->next = newStudent;
}

void free_students(Student* head){
    Student* current = head;
    Student* nextNode = NULL;

    while(current!=NULL){
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
}