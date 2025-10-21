#include <stdio.h>

int main(int argc, int* argv[]) {
    printf("%d\n", argc);
    printf("%s\n", *argv);
    printf("%s\n", argv[1]);
    printf("Hello, World!\n");
    return 0;
}
