#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

long double a = 0xFFFFFFFFFFFFFFFF;
uint64_t b = 0;
unsigned long long int c = 0;

int main() {


    printf("\n%i\n",sizeof(a));
    printf("%i\n",sizeof(b));
    printf("%i\n",sizeof(c));
    printf("%d\n",a);
}