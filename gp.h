#include <stdbool.h>

typedef uint8_t instruction_t;

typedef struct {
    size_t length;
    instruction_t *code;
} Program;

enum instruction_enum {
    ADD, SUB, MUL, DIV, SQRT, EXP, COS, X, C1, C2, C3, C4, C5, C6, C7, C8
};

extern float C1_VAL, C2_VAL, C3_VAL, C4_VAL, C5_VAL, C6_VAL, C7_VAL, C8_VAL, CROSSOVER_PROB;

#define STACK_SIZE 1024

Program *optimize_program(Program *program);
float eval_optimized(Program *program, float x);
float eval(Program *program, float x);

typedef struct {
    unsigned seed;
    size_t population_size;
    size_t n_generations;
    bool optimize;
    float *x_samples;
    float *y_samples;
    size_t n_samples;
} EvolveConfig;

float evolve(EvolveConfig config, Program *evolved);

float calc_error(EvolveConfig config, Program *program);
Program *random_program();
