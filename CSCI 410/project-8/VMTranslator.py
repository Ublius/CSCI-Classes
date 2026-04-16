import os
import sys

def read_vm_file(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    return [line.split('//')[0].strip() for line in lines if line.split('//')[0].strip() != ""]

def command_type(command):
    cmd = command.split()[0]
    return {
        "push": "C_PUSH",
        "pop": "C_POP",
        "label": "C_LABEL",
        "goto": "C_GOTO",
        "if-goto": "C_IF",
        "function": "C_FUNCTION",
        "call": "C_CALL",
        "return": "C_RETURN"
    }.get(cmd, "C_ARITHMETIC")

def arg1(command):
    tokens = command.split()
    return tokens[0] if command_type(command) == "C_ARITHMETIC" else (tokens[1] if len(tokens) >= 2 else None)

def arg2(command):
    tokens = command.split()
    return tokens[2] if len(tokens) >= 3 else None

def write_init(file):
    output = [
        "// Bootstrap code: initialize SP to 256",
        "@256",
        "D=A",
        "@SP",
        "M=D",
        "",
        "// call Sys.init 0",
    ]

    ret_label = "Sys.init$ret.0"
    output += [
        f"@{ret_label}",
        "D=A",
        "@SP",
        "A=M",
        "M=D",
        "@SP",
        "M=M+1",
    ]

    for seg in ["LCL","ARG","THIS","THAT"]:
        output += [
            f"@{seg}",
            "D=M",
            "@SP",
            "A=M",
            "M=D",
            "@SP",
            "M=M+1",
        ]

    output += [
        "@SP",
        "D=M",
        "@5",
        "D=D-A",
        "@ARG",
        "M=D",
        "@SP",
        "D=M",
        "@LCL",
        "M=D",
        "@Sys.init",
        "0;JMP",
        f"({ret_label})",
        ""
    ]

    for line in output:
        print(line, file=file)

def write_arithmetic(file, command, label_counter):
    output = [f"// {command}"]

    if command == "add":
        output += [
            "@SP", 
            "AM=M-1", 
            "D=M", 
            "A=A-1", 
            "M=D+M"
        ]

    elif command == "sub":
        output += [
            "@SP", 
            "AM=M-1", 
            "D=M", 
            "A=A-1", 
            "M=M-D"
        ]

    elif command == "neg":
        output += [
            "@SP", 
            "A=M-1", 
            "M=-M"
        ]

    elif command in ("eq", "gt", "lt"):
        true_label = f"TRUE{label_counter}"
        end_label = f"END{label_counter}"

        output += [
            "@SP", 
            "AM=M-1", 
            "D=M", 
            "A=A-1", 
            "D=M-D", 
            f"@{true_label}"
        ]

        if command == "eq": output.append("D;JEQ")
        elif command == "gt": output.append("D;JGT")
        elif command == "lt": output.append("D;JLT")

        output += [
            f"@SP", 
            "A=M-1", 
            "M=0", 
            f"@{end_label}", 
            "0;JMP", 
            f"({true_label})", 
            "@SP", 
            "A=M-1", 
            "M=-1", 
            f"({end_label})"
        ]

    elif command == "and":
        output += [
            "@SP", 
            "AM=M-1", 
            "D=M", 
            "A=A-1", 
            "M=D&M"
        ]

    elif command == "or":
        output += [
            "@SP", 
            "AM=M-1", 
            "D=M", 
            "A=A-1", 
            "M=D|M"
        ]

    elif command == "not":
        output += [
            "@SP",
            "A=M-1", 
            "M=!M"
        ]

    else:
        output += [f"// Unknown arithmetic command: {command}"]

    for line in output:
        print(line, file=file)

def write_push_pop(file, command, segment, index, current_file):
    output = [f"// {command.lower()} {segment} {index}"]
    if command == "C_PUSH":
        if segment == "constant":
            output += [
                f"@{index}", 
                "D=A", 
                "@SP", 
                "A=M", 
                "M=D", 
                "@SP", 
                "M=M+1"
            ]

        elif segment in ("local", "argument", "this", "that"):
            base = {"local":"LCL","argument":"ARG","this":"THIS","that":"THAT"}[segment]

            output += [
                f"@{index}", 
                "D=A", 
                f"@{base}", 
                "D=D+M", 
                "@R13", 
                "M=D", 
                "@R13", 
                "A=M", 
                "D=M", 
                "@SP", 
                "A=M", 
                "M=D", 
                "@SP", 
                "M=M+1"
            ]

        elif segment == "pointer":
            if index == "0": output += ["@THIS", "D=M", "@SP", "A=M", "M=D", "@SP", "M=M+1"]
            elif index == "1": output += ["@THAT", "D=M", "@SP", "A=M", "M=D", "@SP", "M=M+1"]

        elif segment == "temp":
            addr = 5 + int(index)
            output += [
                f"@{addr}", 
                "D=M", 
                "@SP", 
                "A=M", 
                "M=D", 
                "@SP", 
                "M=M+1"
            ]

        elif segment == "static":
            output += [
                f"@{current_file}.{index}", 
                "D=M", 
                "@SP", 
                "A=M", 
                "M=D", 
                "@SP", 
                "M=M+1"
            ]

    elif command == "C_POP":
        if segment in ("local","argument","this","that"):
            base = {"local":"LCL","argument":"ARG","this":"THIS","that":"THAT"}[segment]
            output   += [f"@{base}", "D=M", f"@{index}", "D=D+A", "@R13", "M=D", "@SP", "AM=M-1", "D=M", "@R13", "A=M", "M=D"]

        elif segment == "pointer":
            output += ["@SP", "AM=M-1", "D=M"]
            if index=="0": output += ["@THIS", "M=D"]
            elif index=="1": output += ["@THAT", "M=D"]

        elif segment == "temp":
            addr = 5 + int(index)
            output += [
                "@SP", 
                "AM=M-1", 
                "D=M", 
                f"@{addr}", 
                "M=D"
            ]

        elif segment == "static":
            output += [
                "@SP", 
                "AM=M-1", 
                "D=M", 
                f"@{current_file}.{index}", 
                "M=D"
            ]

    for line in output:
        print(line, file=file)

def write_label(file, label, current_function):
    qualified = f"{current_function}${label}" if current_function else label
    for output in [f"// label {label}", f"({qualified})"]:
        print(output, file=file)

def write_goto(file, label, current_function):
    qualified = f"{current_function}${label}" if current_function else label
    for output in [f"// goto {label}", f"@{qualified}", "0;JMP"]:
        print(output, file=file)

def write_if(file, label, current_function):
    qualified = f"{current_function}${label}" if current_function else label
    for output in [f"// if-goto {label}", "@SP", "AM=M-1", "D=M", f"@{qualified}", "D;JNE"]:
        print(output, file=file)

def write_function(file, function_name, n_vars):
    output = [f"// function {function_name} {n_vars}", f"({function_name})"]

    for _ in range(int(n_vars)):
        output += [
            "@0", 
            "D=A", 
            "@SP", 
            "A=M", 
            "M=D", 
            "@SP", 
            "M=M+1"
        ]

    for line in output:
        print(line, file=file)

def write_call(file, function_name, n_args, label_counter):
    ret_label = f"{function_name}$ret.{label_counter}"

    output = [
        f"// call {function_name} {n_args}", 
        f"@{ret_label}", 
        "D=A", 
        "@SP", 
        "A=M", 
        "M=D", 
        "@SP", 
        "M=M+1"
    ]

    for seg in ["LCL","ARG","THIS","THAT"]:
        output += [
            f"@{seg}", 
            "D=M", 
            "@SP", 
            "A=M", 
            "M=D", 
            "@SP", 
            "M=M+1"
        ]

    output += [
        "@SP", 
        "D=M", 
        f"@{int(n_args)+5}", 
        "D=D-A", 
        "@ARG", 
        "M=D", 
        "@SP", 
        "D=M", 
        "@LCL", 
        "M=D", 
        f"@{function_name}", 
        "0;JMP", 
        f"({ret_label})"
    ]
    
    for line in output:
        print(line, file=file)

def write_return(file):
    output = [
        "// return",
        "@LCL",
        "D=M",
        "@R13",
        "M=D",
        "@5",
        "A=D-A",
        "D=M",
        "@R14",
        "M=D",        
        "@SP",
        "AM=M-1",
        "D=M",
        "@ARG",
        "A=M",
        "M=D",          
        "@ARG",
        "D=M+1",
        "@SP",
        "M=D",
        "@R13",
        "AM=M-1",
        "D=M",
        "@THAT",
        "M=D",
        "@R13",
        "AM=M-1",
        "D=M",
        "@THIS",
        "M=D",
        "@R13",
        "AM=M-1",
        "D=M",
        "@ARG",
        "M=D",
        "@R13",
        "AM=M-1",
        "D=M",
        "@LCL",
        "M=D",
        "@R14",
        "A=M",
        "0;JMP"
    ]

    for line in output:
        print(line, file=file)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 VMTranslator.py [file.vm] | [directory]")
        sys.exit(1)

    input_path = sys.argv[1]
    if os.path.isdir(input_path):
        dir_name = os.path.basename(os.path.normpath(input_path))
        output_file = os.path.join(input_path, dir_name + ".asm")
        vm_files = [os.path.join(input_path,f) for f in os.listdir(input_path) if f.endswith(".vm")]
    else:
        output_file = input_path.replace(".vm",".asm")
        vm_files = [input_path]

    with open(output_file, "w") as out_file:
        label_counter = 0
        current_function = ""
        if len(vm_files) > 1:
            write_init(out_file)

        for vm_file in vm_files:
            commands = read_vm_file(vm_file)
            current_file = os.path.splitext(os.path.basename(vm_file))[0]

            for cmd in commands:
                ctype = command_type(cmd)
                if ctype == "C_ARITHMETIC":
                    write_arithmetic(out_file, arg1(cmd), label_counter)
                    if arg1(cmd) in ("eq","gt","lt"):
                        label_counter +=1
                elif ctype in ("C_PUSH","C_POP"):
                    write_push_pop(out_file, ctype, arg1(cmd), arg2(cmd), current_file)
                elif ctype == "C_LABEL":
                    write_label(out_file, arg1(cmd), current_function)
                elif ctype == "C_GOTO":
                    write_goto(out_file, arg1(cmd), current_function)
                elif ctype == "C_IF":
                    write_if(out_file, arg1(cmd), current_function)
                elif ctype == "C_FUNCTION":
                    current_function = arg1(cmd)
                    write_function(out_file, arg1(cmd), arg2(cmd))
                elif ctype == "C_CALL":
                    write_call(out_file, arg1(cmd), arg2(cmd), label_counter)
                    label_counter +=1
                elif ctype == "C_RETURN":
                    write_return(out_file)
                else:
                    print(f"// Unhandled command: {cmd}", file=out_file)