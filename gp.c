#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "gp.h"

float safesqrt(float x) {
    return x >= 0.0f ? sqrtf(x) : 0.0f;
}

float safediv(float x, float y) {
  return y == 0.0f ? 1.0f : x / y;
}

float eval(Program *program, float x) {
    float stack[STACK_SIZE];
    size_t tos = 0;
    size_t pc = 0;
    while (pc < program->length) {
        if (tos >= STACK_SIZE) {
            return 0.0f;
        }
        switch (program->code[pc++]) {
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
		stack[tos - 2] = safediv(stack[tos - 2], stack[tos - 1]);
                tos--;
            }
            break;
        case SQRT:
            if (tos >= 1) {
                stack[tos - 1] = safesqrt(stack[tos - 1]);                
            }
            break;
        case EXP:
            if (tos >= 1) {
                stack[tos - 1] = expf(stack[tos - 1]);
            }
            break;
        case COS:
            if (tos >= 1) {
                stack[tos - 1] = cosf(stack[tos - 1]);
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
        case C8:
            stack[tos++] = C8_VAL;
            break;
        }        
    }
    return tos > 0 ? stack[tos - 1] : 0.0f;
}

size_t randsize(size_t min_size, size_t max_size) {
    /* [min_size,max_size) */
    return min_size + (max_size - min_size - 1) * ((float)rand() / (float)RAND_MAX);
}

void crossover(Program *p1, Program *p2, Program **c1, Program **c2) {
    size_t i = randsize(0, p1->length);
    size_t j = randsize(0, p2->length);
    *c1 = malloc(sizeof(Program));
    *c2 = malloc(sizeof(Program));
    (*c1)->length = i + p2->length - j;
    (*c2)->length = j + p1->length - i;
    (*c1)->code = malloc(sizeof(instruction_t) * (i + p2->length - j));
    (*c2)->code = malloc(sizeof(instruction_t) * (j + p1->length - i));
    memcpy((*c1)->code, p1->code, sizeof(instruction_t) * i);
    memcpy((*c2)->code, p2->code, sizeof(instruction_t) * j);
    memcpy((*c1)->code + i, p2->code + j, sizeof(instruction_t) * (p2->length - j));
    memcpy((*c2)->code + j, p1->code + i, sizeof(instruction_t) * (p1->length - i));        
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

#define SIZE_PENALTY 0.005

float calc_error(Program *program) {
    float error = 0.0f;
    for (int i = 0; i < 20; i++) {
        float x = -1.0f + ((float)i/10.0f);
        float y = x*x;
        error += fabs(y - eval(program, x));
    }
    return error + SIZE_PENALTY * (float)(program->length);
}

float randprob() {
    return (float)(rand())/((float)(RAND_MAX));
}

Program *select_prop(Program **population, size_t population_size, float *fitnesses, float max_fitness) {
    size_t randi = randsize(0, population_size);
    if (randprob() < fitnesses[randi]/max_fitness) {
        return population[randi];
    }
    return select_prop(population, population_size, fitnesses, max_fitness);
}

void copy_program_to(Program *dst, Program *src) {
    dst->length = src->length;
    dst->code = malloc(sizeof(instruction_t) * src->length);
    memcpy(dst->code, src->code, sizeof(instruction_t) * src->length);
}

Program *copy_program(Program *program) {
    Program *new_program = malloc(sizeof(Program));
    copy_program_to(new_program, program);
    return new_program;
}

void free_program(Program *program) {
    free(program->code);
    free(program);
}

void free_programs(Program **population, size_t population_size) {
    for (int i = 0; i < population_size; i++) {
        free_program(population[i]);
    }
}

float evolve(unsigned seed, size_t population_size, size_t n_generations, Program *evolved) {
    srand(seed);

    Program *population[population_size];
    Program *new_population[population_size];

    for (int i = 0; i < population_size; i++) {
	population[i] = random_program();
    }

    float fitnesses[population_size];
    float evolved_fitness = -1;

    for (int g = 1; g <= n_generations; g++) {
        float max_fitness = 0.0f;
        float total_fitness = 0.0f;
        int max_i = -1;
        for (int i = 0; i < population_size; i++) {
            float fitness = 1.0f / (1.0f + calc_error(population[i]));
            total_fitness += fitness;
            fitnesses[i] = fitness;
            if (fitness > max_fitness) {
                max_fitness = fitness;
                max_i = i;
            }
        }

        if (g == 1 || g % 100 == 0) {
            fprintf(stderr, "gen %d, max fitness %f, avg. fitness %f\n", g, max_fitness, total_fitness / (float)population_size);
        }

        if (g < n_generations) {
            new_population[0] = copy_program(population[max_i]);
            int j = 1;
            while (j < population_size) {
                if (randprob() < CROSSOVER_PROB && j + 1 < population_size) {
                    crossover(select_prop(population, population_size, fitnesses, max_fitness), select_prop(population, population_size, fitnesses, max_fitness), &new_population[j], &new_population[j+1]);
                    j += 2;
                } else {
                    new_population[j++] = copy_program(select_prop(population, population_size, fitnesses, max_fitness));
                }
            }
            free_programs(population, population_size);
            memcpy(population, new_population, sizeof(Program *) * population_size);
        } else {
	    copy_program_to(evolved, population[max_i]);
	    evolved_fitness = fitnesses[max_i];
	}
    }
    free_programs(population, population_size);
    return evolved_fitness;
}

int main(int argc, char **argv) {
    // pass
}
