//#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>



#define BUFFER_SIZE 8
#define RECURSION_MAX 98
#define PRIMES 1000
#define CANARY_MAX 0xFF
#define CANARY_MIN 0

int selection;
uint8_t canary_value;

//Canary Functions
void canary_tripped(void) {
    printf("Canary Has Been Detected To Be Changed. Restarting Function");
}

void canary_setup(void) {
     canary_value = rand() % (CANARY_MAX - CANARY_MIN + 1) + CANARY_MIN;
     printf("%x\n", canary_value);
}

void print_memory_status(uint8_t local_canary, char* buffer) {
    printf("canary value: %p\tglobal canary value: %p\tbuffer memory: %p\n",local_canary,canary_value,buffer);
    printf("canary address: %p\tglobal canary address %p\tbuffer address: %p\nbuffer value: ", (void*)&local_canary,(void*)&canary_value,(void*)&buffer);
}

//function to know that canary has been bypassed fully
void bypassed() {
    printf("Canary Bypassed");
}

//Functions for the actual tests

void prime_test() {
    //This is a prime cruncher to test cpu
    uint8_t canary;
    memcpy(&canary, &canary_value, sizeof(canary_value));

    int current_num = 2;
    int index = 0;
    int divisor;
    bool prime;
    int primes_calculated[PRIMES];
    while (true) {
        divisor = current_num - 1;
        prime = true;
        while (divisor > 1) {

            if (current_num % divisor == 0) {
                prime = false;
                break;
            }
            divisor--;
        }
        if (prime) {
            primes_calculated[index] = current_num;
            index++;
            //printf("Prime Checked: %d\n",current_num);

        }
        if (index >= PRIMES) {
            break;
        }
        current_num++;
    }

    printf("PRIMES");
    for (int i = 0; i < PRIMES; i++) {
        printf("%d ",primes_calculated[i]);
    }

    if (memcmp(&canary,&canary_value,sizeof(canary_value))) {
        canary_tripped();
    }
}

void canary_test() {
    //This is roppable
    uint8_t canary;
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

void recursive_function_to_have_many_locals_on_the_stack(int recursion_num) {
    uint8_t canary;
    memcpy(&canary, &canary_value, sizeof(canary_value));
    if (recursion_num > 0) {
        recursive_function_to_have_many_locals_on_the_stack(recursion_num-1);
    } else if (selection == 1) {
        prime_test();
    } else if (selection == 2) {
        canary_test();
    }

    if (memcmp(&canary,&canary_value,sizeof(canary_value))) {
        canary_tripped();
    }
}

int main() {
    clock_t program_start = clock();

    srand(time(0));
    canary_setup();

    uint8_t canary;

    printf("Enter '1' to perform a prime cruncher test.\nEnter '2' to perform a canary test\nInput: ");
    scanf("%d", &selection);

    recursive_function_to_have_many_locals_on_the_stack(RECURSION_MAX);

    memcpy(&canary, &canary_value, sizeof(canary_value));
    

    //gets(buf);

    if (canary_value == 0) {
        bypassed();
    }

    if (memcmp(&canary,&canary_value,sizeof(canary_value))) {
        canary_tripped();
    }
    clock_t program_end = clock();
    printf("\nFinal Time: %f", (double)(program_end-program_start)/CLOCKS_PER_SEC);
}