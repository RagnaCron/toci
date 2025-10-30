#ifndef TODO_H
#define TODO_H

#include <stdbool.h>

int listTodos(const char *file_name, const char *option);
int checkTodos(const char *file_name, const char *option, bool isCheck);
int newTodos(const char *file_name);
int deleteTodos(const char *file_name, const char *option);
int fixTodos(const char *file_name);

#endif // !TODO_H
