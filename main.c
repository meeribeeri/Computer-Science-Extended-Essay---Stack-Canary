#include "Include/stack.h"

int main() {
    canary_setup();

    
    char buf[8];
    gets(buf);
    printf("%s",buf);
}