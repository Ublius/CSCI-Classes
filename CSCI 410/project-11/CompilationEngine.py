from JackTokenizer import JackTokenizer
from VMWriter import VMWriter
from SymbolTable import SymbolTable

class CompilationEngine:
    keywordConsts = ["null", "true", "false", "this"] 

    def __init__(self, tokenizer, outputFile, vmFile):
        self.tokenizer = tokenizer
        self.outputFile = outputFile
        self.symbolTable = SymbolTable()
        self.vmWriter = VMWriter(vmFile)
        self.labelNum = 0
    
    def compileClass(self):
        self.indentLevel = 0
        NUM_OPENING_STATEMENTS = 3
        classVarOpenings = ['static', 'field']
        subOpenings = ['constructor', 'function', 'method']

        if self.tokenizer.currentToken != "class":
            raise Exception("Keyword 'class' expected")
        self.writeFormatted("<class>")
        self.indentLevel += 1
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
            self.className = self.tokenizer.identifier()
            self.writeClassOrSubInfo("class", False)

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
        
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        
        self.fieldCount = 0
        while self.tokenizer.hasMoreTokens() and self.tokenizer.keyWord() in classVarOpenings:
            if self.tokenizer.keyWord() == "field":
                self.fieldCount += 1
            self.compileClassVarDec()
        while(self.tokenizer.hasMoreTokens() and self.tokenizer.tokenType == JackTokenizer.KEYWORD 
                and self.tokenizer.keyWord() in subOpenings):
            self.compileSubroutine()
        self.printToken()
        self.writeFormatted("</class>")
        self.indentLevel -= 1
    
    def compileClassVarDec(self):
        self.writeFormatted("<classVarDec>")
        self.indentLevel += 1
        self.printToken()
        if self.tokenizer.tokenType == JackTokenizer.KEYWORD:
            if self.tokenizer.keyWord() == "static":
                kind = SymbolTable.STATIC
            elif self.tokenizer.keyWord() == "field":
                kind = SymbolTable.FIELD
            else:
                raise Exception("Invalid kind of class variable " + self.tokenizer.keyWord())
        else:
            raise Exception("Keyword expected")
        
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
            identifierType = self.tokenizer.currentToken
            isKeyword = self.tokenizer.tokenType == JackTokenizer.KEYWORD

        if not isKeyword:
            self.writeClassOrSubInfo("class", True)

        varNames = []
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
            varNames.append(self.tokenizer.currentToken)
            if self.tokenizer.hasMoreTokens(): 
                self.tokenizer.advance()

        while self.tokenizer.symbol() != ";" and self.tokenizer.hasMoreTokens():
            if self.tokenizer.symbol() != ",":
                raise Exception("Invalid variable list")
            self.printToken()
            self.tokenizer.advance()
            self.printToken()
            varNames.append(self.tokenizer.currentToken)
            if kind == SymbolTable.FIELD:
                self.fieldCount += 1
            if not self.tokenizer.hasMoreTokens():
                raise Exception("More tokens expected")
            self.tokenizer.advance()
        self.printToken()
    
        for name in varNames:
            self.symbolTable.define(name, identifierType, kind)
            self.writeVarInfo(name, False)

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</classVarDec>")

    def compileSubroutine(self):
        self.writeFormatted("<subroutineDec>")
        self.symbolTable.startSubroutine()
        self.indentLevel += 1
        NUM_OPENING_STATEMENTS = 4
        
        self.printToken()
        self.isConstructor = False
        self.isMethod = False
        if self.tokenizer.keyWord() == "constructor":
            self.isConstructor = True
        elif self.tokenizer.keyWord() == "method":
            self.isMethod = True
            self.symbolTable.define("this", self.className, SymbolTable.ARG)

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance() 
            self.printToken()

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance() 
            self.printToken()
            self.subName = self.tokenizer.identifier()

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance() 
            self.printToken()

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance() 

        self.compileParameterList()
        self.printToken()

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()

        self.numLocalVariables = 0
        self.compileSubroutineBody()
        self.indentLevel -= 1
        self.writeFormatted("</subroutineDec>")
    
    def compileSubroutineBody(self):
        self.writeFormatted("<subroutineBody>")
        self.indentLevel += 1
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        
        while(self.tokenizer.hasMoreTokens() and self.tokenizer.tokenType == JackTokenizer.KEYWORD
                and self.tokenizer.keyWord() == "var"):
            self.compileVarDec()
        
        self.vmWriter.writeFunction(self.className + "." + self.subName, self.numLocalVariables) 
        if self.isConstructor:
            self.vmWriter.writePush("constant", self.fieldCount)
            self.vmWriter.writeCall("Memory.alloc", 1)
            self.vmWriter.writePop("pointer", 0)
        elif self.isMethod:
            self.vmWriter.writePush(SymbolTable.ARG, 0)
            self.vmWriter.writePop("pointer", VMWriter.THIS_POINTER)
            
        self.compileStatements()
        self.printToken()

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()

        self.indentLevel -= 1
        self.writeFormatted("</subroutineBody>")

    def compileParameterList(self):
        self.writeFormatted("<parameterList>")
        self.indentLevel += 1

        if self.tokenizer.currentToken != ")":
            self.printToken()
            argType = self.tokenizer.currentToken
            self.tokenizer.advance()
            self.printToken()
            argName = self.tokenizer.currentToken
            self.symbolTable.define(argName, argType, SymbolTable.ARG)
            self.writeVarInfo(argName, False)
            self.tokenizer.advance()

        while self.tokenizer.tokenType != JackTokenizer.SYMBOL or self.tokenizer.symbol() != ")":
            self.printToken()
            if self.tokenizer.currentToken != ",":
                raise Exception("Comma expected")
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
                self.printToken()
                argType = self.tokenizer.currentToken
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
                self.printToken()
                argName = self.tokenizer.currentToken
                self.symbolTable.define(argName, argType, SymbolTable.ARG)
                self.writeVarInfo(argName, False)
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
            
        self.indentLevel -= 1
        self.writeFormatted("</parameterList>")

    def compileVarDec(self):
        self.numLocalVariables += 1
        self.writeFormatted("<varDec>")
        self.indentLevel += 1
        
        varNames = []
        self.printToken()
        if self.tokenizer.currentToken != "var":
            raise Exception("'var' keyword expected")
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
            varType = self.tokenizer.currentToken
            isKeyword = self.tokenizer.tokenType == JackTokenizer.KEYWORD

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance() 
            self.printToken()
            varNames.append(self.tokenizer.currentToken) 
        
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()

        while(self.tokenizer.hasMoreTokens() and 
                (self.tokenizer.tokenType != JackTokenizer.SYMBOL or self.tokenizer.symbol() != ";")):
            self.printToken()
            self.tokenizer.advance()
            self.printToken()
            varNames.append(self.tokenizer.currentToken)
            self.tokenizer.advance()
            self.numLocalVariables += 1
        
        if not isKeyword:
            self.writeClassOrSubInfo("class", "True")

        for name in varNames:
            self.symbolTable.define(name, varType, SymbolTable.VAR)
            self.writeVarInfo(name, False)

        self.printToken()
        self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</varDec>")

    def compileStatements(self):
        self.writeFormatted("<statements>")
        self.indentLevel += 1
        stmtStarts = ['do', 'while', 'let', 'if', 'return']
        while(self.tokenizer.hasMoreTokens() and self.tokenizer.tokenType == JackTokenizer.KEYWORD and self.tokenizer.keyWord() in stmtStarts):
            if self.tokenizer.keyWord() == "do":
                self.compileDo()
            elif self.tokenizer.keyWord() == "while":
                self.compileWhile()
            elif self.tokenizer.keyWord() == "let":
                self.compileLet()
            elif self.tokenizer.keyWord() == "if":
                self.compileIf()
            elif self.tokenizer.keyWord() == "return":
                self.compileReturn()
        self.indentLevel -= 1
        self.writeFormatted("</statements>")

    def compileDo(self):
        self.writeFormatted("<doStatement>")
        self.indentLevel += 1
        if self.tokenizer.keyWord() != "do":
            raise Exception("'do' keyword expected")
        self.printToken()

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.compileSubroutineCall()
            self.vmWriter.writePop("temp", 0)

        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</doStatement>")

    def compileLet(self):
        self.writeFormatted("<letStatement>")
        self.indentLevel += 1
        isArray = False
        if self.tokenizer.keyWord() != "let":
            raise Exception("Let keyword expected")
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
            varName = self.tokenizer.identifier()
            self.writeVarInfo(self.tokenizer.identifier(), True)
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
        if self.tokenizer.currentToken == "[":
            isArray = True
            self.tokenizer.advance()

            if self.symbolTable.isDefined(varName):
                varKind = self.symbolTable.kindOf(varName)
                self.vmWriter.writePush(varKind, self.symbolTable.indexOf(varName))

                self.compileExpression()
                self.printToken()
                self.vmWriter.writeArithmetic("add")
                segment = "that"
                index = 0

            else:
                raise Exception("Symbol " + varName + " is not defined")
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
                self.printToken()
        else:
            varKind = self.symbolTable.kindOf(varName)
            segment = varKind
            index = self.symbolTable.indexOf(varName)

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()

        self.compileExpression()
        if isArray:
            self.vmWriter.writePop("temp", 0)
            self.vmWriter.writePop("pointer", VMWriter.THAT_POINTER)
            self.vmWriter.writePush("temp", 0)

        self.vmWriter.writePop(segment, index)
        self.printToken()
        
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</letStatement>")

    def compileWhile(self):
        self.writeFormatted("<whileStatement>")
        self.indentLevel += 1

        self.labelNum += 1
        firstLabel = "W" + str(self.labelNum)
        self.labelNum += 1
        secondLabel = "W" + str(self.labelNum)

        if not(self.tokenizer.tokenType == JackTokenizer.KEYWORD and self.tokenizer.keyWord() == "while"):
            raise Exception("'while' keyword was expected")
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
            self.vmWriter.writeLabel(firstLabel)
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.compileExpression()
            self.vmWriter.writeArithmetic("not")
            self.vmWriter.writeIf(secondLabel)
            self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.compileStatements()
            self.vmWriter.writeGoto(firstLabel)
            self.printToken()
            self.vmWriter.writeLabel(secondLabel)
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</whileStatement>")

    def compileReturn(self):
        self.writeFormatted("<returnStatement>")
        self.indentLevel += 1
        if self.tokenizer.keyWord() != "return":
            raise Exception("'return' keyword was expected")
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        if not(self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() == ";"):
            self.compileExpression()
        else:
            self.vmWriter.writePush("constant", 0)
        self.printToken()
        self.vmWriter.writeReturn()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</returnStatement>")

    def compileIf(self):
        self.writeFormatted("<ifStatement>")
        self.indentLevel += 1
        trueLabel = "IF-TRUE" + str(self.labelNum)
        falseLabel = "IF-FALSE" + str(self.labelNum)
        endLabel = "END-IF" + str(self.labelNum)
        self.labelNum += 1
        if self.tokenizer.keyWord() != "if":
            raise Exception("'if' keyword was expected")
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.compileExpression()
            self.printToken()
            self.vmWriter.writeIf(trueLabel)
            self.vmWriter.writeGoto(falseLabel)
            self.vmWriter.writeLabel(trueLabel)
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.compileStatements()
            self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        if self.tokenizer.tokenType == JackTokenizer.KEYWORD and self.tokenizer.keyWord() == "else":
            self.vmWriter.writeGoto(endLabel)
            self.vmWriter.writeLabel(falseLabel)
            self.printToken()
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
                self.printToken()
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
                self.compileStatements()
            self.printToken()
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
            self.vmWriter.writeLabel(endLabel)
        else:
            self.vmWriter.writeLabel(falseLabel)
        self.indentLevel -= 1
        self.writeFormatted("</ifStatement>")

    def compileExpression(self):
        functionSymbols = [ '*', '/']
        functionNames = ["Math.multiply", "Math.divide"]
        builtInCommands = ["add", "sub", "and", "or", "lt", "gt", "eq"]
        builtInSymbols = ['+', '-', '&amp;', '|', '&lt;', '&gt;', '=']
        self.writeFormatted("<expression>")
        self.indentLevel += 1
        self.compileTerm()
        while(self.tokenizer.tokenType == JackTokenizer.SYMBOL and 
                (self.tokenizer.symbol() in builtInSymbols or self.tokenizer.symbol() in functionSymbols)):
            self.printToken()
            operator = self.tokenizer.symbol() 
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
                self.compileTerm()

            if operator in builtInSymbols:
                self.vmWriter.writeArithmetic(builtInCommands[builtInSymbols.index(operator)])
            else:
                self.vmWriter.writeCall(functionNames[functionSymbols.index(operator)], 2)

            
        self.indentLevel -= 1
        self.writeFormatted("</expression>")

    def compileTerm(self):
        unaryOps = ['-', '~']
        unaryCommands = ["neg", "not"]
        self.writeFormatted("<term>")
        self.indentLevel += 1
        self.printToken()
        if self.tokenizer.tokenType == JackTokenizer.IDENTIFIER:
            name = self.tokenizer.identifier()
            self.tokenizer.advance()
            if self.tokenizer.tokenType == JackTokenizer.SYMBOL:
                if self.tokenizer.symbol() == ".":
                    if self.symbolTable.isDefined(name):
                        self.writeVarInfo(name, True)
                    else:
                        self.writeClassOrSubInfo("class", True)

                    self.printToken()
                    self.tokenizer.advance()
                    self.printToken()
                    subName = self.tokenizer.identifier()

                    self.tokenizer.advance() 
                    self.printToken()

                    if self.symbolTable.isDefined(name):
                        self.vmWriter.writePush(self.symbolTable.kindOf(name), self.symbolTable.indexOf(name))

                    self.numExpressions = 0
                    self.tokenizer.advance()
                    self.compileExpressionList()
                    self.printToken()
                    
                    if self.symbolTable.isDefined(name):
                        self.vmWriter.writeCall(self.symbolTable.typeOf(name) + "." + subName, self.numExpressions + 1)
                    else:
                        self.vmWriter.writeCall(name + "." + subName, self.numExpressions)

                    self.tokenizer.advance()
                elif self.tokenizer.symbol() == "(":
                    self.printToken()
                    self.writeClassOrSubInfo("subroutine", True)
                    if self.tokenizer.hasMoreTokens():
                        self.tokenizer.advance()

                        self.vmWriter.writePush("pointer", VMWriter.THIS_POINTER)
                        self.compileExpressionList()
                        self.numExpressions += 1

                        self.printToken()
                        self.vmWriter.writeCall(self.className +  "." + name, self.numExpressions)
                        if self.tokenizer.hasMoreTokens():
                            self.tokenizer.advance()
                elif self.tokenizer.symbol() == "[":
                    self.writeVarInfo(name, True)
                    self.printToken()

                    self.vmWriter.writePush(self.symbolTable.kindOf(name), self.symbolTable.indexOf(name))
                    if self.tokenizer.hasMoreTokens():
                        self.tokenizer.advance()
                        self.compileExpression()
                        self.printToken()

                        self.vmWriter.writeArithmetic("add")
                        self.vmWriter.writePop("pointer", VMWriter.THAT_POINTER)
                        self.vmWriter.writePush("that", 0)
                        if self.tokenizer.hasMoreTokens():
                            self.tokenizer.advance()
                else:
                    self.vmWriter.writePush(self.symbolTable.kindOf(name), self.symbolTable.indexOf(name))
                    self.writeVarInfo(name, True)
        elif self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() == "(":
            self.tokenizer.advance()
            self.compileExpression()
            self.printToken()
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
        elif self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() in unaryOps:
            op = self.tokenizer.symbol()
            self.tokenizer.advance()
            self.compileTerm()
            self.vmWriter.writeArithmetic(unaryCommands[unaryOps.index(op)])
        elif self.tokenizer.tokenType == JackTokenizer.INT_CONST:
            self.vmWriter.writePush("constant", self.tokenizer.intVal())
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
        elif self.tokenizer.currentToken in CompilationEngine.keywordConsts:
            if self.tokenizer.keyWord() == "null" or self.tokenizer.keyWord() == "false":
                self.vmWriter.writePush("constant", 0)
            elif self.tokenizer.keyWord() == "true":
                self.vmWriter.writePush("constant", 1)
                self.vmWriter.writeArithmetic("neg")
            elif self.tokenizer.keyWord() == "this":
                self.vmWriter.writePush("pointer", VMWriter.THIS_POINTER) 
            else:
                raise Exception("Invalid keyword constant " + self.tokenizer.keyWord())

            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
        elif self.tokenizer.tokenType == JackTokenizer.STRING_CONST:
            self.vmWriter.writePush("constant", len(self.tokenizer.stringVal()))
            self.vmWriter.writeCall("String.new", 1)
            for char in self.tokenizer.stringVal():
                self.vmWriter.writePush("constant", ord(char))
                self.vmWriter.writeCall("String.appendChar", 2)

            if self.tokenizer.hasMoreTokens():
               self.tokenizer.advance() 
        else:
            raise Exception("Invalid term provided")
        self.indentLevel -= 1
        self.writeFormatted("</term>")

    def compileExpressionList(self):
        self.writeFormatted("<expressionList>")
        self.indentLevel += 1
        self.numExpressions = 0
        while not(self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() == ")"):
           self.compileExpression() 
           self.numExpressions += 1
           if self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() == ",":
               self.printToken()
               if self.tokenizer.hasMoreTokens():
                   self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</expressionList>")

    def compileSubroutineCall(self):
        self.printToken()
        firstToken = self.tokenizer.currentToken
        secondToken = ""
        isClassOrObj = False

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()

        if self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() == ".":
            isClassOrObj = True

            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance() 
                self.printToken()
                secondToken = self.tokenizer.currentToken

            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
                self.printToken()

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()

            if secondToken != "" and self.symbolTable.isDefined(firstToken):
                self.vmWriter.writePush(self.symbolTable.kindOf(firstToken), self.symbolTable.indexOf(firstToken))
            
            if secondToken == "":
                self.vmWriter.writePush("pointer", VMWriter.THIS_POINTER)

            self.compileExpressionList()

            self.printToken()

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()

        if secondToken != "":
            if self.symbolTable.isDefined(firstToken):
                callName = self.symbolTable.typeOf(firstToken) + "." + secondToken
                self.numExpressions += 1
            else:
                callName = firstToken + "." + secondToken
        
        else:
            self.numExpressions += 1
            callName = self.className + "." + firstToken

        self.vmWriter.writeCall(callName, self.numExpressions) 

        if isClassOrObj and self.symbolTable.isDefined(firstToken):
            self.writeVarInfo(firstToken, True)
        elif isClassOrObj:
            self.writeClassOrSubInfo("class", True)
        self.writeClassOrSubInfo("subroutine", True)

    def printToken(self):
        if self.tokenizer.tokenType == JackTokenizer.KEYWORD:
           self.writeFormatted("<keyword>" + self.tokenizer.keyWord() + "</keyword>")
        elif self.tokenizer.tokenType == JackTokenizer.SYMBOL:
            self.writeFormatted("<symbol>" + self.tokenizer.symbol() + "</symbol>")
        elif self.tokenizer.tokenType == JackTokenizer.IDENTIFIER:
            self.writeFormatted("<identifier>" + self.tokenizer.identifier() + "</identifier>")
        elif self.tokenizer.tokenType == JackTokenizer.INT_CONST:
            self.writeFormatted("<integerConstant>" + self.tokenizer.intVal() + "</integerConstant>")
        elif self.tokenizer.tokenType == JackTokenizer.STRING_CONST:
            self.writeFormatted("<stringConstant>" + self.tokenizer.stringVal() + "</stringConstant>")

    def writeFormatted(self, string):
        self.outputFile.write("  " * self.indentLevel + string + "\n")
    
    def writeVarInfo(self, varName, inUse):
        self.writeFormatted("<IdentifierInfo>")
        self.indentLevel += 1
        self.writeFormatted("<type>" + self.symbolTable.typeOf(varName) + "</type>")
        self.writeFormatted("<kind>" + self.symbolTable.stringKindOf(varName) + "</kind>")
        self.writeFormatted("<index>" + str(self.symbolTable.indexOf(varName)) + "</index>")
        self.writeFormatted("<inUse>" + str(inUse) + "</inUse>")
        self.indentLevel -= 1
        self.writeFormatted("</IdentifierInfo>")

    def writeClassOrSubInfo(self, kind, inUse):
       self.writeFormatted("<IdentifierInfo>")
       self.indentLevel += 1
       self.writeFormatted("<kind>" + kind + "</kind>")
       self.writeFormatted("<inUse>" + str(inUse) + "</inUse>")
       self.indentLevel -= 1
       self.writeFormatted("</IdentifierInfo>")