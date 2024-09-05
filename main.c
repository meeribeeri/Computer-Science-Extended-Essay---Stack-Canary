#include "Include/stack.h"

extern void canary_setup(void);
extern unsigned long canary_value;

int main() {
    canary_setup();

    unsigned long canary;
    memcpy(&canary, &canary_value, sizeof(canary_value));
    //puts(canary_value);
    //puts(canary);
    char buf[8];
    //gets(buf);
    //puts(canary_value);
    //puts(canary);
    printf("%s",buf);
}