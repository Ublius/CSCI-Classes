import re
from utils import remove_comments

class JackTokenizer:
    symbols = set('{}()[].,;+-*/&|<>=~')
    keywords = {
        "class","constructor","function","method","field","static","var",
        "int","char","boolean","void","true","false","null","this",
        "let","do","if","else","while","return"
    }

    def __init__(self, input_stream):
        text = input_stream.read() if hasattr(input_stream, 'read') else input_stream
        text = remove_comments(text)

        token_pattern = re.compile(
            r'"[^"\n]*"|[A-Za-z_]\w*|\d+|[\{\}\(\)\[\]\.\,\;\+\-\*\/&\|<>=~]'
        )

        self.tokens = []
        for token in token_pattern.findall(text):
            if token.startswith('"'):
                self.tokens.append(("stringConstant", token[1:-1]))
            elif token.isdigit():
                self.tokens.append(("integerConstant", int(token)))
            elif token in self.keywords:
                self.tokens.append(("keyword", token))
            elif token in self.symbols:
                self.tokens.append(("symbol", token))
            else:
                self.tokens.append(("identifier", token))

        self.current = 0

    def has_more_tokens(self):
        return self.current < len(self.tokens)

    def peek(self):
        return self.tokens[self.current] if self.has_more_tokens() else None

    def advance(self):
        tok = self.tokens[self.current] if self.has_more_tokens() else None
        if tok:
            self.current += 1
        return tok