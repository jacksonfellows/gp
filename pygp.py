from math import *

import _gp

def evolve_program(**kwargs):
    res = _gp.ffi.new('Program *')
    assert(set(kwargs.keys()) == {'seed', 'population_size', 'n_generations', 'optimize', 'x_samples', 'y_samples', 'n_samples'})
    fitness = _gp.lib.evolve(kwargs, res)
    return res

def code_from_program(program):
    return [_gp.ffi.string(_gp.ffi.cast('enum instruction_enum', i)) for i in _gp.ffi.unpack(program.code, program.length)]

def program_from_code(code):
    program = _gp.ffi.new('Program *')
    program.length = len(code)
    program.code = _gp.ffi.new(f'instruction_t[{len(code)}]', bytes([_gp.lib.__dict__[i] for i in code]))
    return program

binops = ['ADD', 'SUB', 'MUL', 'DIV']
unops = ['SQRT', 'EXP', 'COS']

def tree_from_code(code):
    stack = []
    for insn in code:
        if insn in binops:
            if len(stack) >= 2:
                y = stack.pop()
                x = stack.pop()
                stack.append((insn, x, y))
        elif insn in unops:
            if len(stack) >= 1:
                stack.append((insn, stack.pop()))
        else:
            stack.append(insn)
    return stack.pop()

constants = ['C1', 'C2', 'C3', 'C4', 'C5', 'C6', 'C7', 'C8']

def const_val(constant):
    return getattr(_gp.lib, constant + '_VAL')

def eval_binop(op, x, y):
    return {
        'ADD': lambda x,y: x + y,
        'SUB': lambda x,y: x - y,
        'MUL': lambda x,y: x * y,
        'DIV': lambda x,y: 1 if y == 0 else x / y
    }[op](x,y)

def eval_unop(op, x):
    return {
        'SQRT': lambda x: 0 if x < 0 else sqrt(x),
        'EXP': lambda x: exp(x),
        'COS': lambda x: cos(x)
    }[op](x)

def simplify_tree(tree):
    if type(tree) != tuple:
        if tree in constants:
            return const_val(tree)
        else:
            return tree
    else:
        simplified = tuple([tree[0], *[simplify_tree(x) for x in tree[1:]]])
        if all(type(x) == float for x in simplified[1:]):
            if simplified[0] in binops:
                return eval_binop(simplified[0], simplified[1], simplified[2])
            elif simplified[0] in unops:
                return eval_unop(simplified[0], simplified[1])
        return simplified

import timeit

def benchmark1():
    prog = program_from_code(['C4', 'MUL', 'C8', 'EXP', 'C4', 'X', 'C3', 'EXP', 'X', 'C3', 'EXP', 'C5', 'COS', 'SUB', 'X', 'MUL', 'MUL', 'MUL', 'X', 'MUL', 'C4', 'MUL', 'MUL', 'MUL', 'C5', 'DIV', 'MUL', 'SQRT'])
    return timeit.timeit(lambda: _gp.lib.calc_error(prog, False), number=5000000), timeit.timeit(lambda: _gp.lib.calc_error(prog, True), number=5000000)

import numpy as np
import matplotlib.pyplot as plt

def benchmark2():
    def go():
        xs = np.linspace(0,10,100,dtype='single')
        ys = xs*xs
        p = evolve_program(population_size=1000, n_generations=1000, seed=123, optimize=True, x_samples=_gp.ffi.from_buffer(xs), y_samples=_gp.ffi.from_buffer(ys), n_samples=len(xs))
        print(code_from_program(p))
    return timeit.timeit(go, number=1)
