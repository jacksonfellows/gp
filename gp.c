#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "gp.h"

float C1_VAL = +1.0f;
float C2_VAL = -1.0f;
float C3_VAL = +0.5f;
float C4_VAL = -0.5f;
float C5_VAL = +0.25f;
float C6_VAL = -0.25f;
float C7_VAL = +0.125f;
float C8_VAL = -0.125f;

float CROSSOVER_PROB = 0.80f;

float safesqrt(float x) {
    return x >= 0.0f ? sqrtf(x) : 0.0f;
}

float safediv(float x, float y) {
  return y == 0.0f ? 1.0f : x / y;
}

Program *optimize_program(Program *program) {
    size_t length = 0;
    instruction_t *code = malloc(sizeof(instruction_t) * program->length);
    size_t tos = 0;
    size_t min_stack[STACK_SIZE];
    for (size_t pc = 0; pc < program->length; pc++) {
	if (tos >= STACK_SIZE) {
	    Program *new = malloc(sizeof(Program));
	    new->length = 0;
	    new->code = NULL;
	    return new;
	}
	switch (program->code[pc]) {
	case ADD:
	case SUB:
	case MUL:
	case DIV:
	    if (tos >= 2) {
		tos--;
		code[length++] = program->code[pc];
	    }
	    break;
	case SQRT:
	case EXP:
	case COS:
	    if (tos >= 1) {
		code[length++] = program->code[pc];
	    }
	    break;
	case X:
	case C1:
	case C2:
	case C3:
	case C4:
	case C5:
	case C6:
	case C7:
	case C8:
	    min_stack[tos++] = length;
	    code[length++] = program->code[pc];
	    break;
	}
    }
    if (tos >= 1) {
	size_t min = min_stack[tos-1];
	Program *new = malloc(sizeof(Program));
	new->length = length - min;
	new->code = malloc(sizeof(instruction_t) * new->length);
	memcpy(new->code, code + min, new->length);
	free(code);
	return new;
    } else {
	Program *new = malloc(sizeof(Program));
	new->length = 0;
	new->code = NULL;
	return new;
    }
}

float eval_optimized(Program *program, float x) {
    float stack[STACK_SIZE];
    size_t tos = 0;
    for (size_t pc = 0; pc < program->length; pc++) {
        switch (program->code[pc]) {
        case ADD:
	    stack[tos - 2] = stack[tos - 2] + stack[tos - 1];
	    tos--;
            break;
        case SUB:
	    stack[tos - 2] = stack[tos - 2] - stack[tos - 1];
	    tos--;
            break;
        case MUL:
	    stack[tos - 2] = stack[tos - 2] * stack[tos - 1];
	    tos--;
            break;
        case DIV:
	    stack[tos - 2] = safediv(stack[tos - 2], stack[tos - 1]);
	    tos--;
            break;
        case SQRT:
	    stack[tos - 1] = safesqrt(stack[tos - 1]);
            break;
        case EXP:
	    stack[tos - 1] = expf(stack[tos - 1]);
            break;
        case COS:
	    stack[tos - 1] = cosf(stack[tos - 1]);
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

float eval(Program *program, float x) {
    float stack[STACK_SIZE];
    size_t tos = 0;
    for (size_t pc = 0; pc < program->length; pc++) {
        if (tos >= STACK_SIZE) {
            return 0.0f;
        }
        switch (program->code[pc]) {
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

void free_program(Program *program) {
    free(program->code);
    free(program);
}

#define SIZE_PENALTY 0.005

float calc_error(EvolveConfig config, Program *program) {
    float error = 0.0f;
    if (config.optimize) {
	Program *optimized = optimize_program(program);
	for (size_t i = 0; i < config.n_samples; i++) {
	    error += fabs(config.y_samples[i] - eval_optimized(optimized, config.x_samples[i]));
	}
	free_program(optimized);
    } else {
	for (size_t i = 0; i < config.n_samples; i++) {
	    error += fabs(config.y_samples[i] - eval(program, config.x_samples[i]));
	}
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

void free_programs(Program **population, size_t population_size) {
    for (int i = 0; i < population_size; i++) {
        free_program(population[i]);
    }
}

float evolve(EvolveConfig config, Program *evolved) {
    srand(config.seed);

    Program *population[config.population_size];
    Program *new_population[config.population_size];

    for (int i = 0; i < config.population_size; i++) {
	population[i] = random_program();
    }

    float fitnesses[config.population_size];
    float evolved_fitness = -1;

    for (int g = 1; g <= config.n_generations; g++) {
        float max_fitness = 0.0f;
        float total_fitness = 0.0f;
        int max_i = -1;
        #pragma omp parallel for
        for (int i = 0; i < config.population_size; i++) {
            float fitness = 1.0f / (1.0f + calc_error(config, population[i]));
            total_fitness += fitness;
            fitnesses[i] = fitness;
            if (fitness > max_fitness) {
                max_fitness = fitness;
                max_i = i;
            }
        }

        if (g == 1 || g % 100 == 0) {
            fprintf(stderr, "gen %d, max fitness %f, avg. fitness %f\n", g, max_fitness, total_fitness / (float)config.population_size);
        }

        if (g < config.n_generations) {
            new_population[0] = copy_program(population[max_i]);
            int j = 1;
            while (j < config.population_size) {
                if (randprob() < CROSSOVER_PROB && j + 1 < config.population_size) {
                    crossover(select_prop(population, config.population_size, fitnesses, max_fitness), select_prop(population, config.population_size, fitnesses, max_fitness), &new_population[j], &new_population[j+1]);
                    j += 2;
                } else {
                    new_population[j++] = copy_program(select_prop(population, config.population_size, fitnesses, max_fitness));
                }
            }
            free_programs(population, config.population_size);
            memcpy(population, new_population, sizeof(Program *) * config.population_size);
        } else {
	    copy_program_to(evolved, population[max_i]);
	    evolved_fitness = fitnesses[max_i];
	}
    }
    free_programs(population, config.population_size);
    return evolved_fitness;
}
