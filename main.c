#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "file_io.h"
#include "command.h"

const char* g_csv_path = NULL;

void run_shell(const char *csv_path) {
    Student* head = load_csv(csv_path);
    g_batch_line=0;

    #ifdef ADMIN_MODE
        printf("[Admin Program]\n");
    #else
        printf("[Client Program]\n");
    #endif

    int count = 0;
    Student* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    printf("Loaded %d students from %s.\n", count, csv_path);

    char input[100];
    while (1) {
        #ifdef ADMIN_MODE
            printf("admin> ");
        #else
            printf("client> ");
        #endif

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\r\n")] = '\0';
        
        char* cmd_name = strtok(input, " ");
        if (cmd_name == NULL) {
            continue;
        }

        char* args = strtok(NULL, ""); 

        int found = 0;
        for (int i = 0; i < num_commands; i++) {
            if (strcmp(cmd_name, commands[i].name) == 0) {
                ShellResult res = commands[i].handler(args, &head);
                if (res == SHELL_EXIT) {
                    free_students(head);
                    return;
                }
                found = 1;
                break;
            }
        }
        
        if (!found) {
            printf("Unknown command or permission denied.\n");
        }
    }
    free_students(head);
}

void run_command_file(const char *cmd_file, const char *csv_path) {
    Student* head = load_csv(csv_path);
    FILE* file = fopen(cmd_file, "r");
    
    if (file == NULL) {
        printf("Error: Could not open command file.\n");
        free_students(head);
        return;
    }

    #ifdef ADMIN_MODE
        printf("[Admin Program]\n");
    #else
        printf("[Client Program]\n");
    #endif

    int count = 0;
    Student* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    printf("Loaded %d students from %s.\n", count, csv_path);

    char input[100];
    int line_num = 1;
    
    while (fgets(input, sizeof(input), file) != NULL) {
        input[strcspn(input, "\r\n")] = '\0';
        
        char original_input[100];
        strcpy(original_input, input);

        char* cmd_name = strtok(input, " ");
        
        if (cmd_name == NULL || cmd_name[0] == '#') {
            line_num++;
            continue;
        }

        printf("[command file:%d] %s\n", line_num, original_input);
        
        char* args = strtok(NULL, ""); 

        int found = 0;
        g_batch_line=line_num;

        for (int i = 0; i < num_commands; i++) {
            if (strcmp(cmd_name, commands[i].name) == 0) {
                ShellResult res = commands[i].handler(args, &head);
                if (res == SHELL_EXIT) {
                    fclose(file);
                    free_students(head);
                    return;
                }
                found = 1;
                break;
            }
        }
        
        if (!found) {
            print_error("Unknown command or permission denied.");
        }
        line_num++;
    }
    
    fclose(file);
    free_students(head);
}

int main(int argc, char *argv[]) {
    const char *csv_path = "students.csv";
    const char *cmd_file = NULL;

    if (argc < 2){
        #ifdef ADMIN_MODE
            printf("Usage: ./admin_shell <csv_file> [-f command_file]\n");
        #else
            printf("Usage: ./client_shell <csv_file> [-f command_file]\n");
        #endif
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            cmd_file = argv[++i];
        } else {
            csv_path = argv[i];
        }
    }

    g_csv_path = csv_path;

    #ifdef ADMIN_MODE
        if (cmd_file) {
            run_command_file(cmd_file, csv_path);
        } else {
            run_shell(csv_path);
        }
    #elif defined(CLIENT_MODE)
        if (cmd_file) {
            run_command_file(cmd_file, csv_path);
        } else {
            run_shell(csv_path);
        }
    #else
        #error "Define either -DADMIN_MODE or -DCLIENT_MODE when compiling."
    #endif

    return 0;
}