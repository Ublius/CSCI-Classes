import re

class JackTokenizer:
    KEYWORDS = {
        "class", "constructor", "function", "method", "field", "static",
        "var", "int", "char", "boolean", "void",
        "true", "false", "null", "this",
        "let", "do", "if", "else", "while", "return"
    }

    SYMBOLS = set('{}()[].,;+-*/&|<>=~')

    KEYWORD = 0
    SYMBOL = 1
    IDENTIFIER = 2
    INT_CONST = 3
    STRING_CONST = 4

    def __init__(self, inputFile):
        text = inputFile.read()

        text = re.sub(r"//.*?$", "", text, flags=re.MULTILINE)
        text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)

        self.text = text
        self.tokenIndex = 0
        self.currentToken = None
        self.tokenType = None

    def hasMoreTokens(self):
        i = self.tokenIndex
        while i < len(self.text):
            if not self.text[i].isspace():
                return True
            i += 1
        return False

    def advance(self):
        s = self.text
        n = len(s)

        while self.tokenIndex < n and s[self.tokenIndex].isspace():
            self.tokenIndex += 1

        if self.tokenIndex >= n:
            return

        c = s[self.tokenIndex]

        if c == '"':
            end = s.find('"', self.tokenIndex + 1)
            self.currentToken = s[self.tokenIndex:end + 1]
            self.tokenType = JackTokenizer.STRING_CONST
            self.tokenIndex = end + 1
            return

        if c in JackTokenizer.SYMBOLS:
            self.currentToken = c
            self.tokenType = JackTokenizer.SYMBOL
            self.tokenIndex += 1
            return

        if c.isdigit():
            match = re.match(r"\d+", s[self.tokenIndex:])
            self.currentToken = match.group(0)
            self.tokenType = JackTokenizer.INT_CONST
            self.tokenIndex += len(self.currentToken)
            return

        if c.isalpha() or c == "_":
            match = re.match(r"[a-zA-Z_][a-zA-Z0-9_]*", s[self.tokenIndex:])
            word = match.group(0)

            if word in JackTokenizer.KEYWORDS:
                self.tokenType = JackTokenizer.KEYWORD
            else:
                self.tokenType = JackTokenizer.IDENTIFIER

            self.currentToken = word
            self.tokenIndex += len(word)
            return

        raise Exception(f"Invalid token at: {s[self.tokenIndex:self.tokenIndex+10]}")

    def keyWord(self):
        self._check(self.KEYWORD)
        return self.currentToken

    def symbol(self):
        self._check(self.SYMBOL)

        if self.currentToken == "<":
            return "&lt;"
        elif self.currentToken == ">":
            return "&gt;"
        elif self.currentToken == "&":
            return "&amp;"
        elif self.currentToken == '"':
            return "&quot;"
        return self.currentToken

    def identifier(self):
        self._check(self.IDENTIFIER)
        return self.currentToken

    def intVal(self):
        self._check(self.INT_CONST)
        return self.currentToken

    def stringVal(self):
        self._check(self.STRING_CONST)
        return self.currentToken[1:-1]

    def _check(self, expected):
        if self.tokenType != expected:
            raise TypeError("Token type not available")