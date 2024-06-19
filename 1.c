#include <stdio.h>
#include <stdlib.h>

int max(int x, int y) {
    int k;
    if (x > y) 
        k = x;
    else  
        k = y;
    return k;
}

void __ctt_error(int a, int b) {
    if (a != b) {
        fprintf(stderr, "Signatures are not matching\n");
        exit(0);
    }
}

int main() {
    int a = 10;
    int b = 20;

    // Correct path
    int max_value = max(a, b);
    printf("Max value: %d\n", max_value);

   
    __ctt_error(1, 2); 
    return 0;
}
