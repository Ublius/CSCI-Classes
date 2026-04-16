import sys
import re

comp_table = {
    "0": "0101010",
    "1": "0111111",
    "-1": "0111010",
    "D": "0001100",
    "A": "0110000",
    "!D": "0001101",
    "!A": "0110001",
    "-D": "0001111",
    "-A": "0110011",
    "D+1": "0011111",
    "A+1": "0110111",
    "D-1": "0001110",
    "A-1": "0110010",
    "D+A": "0000010",
    "D-A": "0010011",
    "A-D": "0000111",
    "D&A": "0000000",
    "D|A": "0010101",
    "M": "1110000",
    "!M": "1110001",
    "-M": "1110011",
    "M+1": "1110111",
    "M-1": "1110010",
    "D+M": "1000010",
    "D-M": "1010011",
    "M-D": "1000111",
    "D&M": "1000000",
    "D|M": "1010101",
    "A+D": "0000010",
    "A&D": "0000000",
    "A|D": "0010101",
    "M+D": "1000010",
    "M&D": "1000000",
    "M|D": "1010101"
}

dest_table = {
    "null": "000",
    "M": "001",
    "D": "010",
    "DM": "011",
    "A": "100",
    "AM": "101",
    "AD": "110",
    "ADM": "111",
}

jump_table = {
    "null": "000",
    "JGT": "001",
    "JEQ": "010",
    "JGE": "011",
    "JLT": "100",
    "JNE": "101",
    "JLE": "110",
    "JMP": "111"
}

label_pattern = re.compile(r'^\(([^)]+)\)$')
a_pattern = re.compile(r'^@(.+)$')
c_pattern = re.compile(r'(?:(\w+)=)?([^;]+)(?:;(\w+))?')
comment_pattern = re.compile(r'//.*')

def int_to_binary(value: int) -> str:
    return format(value, '016b')

def translate_A(command, symbol_table, next_var) -> str:
    symbol = command[1:]
    if symbol.isdigit():
        value = int(symbol)
    else:
        if symbol not in symbol_table:
            symbol_table[symbol] = next_var[0]
            next_var[0] += 1
        value = symbol_table[symbol]

    return int_to_binary(value)

def translate_C(command) -> str:
    match = c_pattern.match(command)

    dest = match.group(1) or "null"
    comp = match.group(2).strip()
    jump = match.group(3) or "null"

    if dest != "null":
        dest = ''.join(sorted(dest))

    binary = "111"
    binary += comp_table[comp]
    binary += dest_table[dest]
    binary += jump_table[jump]

    return binary

def translate(command, symbol_table, next_var) -> str:
    if command.startswith("@"):
        return translate_A(command, symbol_table, next_var)
    else:
        return translate_C(command)

def first_pass(input_file: str) -> tuple:
    symbol_table = {
        "SP": 0,
        "LCL": 1,
        "ARG": 2,
        "THIS": 3,
        "THAT": 4,
        "SCREEN": 16384,
        "KBD": 24576,
    }
    commands = []
    pc = 0

    for i in range(16):
        symbol_table[f"R{i}"] = i

    with open(input_file, 'r') as file:
        for line in file:
            line = comment_pattern.sub('', line).strip()
            if not line:
                continue

            label_match = label_pattern.match(line)
            if label_match:
                symbol_table[label_match.group(1)] = pc
            else:
                commands.append(line)
                pc += 1

    return symbol_table, commands

def second_pass(commands, symbol_table, output_file) -> None:
    next_var = [16]
    with open(output_file, 'w') as out:
        for command in commands:
            if command.startswith("("):
                continue

            binary = translate(command, symbol_table, next_var)
            out.write(binary + "\n")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python assembler.py prog.asm")
        exit(-1)

    input_file = sys.argv[1]
    output_file = input_file.replace(".asm", ".hack")

    symbol_table, commands = first_pass(input_file)
    second_pass(commands, symbol_table, output_file)