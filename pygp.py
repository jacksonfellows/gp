from math import *

import _gp

def evolve_program(population_size, n_generations):
    res = _gp.ffi.new('Program *')
    fitness = _gp.lib.evolve(population_size, n_generations, res)
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

def code_to_tree(code):
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
    
