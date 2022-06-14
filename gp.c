#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <time.h>

typedef uint8_t instruction_t;

typedef struct {
    size_t length;
    instruction_t *code;
} Program;

enum instruction_enum {
    NOP, ADD, SUB, MUL, DIV, POW, SIN, COS,
    X, C1, C2, C3, C4, C5, C6, C7
};

#define C1_VAL +1.0
#define C2_VAL -1.0
#define C3_VAL +0.5
#define C4_VAL -0.5
#define C5_VAL +0.25
#define C6_VAL -0.25
#define C7_VAL 0.0

#define STACK_SIZE 1024

float eval(Program *program, float x) {
    float stack[STACK_SIZE];
    size_t tos = 0;
    size_t pc = 0;
    while (pc < program->length) {
        assert(tos < STACK_SIZE);
        switch (program->code[pc++]) {
        case NOP:
            break;
        case ADD:
            if (tos >= 2) {
                stack[tos - 2] = stack[tos - 2] + stack[tos - 1];
                tos--;
            }
            break;
        case SUB:
            if (tos >= 2) {
                stack[tos - 2] = stack[tos - 2] - stack[tos - 1];
                tos--;
            }
            break;
        case MUL:
            if (tos >= 2) {
                stack[tos - 2] = stack[tos - 2] * stack[tos - 1];
                tos--;
            }
            break;
        case DIV:
            if (tos >= 2) {
                if (stack[tos - 1] == 0) {
                    stack[tos - 2] = 1;
                } else {
                    stack[tos - 2] = stack[tos - 2] / stack[tos - 1];
                }
                tos--;
            }
            break;
        case POW:
            if (tos >= 2) {
                stack[tos - 2] = pow(stack[tos - 2], stack[tos - 1]);
                tos--;
            }
            break;
        case SIN:
            if (tos >= 1) {
                stack[tos] = sin(stack[tos]);
            }
            break;
        case COS:
            if (tos >= 1) {
                stack[tos] = cos(stack[tos]);
            }
            break;
        case X:
            stack[tos++] = x;
            break;
        case C1:
            stack[tos++] = C1_VAL;
            break;
        case C2:
            stack[tos++] = C2_VAL;
            break;
        case C3:
            stack[tos++] = C3_VAL;
            break;
        case C4:
            stack[tos++] = C4_VAL;
            break;
        case C5:
            stack[tos++] = C5_VAL;
            break;
        case C6:
            stack[tos++] = C6_VAL;
            break;
        case C7:
            stack[tos++] = C7_VAL;
            break;
        }        
    }
    return tos > 0 ? stack[tos - 1] : 0;
}

void print_program(Program *program) {
    for (ssize_t i = 0; i < program->length; i++) {
        switch (program->code[i]) {
        case NOP:
            printf("NOP\n");
            break;
        case ADD:
            printf("ADD\n");
            break;
        case SUB:
            printf("SUB\n");
            break;
        case MUL:
            printf("MUL\n");
            break;
        case DIV:
            printf("DIV\n");
            break;
        case POW:
            printf("POW\n");
            break;
        case SIN:
            printf("SIN\n");
            break;
        case COS:
            printf("COS\n");
            break;
        case X:
            printf("PUSH X\n");
            break;
        case C1:
            printf("PUSH %f\n", C1_VAL);
            break;
        case C2:
            printf("PUSH %f\n", C2_VAL);
            break;
        case C3:
            printf("PUSH %f\n", C3_VAL);
            break;
        case C4:
            printf("PUSH %f\n", C4_VAL);
            break;
        case C5:
            printf("PUSH %f\n", C5_VAL);
            break;
        case C6:
            printf("PUSH %f\n", C6_VAL);
            break;
        case C7:
            printf("PUSH %f\n", C7_VAL);
            break;
        }        
    }
}

size_t randsize(size_t min_size, size_t max_size) {
    return min_size + (max_size - min_size) * ((double)rand() / (double)RAND_MAX);
}

instruction_t randinst() {
    return 0xF & rand();
}

#define MIN_PROGRAM_SIZE 5
#define MAX_PROGRAM_SIZE 100

Program *random_program() {
    Program *program = malloc(sizeof(Program));
    size_t len = randsize(MIN_PROGRAM_SIZE, MAX_PROGRAM_SIZE);
    program->length = len;
    program->code = malloc(sizeof(instruction_t) * len);
    for (size_t i = 0; i < len; i++) {
        program->code[i] = randinst();
    }
    return program;
}

float calc_fitness(Program *program) {
    float fitness = 0;
    for (int i = 0; i < 20; i++) {
        float x = -1 + ((float)i/10.0);
        float y = x*x;
        fitness += fabs(y - eval(program, x));
    }
    return fitness;
}

#define POPULATION_SIZE 1024

int main(int argc, char **argv) {
    srand(time(0));
    
    Program *population[POPULATION_SIZE];
    for (int i = 0; i < POPULATION_SIZE; i++) {
        population[i] = random_program();
    }

    float best_fitness = MAXFLOAT;
    int best_i = -1;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        float fitness = calc_fitness(population[i]);
        if (fitness < best_fitness) {
            best_fitness = fitness;
            best_i = 0;
        }
    }

    printf("got a best fitness of %f\n", best_fitness);
    printf("best program:\n");
    print_program(population[best_i]);
    
    return 0;
}
