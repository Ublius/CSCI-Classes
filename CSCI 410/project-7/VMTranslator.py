import os
import sys

symbols = {
    "add": "M=D+M",
    "sub": "M=M-D",
    "and": "M=D&M",
    "or": "M=D|M",
    "neg": "M=-M",
    "not": "M=!M",
    "eq": "D;JEQ",
    "gt": "D;JGT",
    "lt": "D;JLT",
    "local": "@LCL",
    "argument": "@ARG",
    "this": "@THIS",
    "that": "@THAT",
    "pointer": "@3",
    "temp": "@5"
}

arithmetic_commands = ["add", "sub", "neg", "eq", "gt", "lt", "and", "or", "not"]

def load_commands(file_name):
    commands = []
    with open(file_name) as file:
        for line in file:
            line = line.partition("//")[0].strip()
            if line:
                commands.append(line)
    return commands

def has_more_commands(current, commands):
    return (current + 1) < len(commands)

def advance(current):
    return current + 1

def command_type(command):
    cmd = command.split()[0]

    if cmd in arithmetic_commands:
        return "C_ARITHMETIC"
    elif cmd == "push":
        return "C_PUSH"
    elif cmd == "pop":
        return "C_POP"
    else:
        raise NameError("Unexpected Command Type")

def arg1(command):
    if command_type(command) == "C_ARITHMETIC":
        return command.split()[0]
    return command.split()[1]

def arg2(command):
    return int(command.split()[2])

def write_comment(file, command):
    print("// " + command, file=file)

def write_arithmetic(file, command, symbols, label_counter):
    output = []

    if command in ["add", "sub", "and", "or"]:
        output += [
            "@SP", 
            "AM=M-1", 
            "D=M", 
            "@SP", 
            "A=M-1", 
            symbols[command]
        ]

    elif command in ["neg", "not"]:
        output += [
            "@SP", 
            "A=M-1",
            symbols[command]
        ]

    elif command in ["eq", "gt", "lt"]:
        jump_label = f"CompLabel{label_counter}"
        label_counter += 1

        output += [
            "@SP", 
            "AM=M-1", 
            "D=M",
            "@SP", 
            "A=M-1",
            "D=M-D",
            "M=-1",
            f"@{jump_label}",
            symbols[command],
            "@SP", "A=M-1", "M=0",
            f"({jump_label})"
        ]
    else:
        raise NameError("Unexpected Arithmetic Command")

    output.append("")

    for line in output:
        print(line, file=file)

    return label_counter

def write_push_pop(file, command, segment, index, file_name, symbols):
    output = []

    if command == "C_PUSH":
        if segment == "constant":
            output += [
                f"@{index}", 
                "D=A",
                "@SP", 
                "AM=M+1", 
                "A=A-1", 
                "M=D"
            ]

        elif segment in ["local", "argument", "this", "that", "temp", "pointer"]:
            output += [f"@{index}", "D=A"]

            if segment in ["temp", "pointer"]:
                output.append(symbols[segment])
            else:
                output += [symbols[segment], "A=M"]

            output += [
                "A=D+A", 
                "D=M",
                "@SP", 
                "A=M", 
                "M=D",
                "@SP", 
                "M=M+1"
            ]

        elif segment == "static":
            output += [
                f"@{file_name}.{index}", 
                "D=M",
                "@SP", 
                "A=M", 
                "M=D",
                "@SP", 
                "M=M+1"
            ]

        else:
            raise NameError("Unexpected Push Segment")

    elif command == "C_POP":
        if segment == "constant":
            raise NameError("Cannot Pop Constant Segment")

        elif segment in ["local", "argument", "this", "that", "temp", "pointer"]:
            output += [f"@{index}", "D=A"]

            if segment in ["temp", "pointer"]:
                output.append(symbols[segment])
            else:
                output += [symbols[segment], "A=M"]

            output += [
                "D=D+A",
                "@R13", 
                "M=D",
                "@SP", 
                "AM=M-1", 
                "D=M",
                "@R13", 
                "A=M", 
                "M=D"
            ]

        elif segment == "static":
            output += [
                "@SP", 
                "AM=M-1", 
                "D=M",
                f"@{file_name}.{index}", 
                "M=D"
            ]

        else:
            raise NameError("Unexpected Pop Segment")

    else:
        raise NameError("Unexpected Command Type")

    output.append("")

    for line in output:
        print(line, file=file)

if __name__ == "__main__":
    if len(sys.argv) != 2 or not sys.argv[1].endswith(".vm"):
        print("Usage: python script.py file.vm")
        exit(1)

    input_file = sys.argv[1]
    output_file = input_file[:-3] + ".asm"

    commands = load_commands(input_file)
    current = -1

    file_name = os.path.basename(input_file)[:-3]

    label_counter = 0

    with open(output_file, "w") as out_file:
        while has_more_commands(current, commands):
            current = advance(current)
            command = commands[current]

            write_comment(out_file, command)

            ctype = command_type(command)

            if ctype == "C_ARITHMETIC":
                label_counter = write_arithmetic(
                    out_file, arg1(command), symbols, label_counter)

            elif ctype in ["C_PUSH", "C_POP"]:
                write_push_pop(out_file, ctype, arg1(command), arg2(command), file_name, symbols)
            else:
                raise NameError("Unsupported Command Type")