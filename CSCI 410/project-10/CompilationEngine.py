from JackTokenizer import JackTokenizer

class CompilationEngine:
    keywordConsts = ["null", "true", "false", "this"]
    tokenizer = None
    outputFile = None
    indentLevel = 0
    currentIndex = 0

    def __init__(self, tokenizer, outputFile):
        self.tokenizer = tokenizer
        self.outputFile = outputFile
        self.indentLevel = 0

    def compileClass(self):
        indentLevel = 0
        NUM_OPENING_STATEMENTS = 3
        classVarOpenings = ['static', 'field']
        subOpenings = ['constructor', 'function', 'method']

        if self.tokenizer.currentToken != "class":
            raise Exception("Keyword 'class' expected")
        self.writeFormatted("<class>")
        self.indentLevel += 1
        i = 0
        while self.tokenizer.hasMoreTokens() and i < NUM_OPENING_STATEMENTS: 
            self.printToken()
            self.tokenizer.advance()
            i += 1
        
        classVarCount = 0
        while self.tokenizer.hasMoreTokens() and self.tokenizer.keyWord() in classVarOpenings:
            classVarCount += 1
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
        NUM_OPENING_STATEMENTS = 3
        i = 0
        while self.tokenizer.hasMoreTokens() and i < NUM_OPENING_STATEMENTS:
            self.printToken()
            self.tokenizer.advance()
            i += 1

        while self.tokenizer.symbol() != ";" and self.tokenizer.hasMoreTokens():
            if self.tokenizer.symbol() != ",":
                raise Exception("Invalid variable list")
            self.printToken()
            self.tokenizer.advance()
            self.printToken()
            if not self.tokenizer.hasMoreTokens():
                raise Exception("Another variable expected in list")
            self.tokenizer.advance()
        self.printToken()

        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</classVarDec>")

    def compileSubroutine(self):
        self.writeFormatted("<subroutineDec>")
        self.indentLevel += 1
        NUM_OPENING_STATEMENTS = 4
        i = 0
        while self.tokenizer.hasMoreTokens() and i < NUM_OPENING_STATEMENTS:
            self.printToken()
            self.tokenizer.advance()
            i += 1
        self.compileParameterList()
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
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
        self.compileStatements()
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</subroutineBody>")

    def compileParameterList(self):
        self.writeFormatted("<parameterList>")
        self.indentLevel += 1
        while self.tokenizer.tokenType != JackTokenizer.SYMBOL or self.tokenizer.symbol() != ")":
            boolVal = self.tokenizer.tokenType != JackTokenizer.SYMBOL or self.tokenizer.symbol() != ")"
            self.printToken()
            if self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() == ",":
                self.tokenizer.advance()
                self.printToken()
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
            self.printToken()
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</parameterList>")

    def compileVarDec(self):
        self.writeFormatted("<varDec>")
        self.indentLevel += 1
        while(self.tokenizer.hasMoreTokens() and 
                (self.tokenizer.tokenType != JackTokenizer.SYMBOL or self.tokenizer.symbol() != ";")):
            self.printToken()
            self.tokenizer.advance()
        self.printToken()
        self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</varDec>")

    def compileStatements(self):
        self.writeFormatted("<statements>")
        self.indentLevel += 1
        stmtStarts = ['do', 'while', 'let', 'if', 'return']
        while(self.tokenizer.hasMoreTokens() and self.tokenizer.tokenType == JackTokenizer.KEYWORD 
                and self.tokenizer.keyWord() in stmtStarts):
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
        while (self.tokenizer.hasMoreTokens() and 
                (self.tokenizer.tokenType != JackTokenizer.SYMBOL or self.tokenizer.symbol() != "(")):
            self.tokenizer.advance() 
            self.printToken()
        self.tokenizer.advance()
        self.compileExpressionList()
        self.printToken()
        self.tokenizer.advance()
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</doStatement>")

    def compileLet(self):
        self.writeFormatted("<letStatement>")
        self.indentLevel += 1
        if self.tokenizer.keyWord() != "let":
            raise Exception("Let keyword expected")
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
        if self.tokenizer.currentToken == "[":
            self.tokenizer.advance()
            self.compileExpression()
            self.printToken()
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
                self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        self.compileExpression()
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</letStatement>")

    def compileWhile(self):
        self.writeFormatted("<whileStatement>")
        self.indentLevel += 1
        if not(self.tokenizer.tokenType == JackTokenizer.KEYWORD and self.tokenizer.keyWord() == "while"):
            raise Exception("'while' keyword was expected")
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.compileExpression()
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
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</returnStatement>")

    def compileIf(self):
        self.writeFormatted("<ifStatement>")
        self.indentLevel += 1
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
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.compileStatements()
            self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
        if not(self.tokenizer.tokenType == JackTokenizer.KEYWORD and self.tokenizer.keyWord() == "else"):
            self.writeFormatted("</ifStatement>")
            return
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
        self.indentLevel -= 1
        self.writeFormatted("</ifStatement>")

    def compileExpression(self):
        opList = ['+', '-', '*', '/', '&amp;', '|', '&lt;', '&gt;', '=']
        self.writeFormatted("<expression>")
        self.indentLevel += 1
        self.compileTerm()
        while self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() in opList:
            self.printToken()
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
                self.compileTerm()
        self.indentLevel -= 1
        self.writeFormatted("</expression>")

    def compileTerm(self):
        unaryOps = ['-', '~']
        self.writeFormatted("<term>")
        self.indentLevel += 1
        self.printToken()
        if self.tokenizer.tokenType == JackTokenizer.IDENTIFIER:
            self.tokenizer.advance()
            if self.tokenizer.tokenType == JackTokenizer.SYMBOL:
                if self.tokenizer.symbol() == ".":
                    self.printToken()
                    if self.tokenizer.hasMoreTokens():
                        self.tokenizer.advance()
                        self.compileSubroutineCall()
                elif self.tokenizer.symbol() == "(":
                    self.printToken()
                    if self.tokenizer.hasMoreTokens():
                        self.tokenizer.advance()
                        self.compileExpressionList()
                        self.printToken()
                        if self.tokenizer.hasMoreTokens():
                            self.tokenizer.advance()
                elif self.tokenizer.symbol() == "[":
                    self.printToken()
                    if self.tokenizer.hasMoreTokens():
                        self.tokenizer.advance()
                        self.compileExpression()
                        self.printToken()
                        if self.tokenizer.hasMoreTokens():
                            self.tokenizer.advance()
        elif self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() == "(":
            self.tokenizer.advance()
            self.compileExpression()
            self.printToken()
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
        elif self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() in unaryOps:
            self.tokenizer.advance()
            self.compileTerm()
        elif(self.tokenizer.currentToken in CompilationEngine.keywordConsts or 
                self.tokenizer.tokenType == JackTokenizer.INT_CONST or 
                self.tokenizer.tokenType == JackTokenizer.STRING_CONST):
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance() 
        else:
            raise Exception("Invalid term provided")
        self.indentLevel -= 1
        self.writeFormatted("</term>")

    def compileExpressionList(self):
        self.writeFormatted("<expressionList>")
        self.indentLevel += 1
        while not(self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() == ")"):
            self.compileExpression() 
            if self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() == ",":
                self.printToken()
                if self.tokenizer.hasMoreTokens():
                    self.tokenizer.advance()
        self.indentLevel -= 1
        self.writeFormatted("</expressionList>")

    def compileSubroutineCall(self):
        self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.printToken()
        if self.tokenizer.tokenType == JackTokenizer.SYMBOL and self.tokenizer.symbol() == ".":
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance() 
                self.printToken()
            if self.tokenizer.hasMoreTokens():
                self.tokenizer.advance()
                self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()
            self.compileExpressionList()
            self.printToken()
        if self.tokenizer.hasMoreTokens():
            self.tokenizer.advance()

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