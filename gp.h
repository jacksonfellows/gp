typedef uint8_t instruction_t;

typedef struct {
    size_t length;
    instruction_t *code;
} Program;

enum instruction_enum {
    ADD, SUB, MUL, DIV, SQRT, EXP, COS, X, C1, C2, C3, C4, C5, C6, C7, C8
};

float C1_VAL = +1.0f;
float C2_VAL = -1.0f;
float C3_VAL = +0.5f;
float C4_VAL = -0.5f;
float C5_VAL = +0.25f;
float C6_VAL = -0.25f;
float C7_VAL = +0.125f;
float C8_VAL = -0.125f;

#define STACK_SIZE 1024

float CROSSOVER_PROB = 0.80f;

float eval(Program *program, float x);
float evolve(unsigned seed, size_t population_size, size_t n_generations, Program *evolved);
float calc_error(Program *program);
