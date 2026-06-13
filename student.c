#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

int g_batch_line = 0;

void print_error(const char* msg) {
    if (g_batch_line > 0) {
        printf("%s Skipped line %d.\n", msg, g_batch_line);
    } else {
        printf("%s\n", msg);
    }
}

void add_student(Student** head, int id, const char* name, int score){
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if(newStudent==NULL){
        print_error("Error: It's full");
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
        print_error("Error: Memory allocation failed.");
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

Student* delete_student(Student* head, int id) {
    if (head == NULL) {
        print_error("Error: student not found.");
        return NULL;
    }

    if (head->id == id) {
        Student* temp = head;      
        head = head->next;         
        free(temp);                
        return head;
    }

    Student* current = head;
    while (current->next != NULL && current->next->id != id) {
        current = current->next;
    }

    if (current->next == NULL) {
        print_error("Error: student not found.");
        return head;
    }

    Student* temp = current->next;       
    current->next = temp->next;          
    free(temp);                          

    return head;
}

void update_student(Student* head, int id, const char* new_name, int new_score) {
    (void)new_name; 
    
    Student* current = head;
    while (current != NULL) {
        if (current->id == id) {
            current->score = new_score;
            return; 
        }
        current = current->next; 
    }

    print_error("Error: student not found."); 
}

void find_student(Student* head, int id) {
    Student* current = head;
    
    while (current != NULL) {
        if (current->id == id) {
            printf("ID: %d\n", current->id);
            printf("Name: %s\n", current->name);
            printf("Score: %d\n", current->score);
            return; 
        }
        current = current->next; 
    }

    print_error("Error: student not found."); 
}

void show_stats(Student* head) {
    if (head == NULL) {
        printf("No student data available.\n");
        return;
    }

    int count = 0;
    int sum = 0;
    int max = -1;
    int min = 101;
    Student* current = head;

    while (current != NULL) {
        count++;
        sum += current->score;
        
        if (current->score > max) {
            max = current->score;
        }
        if (current->score < min) {
            min = current->score;
        }
        
        current = current->next;
    }

    printf("Count: %d\n", count);
    printf("Average: %.1f\n", (double)sum / count);
    printf("Max: %d\n", max);
    printf("Min: %d\n", min);
}

void print_students(Student* head) {
    if (head == NULL) {
        printf("No students found.\n");
        return;
    }
    
    printf("ID   Name       Score\n");
    Student* current = head;
    while (current != NULL) {
        printf("%-4d %-10s %d\n", current->id, current->name, current->score);
        current = current->next;
    }
}