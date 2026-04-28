import sys
import os
import glob

from JackTokenizer import JackTokenizer
from VMWriter import VMWriter
from CompilationEngine import CompilationEngine

if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit("Usage: JackCompiler.py <input.jack or directory>")

    path = sys.argv[1]
    files = []

    if os.path.isdir(path):
        files = glob.glob(os.path.join(path, "*.jack"))
    elif path.endswith(".jack"):
        files = [path]
    else:
        sys.exit("Input must be a .jack file or directory")

    for jack_file in files:
        vm_file = jack_file.replace(".jack", ".vm")

        with open(jack_file, "r") as infile, open(vm_file, "w") as outfile:
            tokenizer = JackTokenizer(infile)
            writer = VMWriter(outfile)
            compiler = CompilationEngine(tokenizer, writer)
            compiler.compileClass()