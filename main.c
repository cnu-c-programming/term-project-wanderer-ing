/*
 * main.c  –  Mini Student Shell
 *
 * TODO: Implement admin_shell and client_shell.
 *
 * Build:
 *   make admin   →  admin_shell  (compiled with -DADMIN_MODE)
 *   make client  →  client_shell (compiled with -DCLIENT_MODE)
 *
 * Usage:
 *   ./admin_shell [students.csv]
 *   ./admin_shell -f commands.txt [students.csv]
 *   ./client_shell [students.csv]
 *   ./client_shell -f commands.txt [students.csv]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Add your own header includes here */
 #include "student.h"  
 #include "file_io.h"  
 #include "command.h"  

/* ---------------------------------------------------------------
 * TODO: Implement the interactive shell loop.
 *   - Print a prompt and read a line from stdin.
 *   - Parse the line into a command and arguments.
 *   - Dispatch to the appropriate handler function.
 *   - Loop until the user types "exit" or EOF.
 * --------------------------------------------------------------- */
void run_shell(const char *csv_path) {
    Student* head = load_csv(csv_path);
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

        input[strcspn(input, "\n")] = '\0';
        char* cmd = strtok(input, " "); 
        
        if (cmd == NULL) {
            continue; 
        }

        if (strcmp(cmd, "exit") == 0) {
            break;
        } 
        else if (strcmp(cmd, "list") == 0) {
            handle_list(head);
        } 
        else if (strcmp(cmd, "add") == 0) {
            handle_add(&head);
        } 
        else if(strcmp(cmd, "save")==0){
            save_csv(csv_path,head);
        }
        else {
            printf("Error\n");
        }
    }
    free_students(head);
}

/* ---------------------------------------------------------------
 * TODO: Implement batch mode – read commands from a file.
 *   - Open cmd_file for reading.
 *   - Execute each line as a command (same logic as run_shell).
 *   - Close the file when done.
 * --------------------------------------------------------------- */
void run_command_file(const char *cmd_file, const char *csv_path) {
    /* TODO */
    (void)cmd_file;
    (void)csv_path;
}

int main(int argc, char *argv[]) {
    const char *csv_path  = "students.csv"; /* default CSV file */
    const char *cmd_file  = NULL;           /* -f <file> argument */

    /* TODO: Parse command-line arguments.
     *   Supported flags:
     *     -f <file>   run commands from <file> instead of stdin
     *   Remaining positional argument (if any): path to students CSV.
     *
     *   Example parsing skeleton:
     *
     *   
     */

    if (argc<2){
        #ifdef ADMIN_MODE
            printf("Usage: ./admin_shell <csv_file> [-command_file]\n");
        #else
            printf("Usage: .client_shell <csv_file> [-f command_file]\n");
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

#ifdef ADMIN_MODE
    /* Admin shell: supports add, delete, update, save, load, sort, list, find, help, exit */
    if (cmd_file) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(csv_path);
    }

#elif defined(CLIENT_MODE)
    /* Client shell: supports find, list, help, exit  (read-only) */
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
