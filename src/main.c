#include <stdio.h>

int main(const int argc, const char* argv[]) {
    printf("%d\n", argc);
    printf("%s\n", *argv);
    printf("%s\n", argv[1]);
    printf("Hello, World!\n");
    return 0;
}
