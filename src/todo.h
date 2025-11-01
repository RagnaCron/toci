#ifndef TODO_H
#define TODO_H

#include <stdbool.h>

int list_todos(const char *file_name, const char *option);
int check_todos(const char *file_name, const char *option, bool isCheck);
int new_todos(const char *file_name);
int delete_todos(const char *file_name, const char *option);
int fix_todos(const char *file_name);

#endif // !TODO_H
