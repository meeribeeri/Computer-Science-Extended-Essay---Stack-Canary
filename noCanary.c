//#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

//to compile: gcc -o 0bytecanary main.c -fno-stack-protector -no-pie

#define BUFFER_SIZE 8
#define RECURSION_MAX 98
#define PRIMES 1000
#define DATA_FILE "0ByteCanaryData.txt"
#define CANARY_BYTES 0

int selection;
clock_t test_start;
clock_t program_start;

//Canary Functions
void canary_test();
void recursive_function_to_have_many_locals_on_the_stack(int recursive_num);

void canary_setup(void) {

    printf("%s\n", "There is no canary");
}

void print_memory_status(uint16_t local_canary, char* buffer) {
    printf("%s\n", "There is no canary");
}

//function to know that canary has been bypassed fully
void bypassed() {
    printf("Canary Bypassed");

    clock_t program_end = clock();
    printf("\nTest Time: %f\nFinal Time: %f", (double)(program_end-test_start)/CLOCKS_PER_SEC,(double)(program_end-program_start)/CLOCKS_PER_SEC);
    FILE *file_pointer;

    file_pointer = fopen(DATA_FILE,"a");
    fprintf(file_pointer, "%d\t%d\t%f\t%fs",selection,CANARY_BYTES,(double)(program_end-test_start)/CLOCKS_PER_SEC,(double)(program_end-program_start)/CLOCKS_PER_SEC);
    fclose(file_pointer);
    exit(0);
}

//Functions for the actual tests

void prime_test() {
    test_start = clock();
    //This is a prime cruncher to test cpu

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

}

void canary_test() {
    test_start = clock();
    //This is roppable

    char buf[BUFFER_SIZE];
    unsigned int read_bytes;

    print_memory_status(0, buf);

    printf("Enter the number of maximum bytes to be read:\n");
    scanf("%d", &read_bytes);
    printf("Enter what to write to the buffer:\n");
    read(0,buf,read_bytes);

    //print_memory_status(canary, buf);
    //printf("%s",buf);

    printf("\nCanary did not trip.\n");
}

void canary_reset() {
    exit(0);
}

void recursive_function_to_have_many_locals_on_the_stack(int recursion_num) {

    if (recursion_num > 0) {
        recursive_function_to_have_many_locals_on_the_stack(recursion_num-1);
    } else if (selection == 1) {
        prime_test();
    } else if (selection == 2) {
        while (true) {
            canary_test();
        }
    } else if (selection == 3) {
        canary_reset();
    }

}

int main() {
    program_start = clock();

    srand(time(0));
    canary_setup();


    printf("Enter '1' to perform a prime cruncher test.\nEnter '2' to perform a canary test\nEnter '3' to change the canary\nInput: ");
    scanf("%d", &selection);

    recursive_function_to_have_many_locals_on_the_stack(RECURSION_MAX);

    
    
    //gets(buf);

    if (1 == 0) {
        bypassed();
    }

    clock_t program_end = clock();
    printf("\nTest Time: %f\nFinal Time: %f", (double)(program_end-test_start)/CLOCKS_PER_SEC,(double)(program_end-program_start)/CLOCKS_PER_SEC);

    //write data to file
    FILE *file_pointer;

    if (fopen(DATA_FILE,"a") == 0) {
        printf("\nWriting\n");
        file_pointer = fopen(DATA_FILE,"w");
        fprintf(file_pointer, "Test Type    Canary Size   Canary Value   Test Time    Program Time\n%d\t%d\t%x\t%f\t%fs",selection,CANARY_BYTES,canary_value,(double)(program_end-test_start)/CLOCKS_PER_SEC,(double)(program_end-program_start)/CLOCKS_PER_SEC);
    } else {
        file_pointer = fopen(DATA_FILE,"a");
        fprintf(file_pointer, "\n%d\t%d\t%x\t%f\t%fs",selection,CANARY_BYTES,canary_value,(double)(program_end-test_start)/CLOCKS_PER_SEC,(double)(program_end-program_start)/CLOCKS_PER_SEC);
        printf("\nAppending\n");
    }
    
    fclose(file_pointer);
}