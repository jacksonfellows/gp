typedef uint8_t instruction_t;

typedef struct {
    size_t length;
    instruction_t *code;
} Program;

enum instruction_enum {
    ADD, SUB, MUL, DIV, SQRT, EXP, COS, X, C1, C2, C3, C4, C5, C6, C7, C8
};

double C1_VAL = +1.0;
double C2_VAL = -1.0;
double C3_VAL = +0.5;
double C4_VAL = -0.5;
double C5_VAL = +0.25;
double C6_VAL = -0.25;
double C7_VAL = +0.125;
double C8_VAL = -0.125;

#define STACK_SIZE 1024

double CROSSOVER_PROB = 0.80;

double eval(Program *program, double x);
double evolve(size_t population_size, size_t n_generations, Program *evolved);
double calc_error(Program *program);
