//#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

//to compile: gcc -o 1bytecanary main.c -fno-stack-protector -no-pie
//a
#define BUFFER_SIZE 8
#define RECURSION_MAX 98
#define PRIMES 1000
#define CANARY_MAX 0xFFFFFFFFFFFFFFFF
#define CANARY_MIN 0
#define DATA_FILE "1ByteCanaryData.txt"
#define CANARY_BYTES 1

int selection;
uint8_t canary_value;
clock_t test_start;
clock_t program_start;

//Canary Functions
void canary_test();
void recursive_function_to_have_many_locals_on_the_stack(int recursive_num);
void canary_tripped(void) {
    //fflush(stdin);
    printf("\nCanary Has Been Detected To Be Changed. Restarting Function\n");
    exit(0);
}

void canary_setup(void) {
    FILE *file_ptr;
    file_ptr = fopen("Canary.txt","r");
    int temp_canary;
    fscanf(file_ptr,"%d",&canary_value);
    printf("\nC: %d\n",canary_value);

    //canary_value = rand() % (CANARY_MAX - CANARY_MIN + 1) + CANARY_MIN;
    printf("%x\n", canary_value);
}

void print_memory_status(uint8_t local_canary, char* buffer) {
    printf("canary value: %p\tglobal canary value: %p\tbuffer memory: %p\n",local_canary,canary_value,buffer);
    printf("canary address: %p\tglobal canary address %p\tbuffer address: %p\nbuffer value: ", (void*)&local_canary,(void*)&canary_value,(void*)&buffer);
}

//function to know that canary has been bypassed fully
void bypassed() {
    printf("Canary Bypassed");

    clock_t program_end = clock();
    printf("\nTest Time: %f\nFinal Time: %f", (double)(program_end-test_start)/CLOCKS_PER_SEC,(double)(program_end-program_start)/CLOCKS_PER_SEC);
    FILE *file_pointer;

    file_pointer = fopen(DATA_FILE,"a");
    fprintf(file_pointer, "%d\t%d\t%x\t%f\t%fs",selection,CANARY_BYTES,canary_value,(double)(program_end-test_start)/CLOCKS_PER_SEC,(double)(program_end-program_start)/CLOCKS_PER_SEC);
    fclose(file_pointer);
    exit(0);
}

//Functions for the actual tests

void prime_test() {
    test_start = clock();
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
    test_start = clock();
    //This is roppable
    uint8_t canary;
    memcpy(&canary, &canary_value, sizeof(canary_value));

    char buf[BUFFER_SIZE];
    unsigned int read_bytes;

    print_memory_status(canary, buf);

    printf("Enter the number of maximum bytes to be read:\n");
    scanf("%d", &read_bytes);
    printf("Enter what to write to the buffer:\n");
    read(0,buf,read_bytes);

    //print_memory_status(canary, buf);
    //printf("%s",buf);

    if (memcmp(&canary,&canary_value,sizeof(canary_value))) {
        canary_tripped();
    } else {
        printf("\nCanary did not trip.\n");
    }
}

void canary_reset() {
    canary_value = 0;
    for (int i = 0; i < CANARY_BYTES; i++) {
        int random = (rand() % (0xFF - 0x00 + 1) + 0x00);
        
        canary_value = (canary_value << 8) | random;
        //printf("%x\t%x\n",random,canary_value);
    }
    //canary_value = rand() % (CANARY_MAX - CANARY_MIN + 1) + CANARY_MIN;
    printf("%x\n", canary_value);

    FILE *file_pointer;
    file_pointer = fopen("Canary.txt","w");

    fprintf(file_pointer, "%d", canary_value);
    fclose(file_pointer);
    exit(0);
}

void recursive_function_to_have_many_locals_on_the_stack(int recursion_num) {
    uint8_t canary;
    memcpy(&canary, &canary_value, sizeof(canary_value));
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

    if (memcmp(&canary,&canary_value,sizeof(canary_value))) {
        canary_tripped();
    }
}

int main() {
    program_start = clock();

    srand(time(0));
    canary_setup();

    uint8_t canary;

    printf("Enter '1' to perform a prime cruncher test.\nEnter '2' to perform a canary test\nEnter '3' to change the canary\nInput: ");
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