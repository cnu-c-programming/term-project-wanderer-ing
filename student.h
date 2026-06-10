#ifndef STUDENT_H
#define STUDENT_H

typedef struct Student{
    int id;
    char nme[32];
    int score;
    struct Student* next;
}Student;

#endif