#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"
#include "file_io.h"
#include <ctype.h>

extern const char* g_csv_path;

int is_all_digits(const char* str) {
    if (!str || *str == '\0') return 0;
    if (*str == '-' || *str == '+') str++;
    if (*str == '\0') return 0;
    while (*str) {
        if (!isdigit((unsigned char)*str)) return 0;
        str++;
    }
    return 1;
}

#ifdef ADMIN_MODE
Command commands[] = {
    {"save",   handle_save,   "save",                    "Save students to CSV"},
    {"reload", handle_reload, "reload",                  "Reload students from CSV"},
    {"add",    handle_add,    "add <id> <name> <score>", "Add a student"},
    {"delete", handle_delete, "delete <id>",             "Delete a student"},
    {"update", handle_update, "update <id> <score>",     "Update student score"},
    {"find",   handle_find,   "find <id>",               "Find student"},
    {"list",   handle_list,   "list",                    "List students"},
    {"stats",  handle_stats,  "stats",                   "Show statistics"},
    {"help",   handle_help,   "help",                    "Show help"},
    {"clear",  handle_clear,  "clear",                   "Clear screen"},
    {"exit",   handle_exit,   "exit",                    "Exit shell"}
};
int num_commands = 11;
#endif

#ifdef CLIENT_MODE
Command commands[] = {
    {"reload", handle_reload, "reload",     "Reload students from CSV"},
    {"find",   handle_find,   "find <id>",  "Find student"},
    {"list",   handle_list,   "list",       "List students"},
    {"stats",  handle_stats,  "stats",      "Show statistics"},
    {"help",   handle_help,   "help",       "Show help"},
    {"clear",  handle_clear,  "clear",      "Clear screen"},
    {"exit",   handle_exit,   "exit",       "Exit shell"}
};
int num_commands = 7;
#endif

ShellResult handle_save(char* args, Student** head) {
    (void)args;
    save_csv(g_csv_path, *head);
    int count = 0;
    Student* temp = *head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    printf("Saved %d students to %s.\n", count, g_csv_path);
    return SHELL_OK;
}

ShellResult handle_reload(char* args, Student** head) {
    (void)args;
    free_students(*head);
    *head = load_csv(g_csv_path);
    int count = 0;
    Student* temp = *head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    printf("Reloaded %d students from %s.\n", count, g_csv_path);
    return SHELL_OK;
}

ShellResult handle_add(char* args, Student** head) {
    if (!args) {
        printf("Error: missing arguments.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }
    char* id_str = strtok(args, " ");
    char* name = strtok(NULL, " ");
    char* score_str = strtok(NULL, " ");
    
    if (!id_str || !name || !score_str) {
        printf("Error: missing arguments.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    if (!is_all_digits(id_str)) {
        print_error("Error: invalid ID.");
        return SHELL_ERR_INVALID_ARGUMENT;
    }
    if (!is_all_digits(score_str)) {
        print_error("Error: invalid score.");
        return SHELL_ERR_INVALID_SCORE;
    }

    int id = atoi(id_str);
    int score = atoi(score_str);

    if (id <= 0) {
        print_error("Error: invalid ID.");
        return SHELL_ERR_INVALID_ARGUMENT;
    }
    if (score < 0 || score > 100) {
        print_error("Error: invalid score.");
        return SHELL_ERR_INVALID_SCORE;
    }

    Student* check = *head;
    while (check != NULL) {
        if (check->id == id) {
            print_error("Error: duplicate ID.");
            return SHELL_ERR_DUPLICATE_STUDENT;
        }
        check = check->next;
    }

    *head = append_student(*head, id, name, score);
    printf("Student added.\n");
    return SHELL_OK;
}

ShellResult handle_delete(char* args, Student** head) {
    if (!args) { print_error("Error: missing arguments."); return SHELL_ERR_MISSING_ARGUMENT; }
    if (!is_all_digits(args)) { print_error("Error: invalid ID."); return SHELL_ERR_INVALID_ARGUMENT; }
    
    int id = atoi(args);
    Student* check = *head;
    int found = 0;
    while (check != NULL) {
        if (check->id == id) { found = 1; break; }
        check = check->next;
    }
    if (!found) { print_error("Error: student not found."); return SHELL_ERR_STUDENT_NOT_FOUND; }

    *head = delete_student(*head, id);
    printf("Student deleted.\n");
    return SHELL_OK;
}

ShellResult handle_update(char* args, Student** head) {
    if (!args) {
        printf("Error: missing arguments.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }
    char* id_str = strtok(args, " ");
    char* score_str = strtok(NULL, " ");
    
    if (!id_str || !score_str) {
        printf("Error: missing arguments.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    if (!is_all_digits(id_str)) { print_error("Error: invalid ID."); return SHELL_ERR_INVALID_ARGUMENT; }
    if (!is_all_digits(score_str)) { print_error("Error: invalid score."); return SHELL_ERR_INVALID_SCORE; }

    int id = atoi(id_str);
    int score = atoi(score_str);

    if (id <= 0) { print_error("Error: invalid ID."); return SHELL_ERR_INVALID_ARGUMENT; }
    if (score < 0 || score > 100) { print_error("Error: invalid score."); return SHELL_ERR_INVALID_SCORE; }

    Student* check = *head;
    int found = 0;
    while (check != NULL) {
        if (check->id == id) { check->score = score; found = 1; break; }
        check = check->next;
    }
    if (!found) { print_error("Error: student not found."); return SHELL_ERR_STUDENT_NOT_FOUND; }

    printf("Student updated.\n");
    return SHELL_OK;
}

ShellResult handle_find(char* args, Student** head) {
    if (!args) {
        printf("Error: missing arguments.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }
    int id = atoi(args);
    find_student(*head, id);
    return SHELL_OK;
}

ShellResult handle_list(char* args, Student** head) {
    (void)args;
    print_students(*head);
    return SHELL_OK;
}

ShellResult handle_stats(char* args, Student** head) {
    (void)args;
    show_stats(*head);
    return SHELL_OK;
}

ShellResult handle_help(char* args, Student** head) {
    (void)args;
    (void)head;
    printf("Commands:\n");
    for (int i = 0; i < num_commands; i++) {
        printf("%-28s %s\n", commands[i].usage, commands[i].description);
    }
    return SHELL_OK;
}

ShellResult handle_clear(char* args, Student** head) {
    (void)args;
    (void)head;
    printf("\033[2J\033[H");
    return SHELL_OK;
}

ShellResult handle_exit(char* args, Student** head) {
    (void)args;
    (void)head;
    printf("Goodbye.\n");
    return SHELL_EXIT;
}