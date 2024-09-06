//#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 8
#define RECURSION_MAX 98
//extern void canary_setup(void);
//extern unsigned long canary_value;

int selection;
unsigned short stack_funcs = 0;
unsigned long canary_value;

//Canary Functions
void canary_tripped(void) {
    printf("AAAJ");
}

void canary_setup(void)
{
     canary_value = 0xBAAAAAAD;//provide some magic numbers
}

void print_memory_status(unsigned long local_canary, char* buffer) {
    printf("canary value: %p\tglobal canary value: %p\tbuffer memory: %p\n",local_canary,canary_value,buffer);
    printf("canary address: %p\tglobal canary address %p\tbuffer address: %p\nbuffer value: ", (void*)&local_canary,(void*)&canary_value,(void*)&buffer);
}

//Functions for the actual tests
void program_test() {
    //This is a prime cruncher to test speed & mem
}

void canary_test() {
    //This is roppable
    unsigned long canary;
    memcpy(&canary, &canary_value, sizeof(canary_value));
    char buf[BUFFER_SIZE];
    unsigned int read_bytes;

    print_memory_status(canary, buf);

    printf("Enter the number of maximum bytes to be read: ");
    scanf("%d", &read_bytes);
    printf("Enter what to write to the buffer: ");
    read(0,buf,read_bytes);

    print_memory_status(canary, buf);
    printf("%s",buf);

    if (memcmp(&canary,&canary_value,sizeof(canary_value))) {
        canary_tripped();
    }
}

void recursive_function_to_have_many_locals_on_the_stack() {
    unsigned long canary;
    memcpy(&canary, &canary_value, sizeof(canary_value));
    if (stack_funcs < RECURSION_MAX) {
        recursive_function_to_have_many_locals_on_the_stack();
    } else if (selection == 1) {
        program_test();
    } else if (selection == 2) {
        canary_test();
    }


    if (memcmp(&canary,&canary_value,sizeof(canary_value))) {
        canary_tripped();
    }
}

int main() {
    canary_setup();

    unsigned long canary;



    
    printf("Enter '1' to perform a program test.\nEnter '2' to perform a canary test\nInput: ");
    scanf("%d", &selection);

    memcpy(&canary, &canary_value, sizeof(canary_value));
    

    //gets(buf);


    if (memcmp(&canary,&canary_value,sizeof(canary_value))) {
        canary_tripped();
    }
}