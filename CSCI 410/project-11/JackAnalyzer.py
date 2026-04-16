import os
from JackTokenizer import JackTokenizer
from CompilationEngine import CompilationEngine
    
def writeFile(inputFile, inputDirName):
    xmlFileName  = os.path.join(inputDirName, os.path.splitext(os.path.basename(inputFile.name))[0] + ".xml")
    vmFileName = os.path.join(inputDirName, os.path.splitext(os.path.basename(inputFile.name))[0] + ".vm") 
    
    if(not os.path.exists(os.path.dirname(xmlFileName))):
        os.makedirs(os.path.dirname(xmlFileName))
        
    xmlFile = open(xmlFileName, 'w')
    vmFile = open(vmFileName, 'w')
    tokenizer = JackTokenizer(inputFile)
    engine = CompilationEngine(tokenizer, xmlFile, vmFile)
    tokenizer = tokenizer.advance()
    engine = engine.compileClass()

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print("Usage: JackAnalyzer <inputFile.jack | inputDirectory>")
        sys.exit(1)

    input = sys.argv[1]
    files = []
    dir = os.path.splitext(input)[0]
    ext = os.path.splitext(input)[1]

    if ext == '':
        for currentFile in os.listdir(input):
            if os.path.splitext(currentFile)[1] == '.jack':
                files.append(os.path.join(input, currentFile))
    elif ext == '.jack':
        dir = os.path.split(input)[0] 
        files.append(input)
    else: 
        raise Exception("File provided doesn't have a .jack extension")

    for fileName in files:
        writeFile(open(fileName, 'r'), dir)