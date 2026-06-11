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

Student* append_student(Student* head, int id, const char* name, int score) {
    Student* new_student = (Student*)malloc(sizeof(Student));
    if (new_student == NULL) {
        printf("Error: Memory allocation failed.\n");
        return head;
    }

    new_student->id = id;
    strcpy(new_student->name, name);
    new_student->score = score;
    new_student->next = NULL;

    if (head == NULL) {
        return new_student;
    }

    
    Student* current = head;
    while (current->next != NULL) { 
        current = current->next;
    }
    current->next = new_student; 
    return head;
}
