/*
    Joshua Wood
    12/3/2021
    Assignment 3
*/

// Preprocessor directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// This structure creates the instructions base structure
typedef struct instruction {
    int opCode, deviceOrAddress; // Stores the opCode and the device/address of the operation
} instruction;

// Globals
#define INITIAL 0
#define START 10
#define SIZE 9
#define COMPLETE 1
#define DEFAULT 20
#define BUFFSIZE 100

// opCodes
#define LOAD 1
#define ADD 2
#define STORE 3
#define SUB 4
#define IN 5
#define OUT 6
#define END 7
#define JMP 8
#define SKIPZ 9

// Global Variables
int INPUTSIZE = DEFAULT; // Default input size value
int ir = INITIAL; // Initialize the instruction register
int pc = START; // Initialize the program counter
int mar = INITIAL; // Initialize the memory address register
int dm[SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // Initialize the data memory
int mdr = INITIAL; // Initialize the memory data register
int ac = INITIAL; // Initialize the accumulator

// Function prototypes
void printData();
int getLinesOfCode(FILE *);
int increasePC(int);
void programStart();
void programEnd();
void simulator(int, int);


// Function prints the values to the screen
void printData()
{
    printf("\nPC = %d  |  A = %d  |  DM = [", pc, ac); // Prints the Program counter, accumulator value, and the data memory to the screen

    for (int i = 0; i < SIZE; i++) { // For loop goes through the data memory array
        if ((i+1) == SIZE) // Checks for last digit for formatting purposes
            printf("%d]\n", dm[i]);
        else
            printf("%d,", dm[i]);
    }
}

// Function determines how many lines of code are in the input file
int getLinesOfCode(FILE * inputFile)
{
    char buffer[BUFFSIZE];
    int counter = 0;

    if (inputFile == NULL) { // Checks then exists if the inputFile is NULL
        printf("There was an issue opening up your file ...\n");
        exit(0);
    }

    while (fgets(buffer, BUFFSIZE, inputFile) != NULL) // Calls the fgets function to go through the text file until it reaches NULL
        counter++; // Increase count per line

    fclose(inputFile);

    return counter; // Returns the counter variable
}

// Function increases the program counter by 1
int increasePC(int pc) {
    return pc+=1;
}

// Outputs beginning printf statements when the program starts
void programStart()
{
    printf("\n=====================================\n");
    printf("Welcome to the Tiny Machine Simulator");
    printf("\n=====================================\n");
    printf("\n**Assembling Program**\n\n");
    printf("\n**Program Assembled Correctly**\n\n");
    printf("\n... RUNNING ...\n\n");
    printf("\nSTART\n");

    printData();
}

// Outputs ending printf statements when the program ends
void programEnd()
{
    printf("\n\nProgram has now concluded. Goodbye!\n\n");

    printf("========================\n");
    printf("\n\n[DEVELOPER]: Joshua Wood\n");
    printf("[DATE]: December 3, 2021\n");
    printf("[COURSE]: CGS 3269\n");
    printf("========================\n");
}

// This function takes the opCode and address and decides which task to perform based on the opCode. This is the bulk of the program and simulates the tiny machine operations.
void simulator(int opCode, int address)
{
    // Switch statements performs different tasks depending on the opCode
    switch(opCode) {
        // This represents the LOAD instruction
        case LOAD:
            printf("\n==LOADING FROM ADDRESS [%d]==\n", address);

            ir = address; // Instruction register is equal to the address
            mar = ir; // Memory access register is set to the instruction register
            mdr = dm[mar]; // Memory data register is set to the value within the data memory array at location specified in the memory access register
            ac = mdr; // AC is now set to the memory data register value gotten from the data memory

            printData(); // Prints data to the screen

            pc = increasePC(pc); // Increase program counter

            break;
        // This represents the ADD Instruction
        case ADD:
            printf("\n==ADDING AC AND VALUE FROM ADDRESS [%d]==\n", address);

            ir = address; // Instruction register is equal to the address
            mar = ir; // Memory access register is set to the instruction register
            mdr = dm[mar]; // Memory data register is set to the value within the data memory array at location specified in the memory access register
            ac += mdr; // AC now adds the value from memory data register

            printData(); // Prints data to the screen

            pc = increasePC(pc); // Increase program counter

            break;
        // This represents the STORE Instruction
        case STORE:
            printf("\n==STORING AC TO MEMORY LOCATION 0==\n");

            ir = address; // Instruction register is equal to the address
            mdr = ac; // Memory data register is set to the ac value
            mar = ir; // Memory access register is set to the instruction register
            dm[mar] = mdr; // Value at data memory arrays at location from memory access is set to the memory data register value

            printData(); // Prints data to the screen

            pc = increasePC(pc); // Increases program counter

            break;
        // This represents the SUB Instruction
        case SUB:
            printf("\n==SUBTRACTING AC AND VALUE FROM ADDRESS [%d]==\n", address);

            ir = address; // Instruction register is equal to the address
            mar = ir; // Memory access register is set to the instruction register
            mdr = dm[mar]; // Memory data register is set to the value within the data memory array at location specified in the memory access register
            ac -= mdr; // AC now subtracts the value from memory data register

            printData(); // Prints data to the screen

            pc = increasePC(pc); // Increases program counter

            break;
        // This represents the IN Instruction
        case IN:
            printf("\n==WAITING FOR USER INPUT==\n");
            printf("-> ");
            scanf("%d", &ac); // Gets the value from the users input to set to the ac

            printData(); // Prints data to the screen

            pc = increasePC(pc); // Increases program counter

            break;
        // This represents the OUT Instruction
        case OUT:
            printf("\n==AC CURRENT VALUE==\n");
            printf("\nAC: %d\n", ac); // Prints out the current value at AC

            printData(); // Prints data to the screen

            pc = increasePC(pc); // Increases program counter

            break;
        // This represents the END Instruction
        case END:
            printf("\n==PROGRAM COMPLETE==\n");
            printf("\n========================\n");
            printf("[DEVELOPER]: Joshua Wood\n");
            printf("[DATE]: December 3, 2021\n");
            printf("[COURSE]: CGS 3269\n");
            printf("========================\n");
            exit(COMPLETE); // Exit program
        // This represents the JMP Instruction
        case JMP:
            printf("\n==JUMPING TO PC %d==\n", address);

            pc = address; // PC is set to the new address

            printData(); // Prints data to the screen. PC does not increase

            break;
        // This represents the SKIPZ Instruction
        case SKIPZ:
            printf("\n==SKIPPING NEXT INSTRUCTION==\n");

            if (ac == 0) // If the ac is at 0, increment by 2 to represent the fetch pc counter increase
                pc += 2;
            else
                pc += 1; // Else only skip one instruction

            printData(); // Prints data to the screen

            break;
        default: // If no opCode generate a response, there was an error. Exit program.
            printf("\nThere was an error that caused the program to crash. Exiting program ...\n");
            exit(0);
    }
}

// Main function that takes in one argument (the text file)
int main(int argc, char * argv[])
{
    INPUTSIZE = getLinesOfCode(fopen(argv[1], "r")); // Reads file and gets a new inputsize
    instruction programMemory[INPUTSIZE]; // Set struct format to variable array programMemory with size INPUTSIZE

    int i = 0;
    char buffer[100];

    FILE * inputFile = fopen(argv[1], "r"); // Open input file

    if (inputFile == NULL) { // Check for error. If there is no file, output error message
        printf("Error with opening your input file.");
        exit(0);
    }

    while(fgets(buffer, sizeof(INPUTSIZE), inputFile) != NULL) { // Call fgets function using buffer, input size, and file name. If not null, perform tasks
        if ((buffer == "\n" || buffer != " ")) { // Checks for spaces or new line characters
            if (isdigit(buffer[0]) && (int)buffer[2] != 0) { // Checks to see if the values at buffer[0] and [2] are a digit and do not equal 0
                programMemory[i].opCode = atoi(&buffer[0]); // Sets value of position i into array programMemory opCode to the int version of the value at buffer[0]
                programMemory[i].deviceOrAddress = atoi(&buffer[2]); // Sets value of position i into array programMemory deviceOrAddress to the int version of the value at buffer[2]
                i++; // Increase position
            }
        }
    }

    fclose(inputFile); // Close the file

    programStart(); // Start the program text

    for (i = (pc / START) - 1; i < sizeof(programMemory); i++) { // Runs through the input file and calls the simulator function for each value of i within program memory
        simulator(programMemory[i].opCode, programMemory[i].deviceOrAddress); // Calls simulator with opCode and deviceOrAddress values at position i in programMemory
    }

    programEnd(); // End program

    return 0;
}
