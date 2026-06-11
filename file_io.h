#pragma once
#include "student.h"

Student* load_csv(const char* filename); 

void save_csv(const char* filename, Student* head);