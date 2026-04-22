#!/usr/bin/python3

import re
import sys

def remove_comments(text: str) -> str:
    """Remove all Jack comments from the source text."""
    result = []
    i, n = 0, len(text)
    in_block = in_line = in_string = False
    while i < n:
        if in_string:
            # End of string literal
            if text[i] == '"':
                in_string = False
            result.append(text[i])
            i += 1
        elif in_block:
            # End of block comment
            if text[i] == '*' and i+1 < n and text[i+1] == '/':
                in_block = False
                i += 2
            else:
                i += 1
        elif in_line:
            # End of line comment
            if text[i] == '\n':
                in_line = False
                result.append(text[i])
            i += 1
        else:
            # Not in a comment or string
            if text[i] == '/' and i+1 < n:
                if text[i+1] == '/':
                    in_line = True
                    i += 2
                    continue
                elif text[i+1] == '*':
                    in_block = True
                    i += 2
                    continue
            if text[i] == '"':
                in_string = True
            result.append(text[i])
            i += 1
    return "".join(result)

class JackTokenizer:
    """Tokenizes Jack source code into tokens of type: keyword, symbol, identifier, intConst, stringConst."""
    symbols = set('{}()[].,;+-*/&|<>=~')
    keywords = {
        "class","constructor","function","method","field","static","var",
        "int","char","boolean","void","true","false","null","this",
        "let","do","if","else","while","return"
    }
    def __init__(self, input_stream):
        # Read the whole file or string and strip comments/whitespace
        text = input_stream.read() if hasattr(input_stream, 'read') else input_stream
        text = remove_comments(text)
        # Regex pattern to match a string constant, integer, identifier, or symbol
        token_pattern = re.compile(r'"[^"\n]*"|[A-Za-z_]\w*|\d+|[\{\}\(\)\[\]\.\,\;\+\-\*\/&\|<>=~]')
        self.tokens = []
        for token in token_pattern.findall(text):
            if token.startswith('"'):
                # Strip quotes for string constant
                self.tokens.append(("stringConstant", token[1:-1]))
            elif token.isdigit():
                self.tokens.append(("integerConstant", int(token)))
            elif token in JackTokenizer.keywords:
                self.tokens.append(("keyword", token))
            elif token in JackTokenizer.symbols:
                self.tokens.append(("symbol", token))
            else:
                self.tokens.append(("identifier", token))
        self.current = 0  # index of next token

    def has_more_tokens(self) -> bool:
        return self.current < len(self.tokens)

    def peek(self):
        """Return the next token tuple without advancing."""
        return self.tokens[self.current] if self.has_more_tokens() else None

    def advance(self):
        """Return the next token and advance the pointer."""
        tok = self.tokens[self.current] if self.has_more_tokens() else None
        if tok: 
            self.current += 1
        return tok

class SymbolTable:
    """Keeps a symbol table for class and subroutine scopes, with types, kinds, and running indices."""
    def __init__(self):
        self.class_table = {}       # symbols in class scope
        self.subroutine_table = {}  # symbols in subroutine scope
        # Running index counters for each kind
        self.index_counter = {'static': 0, 'field': 0, 'arg': 0, 'var': 0}

    def start_subroutine(self):
        """Reset the subroutine scope (for starting a new method/function)."""
        self.subroutine_table.clear()
        self.index_counter['arg'] = 0
        self.index_counter['var'] = 0

    def define(self, name: str, type_name: str, kind: str):
        """Define a new identifier of given kind and type, assign it the next index."""
        kind = 'var' if kind == 'local' else kind  # treat 'local' as 'var'
        if kind in ('static', 'field'):
            index = self.index_counter[kind]
            self.class_table[name] = (type_name, kind, index)
            self.index_counter[kind] += 1
        elif kind in ('arg', 'var'):
            index = self.index_counter[kind]
            self.subroutine_table[name] = (type_name, kind, index)
            self.index_counter[kind] += 1
        else:
            raise ValueError(f"Unknown kind: {kind}")

    def var_count(self, kind: str) -> int:
        """Return the number of variables of the given kind defined in the current scope."""
        kind = 'var' if kind == 'local' else kind
        return self.index_counter.get(kind, 0)

    def kind_of(self, name: str):
        """Return the kind of the named identifier or None if not found."""
        if name in self.subroutine_table:
            return self.subroutine_table[name][1]
        if name in self.class_table:
            return self.class_table[name][1]
        return None

    def type_of(self, name: str):
        """Return the type of the named identifier."""
        if name in self.subroutine_table:
            return self.subroutine_table[name][0]
        if name in self.class_table:
            return self.class_table[name][0]
        return None

    def index_of(self, name: str):
        """Return the index assigned to the named identifier."""
        if name in self.subroutine_table:
            return self.subroutine_table[name][2]
        if name in self.class_table:
            return self.class_table[name][2]
        return None

class VMWriter:
    """Writes VM commands to the output stream."""
    def __init__(self, output_stream):
        self.out = output_stream

    def write_push(self, segment: str, index: int):
        self.out.write(f"push {segment} {index}\n")

    def write_pop(self, segment: str, index: int):
        self.out.write(f"pop {segment} {index}\n")

    def write_arithmetic(self, command: str):
        self.out.write(f"{command}\n")

    def write_label(self, label: str):
        self.out.write(f"label {label}\n")

    def write_goto(self, label: str):
        self.out.write(f"goto {label}\n")

    def write_if(self, label: str):
        self.out.write(f"if-goto {label}\n")

    def write_call(self, name: str, n_args: int):
        self.out.write(f"call {name} {n_args}\n")

    def write_function(self, name: str, n_locals: int):
        self.out.write(f"function {name} {n_locals}\n")

    def write_return(self):
        self.out.write("return\n")

class CompilationEngine:
    """Generates VM code from a stream of Jack tokens using the VMWriter and SymbolTable."""
    def __init__(self, tokenizer: JackTokenizer, vm_writer: VMWriter):
        self.tokenizer = tokenizer
        self.writer = vm_writer
        self.symbols = SymbolTable()
        self.class_name = ""       # current class name
        self.if_count = 0          # counters for unique label generation
        self.while_count = 0

    def compileClass(self):
        # class: 'class' className '{' classVarDec* subroutineDec* '}'
        token_type, token_val = self.tokenizer.advance()  # 'class'
        if token_val != 'class':
            raise SyntaxError("Expected 'class'")
        _, self.class_name = self.tokenizer.advance()     # className
        _, token_val = self.tokenizer.advance()           # '{'
        if token_val != '{':
            raise SyntaxError("Expected '{' after class name")
        # Compile class variable declarations
        nxt = self.tokenizer.peek()
        while nxt and nxt[1] in ('static', 'field'):
            self.compileClassVarDec()
            nxt = self.tokenizer.peek()
        # Compile subroutine declarations (methods, functions, constructors)
        nxt = self.tokenizer.peek()
        while nxt and nxt[1] in ('constructor', 'function', 'method'):
            self.compileSubroutine()
            nxt = self.tokenizer.peek()
        # Expect closing '}'
        _, token_val = self.tokenizer.advance()
        if token_val != '}':
            raise SyntaxError("Expected '}' at end of class")

    def compileClassVarDec(self):
        # classVarDec: ('static'|'field') type varName (',' varName)* ';'
        _, kind = self.tokenizer.advance()  # 'static' or 'field'
        _, type_name = self.tokenizer.advance()  # type (keyword or className)
        _, var_name = self.tokenizer.advance()   # first varName
        self.symbols.define(var_name, type_name, kind)  # define variable
        # Handle multiple declarations in one line
        nxt = self.tokenizer.peek()
        while nxt and nxt[1] == ',':
            self.tokenizer.advance()  # consume ','
            _, var_name = self.tokenizer.advance()  # next varName
            self.symbols.define(var_name, type_name, kind)
            nxt = self.tokenizer.peek()
        # consume terminating ';'
        _, semicolon = self.tokenizer.advance()
        if semicolon != ';':
            raise SyntaxError("Expected ';' after class variable declaration")

    def compileSubroutine(self):
        # subroutineDec: ('constructor'|'function'|'method') (void|type) subroutineName ( parameterList ) subroutineBody
        # Reset subroutine scope
        self.symbols.start_subroutine()
        self.if_count = 0
        self.while_count = 0
        # Keyword: constructor/function/method
        _, subroutine_kind = self.tokenizer.advance()
        _, return_type = self.tokenizer.advance()       # return type (could be 'void' or type, not used in codegen directly)
        _, subroutine_name = self.tokenizer.advance()   # subroutineName
        # If method, define 'this' as arg0 (so that argument indices start at 1 for user-defined arguments)
        if subroutine_kind == 'method':
            self.symbols.define('this', self.class_name, 'arg')
        # Parameter list
        _, token_val = self.tokenizer.advance()
        if token_val != '(':
            raise SyntaxError("Expected '(' after subroutine name")
        self.compileParameterList()
        _, token_val = self.tokenizer.advance()
        if token_val != ')':
            raise SyntaxError("Expected ')' after parameter list")
        # Subroutine body
        _, token_val = self.tokenizer.advance()
        if token_val != '{':
            raise SyntaxError("Expected '{' to begin subroutine body")
        # Local variable declarations
        nxt = self.tokenizer.peek()
        while nxt and nxt[1] == 'var':
            self.compileVarDec()
            nxt = self.tokenizer.peek()
        # Write function VM entry
        n_locals = self.symbols.var_count('var')
        self.writer.write_function(f"{self.class_name}.{subroutine_name}", n_locals)
        # Constructor: allocate memory for all fields and assign to this
        if subroutine_kind == 'constructor':
            n_fields = self.symbols.var_count('field')
            self.writer.write_push("constant", n_fields)
            self.writer.write_call("Memory.alloc", 1)
            self.writer.write_pop("pointer", 0)  # that base address becomes "this"
        # Method: set up this (arg0 is the object on which the method was invoked)
        if subroutine_kind == 'method':
            self.writer.write_push("argument", 0)
            self.writer.write_pop("pointer", 0)
        # Compile statements inside the subroutine body
        self.compileStatements()
        # Expect closing '}'
        _, token_val = self.tokenizer.advance()
        if token_val != '}':
            raise SyntaxError("Expected '}' to close subroutine body")

    def compileParameterList(self):
        # parameterList: ((type varName) (',' type varName)*)?
        nxt = self.tokenizer.peek()
        if nxt and nxt[1] != ')':  # if not an empty parameter list
            # First parameter
            _, type_name = self.tokenizer.advance()
            _, var_name = self.tokenizer.advance()
            self.symbols.define(var_name, type_name, 'arg')
            # Additional parameters
            nxt = self.tokenizer.peek()
            while nxt and nxt[1] == ',':
                self.tokenizer.advance()  # consume ','
                _, type_name = self.tokenizer.advance()
                _, var_name = self.tokenizer.advance()
                self.symbols.define(var_name, type_name, 'arg')
                nxt = self.tokenizer.peek()
        # (nothing to do if parameter list is empty)

    def compileVarDec(self):
        # varDec: 'var' type varName (',' varName)* ';'
        _, var_token = self.tokenizer.advance()
        if var_token != 'var':
            raise SyntaxError("Expected 'var' declaration")
        _, type_name = self.tokenizer.advance()
        _, var_name = self.tokenizer.advance()
        self.symbols.define(var_name, type_name, 'var')
        nxt = self.tokenizer.peek()
        # Handle multiple local vars in one declaration
        while nxt and nxt[1] == ',':
            self.tokenizer.advance()
            _, var_name = self.tokenizer.advance()
            self.symbols.define(var_name, type_name, 'var')
            nxt = self.tokenizer.peek()
        _, semicolon = self.tokenizer.advance()
        if semicolon != ';':
            raise SyntaxError("Expected ';' after var declaration")

    def compileStatements(self):
        # statements: statement*
        nxt = self.tokenizer.peek()
        while nxt:
            keyword = nxt[1]
            if keyword == 'let':
                self.compileLet()
            elif keyword == 'if':
                self.compileIf()
            elif keyword == 'while':
                self.compileWhile()
            elif keyword == 'do':
                self.compileDo()
            elif keyword == 'return':
                self.compileReturn()
            else:
                break  # not a statement keyword
            nxt = self.tokenizer.peek()

    def compileDo(self):
        # doStatement: 'do' subroutineCall ';'
        _, do_token = self.tokenizer.advance()
        if do_token != 'do':
            raise SyntaxError("Expected 'do'")
        self.compileSubroutineCall()       # compile the subroutine call
        self.writer.write_pop("temp", 0)   # dump the return value (if any) since it's not used
        _, semicolon = self.tokenizer.advance()
        if semicolon != ';':
            raise SyntaxError("Expected ';' after do statement")

    def compileLet(self):
        # letStatement: 'let' varName ('[' expression ']')? '=' expression ';'
        _, let_token = self.tokenizer.advance()
        if let_token != 'let':
            raise SyntaxError("Expected 'let'")
        _, var_name = self.tokenizer.advance()
        # Check if it's an array assignment (varName '[' expr ']')
        is_array = False
        nxt = self.tokenizer.peek()
        if nxt and nxt[1] == '[':
            is_array = True
            # Array assignment: compute address of var_name[index]
            self.tokenizer.advance()  # consume '['
            # Push base address of the array (the value of var_name)
            kind = self.symbols.kind_of(var_name)
            index = self.symbols.index_of(var_name)
            seg = {"static": "static", "field": "this", "arg": "argument", "var": "local"}[kind]
            self.writer.write_push(seg, index)
            self.compileExpression()              # compile index expression
            _, bracket = self.tokenizer.advance() # consume ']'
            if bracket != ']':
                raise SyntaxError("Expected ']' in array index")
            self.writer.write_arithmetic("add")   # base address + index
        # '='
        _, eq_token = self.tokenizer.advance()
        if eq_token != '=':
            raise SyntaxError("Expected '=' in let statement")
        # Compile the expression on the right side of '='
        self.compileExpression()
        _, semicolon = self.tokenizer.advance()
        if semicolon != ';':
            raise SyntaxError("Expected ';' after let statement")
        # If it was an array assignment, store the result in the computed address
        if is_array:
            # Now the stack has [ ... base+index, value ]
            self.writer.write_pop("temp", 0)      # pop the value to a temp
            self.writer.write_pop("pointer", 1)   # pop address into pointer1 (that segment)
            self.writer.write_push("temp", 0)     # push value again
            self.writer.write_pop("that", 0)      # store value into *that (i.e., arr[index])
        else:
            # Simple variable assignment: pop the expression result into the variable
            kind = self.symbols.kind_of(var_name)
            index = self.symbols.index_of(var_name)
            seg = {"static": "static", "field": "this", "arg": "argument", "var": "local"}[kind]
            self.writer.write_pop(seg, index)

    def compileWhile(self):
        # whileStatement: 'while' '(' expression ')' '{' statements '}'
        _, while_token = self.tokenizer.advance()
        if while_token != 'while':
            raise SyntaxError("Expected 'while'")
        # Generate unique labels for the while loop
        label_exp = f"WHILE_EXP{self.while_count}"
        label_end = f"WHILE_END{self.while_count}"
        self.while_count += 1
        # Label for loop start (to evaluate the expression each time)
        self.writer.write_label(label_exp)
        # '(' expression ')'
        _, lp = self.tokenizer.advance()
        if lp != '(':
            raise SyntaxError("Expected '(' after 'while'")
        self.compileExpression()  # condition
        _, rp = self.tokenizer.advance()
        if rp != ')':
            raise SyntaxError("Expected ')' after while condition")
        # If condition is false (0), jump to end label (break out of loop)
        self.writer.write_arithmetic("not")    # compute !condition
        self.writer.write_if(label_end)        # if-goto (if !condition) -> end
        # '{' statements '}'
        _, lb = self.tokenizer.advance()
        if lb != '{':
            raise SyntaxError("Expected '{' after while condition")
        self.compileStatements()
        _, rb = self.tokenizer.advance()
        if rb != '}':
            raise SyntaxError("Expected '}' after while body")
        # Jump back to start to re-evaluate condition
        self.writer.write_goto(label_exp)
        # Loop end label
        self.writer.write_label(label_end)

    def compileReturn(self):
        # returnStatement: 'return' expression? ';'
        _, ret_token = self.tokenizer.advance()
        if ret_token != 'return':
            raise SyntaxError("Expected 'return'")
        nxt = self.tokenizer.peek()
        if nxt and nxt[1] != ';':
            # Return expression present
            self.compileExpression()
            _, semicolon = self.tokenizer.advance()
            if semicolon != ';':
                raise SyntaxError("Expected ';' after return expression")
        else:
            # No expression after return (void return) – push 0 as placeholder
            _, semicolon = self.tokenizer.advance()
            if semicolon != ';':
                raise SyntaxError("Expected ';' after return")
            self.writer.write_push("constant", 0)
        self.writer.write_return()

    def compileIf(self):
        # ifStatement: 'if' '(' expression ')' '{' statements '}' ( 'else' '{' statements '}' )?
        _, if_token = self.tokenizer.advance()
        if if_token != 'if':
            raise SyntaxError("Expected 'if'")
        # Unique labels for this if/else
        label_true = f"IF_TRUE{self.if_count}"
        label_false = f"IF_FALSE{self.if_count}"
        label_end = f"IF_END{self.if_count}"
        self.if_count += 1
        # '(' expression ')'
        _, lp = self.tokenizer.advance()
        if lp != '(':
            raise SyntaxError("Expected '(' after 'if'")
        self.compileExpression()
        _, rp = self.tokenizer.advance()
        if rp != ')':
            raise SyntaxError("Expected ')' after if condition")
        # If condition is true (≠0), jump to IF_TRUE, else go to IF_FALSE
        self.writer.write_if(label_true)
        self.writer.write_goto(label_false)
        # If-true block
        self.writer.write_label(label_true)
        _, lb = self.tokenizer.advance()
        if lb != '{':
            raise SyntaxError("Expected '{' after if condition")
        self.compileStatements()
        _, rb = self.tokenizer.advance()
        if rb != '}':
            raise SyntaxError("Expected '}' after if block")
        # If an else block exists, jump to end after executing if-true block
        nxt = self.tokenizer.peek()
        if nxt and nxt[1] == 'else':
            self.writer.write_goto(label_end)
        # If-false label (destination for condition false)
        self.writer.write_label(label_false)
        if nxt and nxt[1] == 'else':
            # Compile else block
            self.tokenizer.advance()  # consume 'else'
            _, lb2 = self.tokenizer.advance()
            if lb2 != '{':
                raise SyntaxError("Expected '{' after 'else'")
            self.compileStatements()
            _, rb2 = self.tokenizer.advance()
            if rb2 != '}':
                raise SyntaxError("Expected '}' after else block")
            # End label after else
            self.writer.write_label(label_end)

    def compileExpression(self):
        # expression: term (op term)*
        self.compileTerm()
        # Map Jack operators to VM commands or OS calls
        op_map = {
            '+' : 'add',
            '-' : 'sub',
            '*' : 'Math.multiply',
            '/' : 'Math.divide',
            '&' : 'and',
            '|' : 'or',
            '<' : 'lt',
            '>' : 'gt',
            '=' : 'eq'
        }
        nxt = self.tokenizer.peek()
        while nxt and nxt[1] in op_map:
            _, op = self.tokenizer.advance()      # consume the operator
            self.compileTerm()                   # compile the term after the op
            # Generate the appropriate VM command for the operator
            if op == '*':
                self.writer.write_call("Math.multiply", 2)   # multiply function call
            elif op == '/':
                self.writer.write_call("Math.divide", 2)     # divide function call
            else:
                self.writer.write_arithmetic(op_map[op])
            nxt = self.tokenizer.peek()

    def compileTerm(self):
        # term: integerConstant | stringConstant | keywordConstant | varName | varName '[' expr ']' 
        #     | subroutineCall | '(' expr ')' | unaryOp term
        token = self.tokenizer.peek()
        if not token:
            raise SyntaxError("Unexpected end of expression")
        ttype, tval = token
        # Integer constant
        if ttype == 'integerConstant':
            self.tokenizer.advance()
            self.writer.write_push("constant", tval)
        # String constant – use String.new(len) and appendChar for each character
        elif ttype == 'stringConstant':
            self.tokenizer.advance()
            s = tval
            self.writer.write_push("constant", len(s))
            self.writer.write_call("String.new", 1)
            for ch in s:
                self.writer.write_push("constant", ord(ch))
                self.writer.write_call("String.appendChar", 2)
        # Keyword constant (true, false, null, this)
        elif ttype == 'keyword':
            self.tokenizer.advance()
            if tval == 'true':
                # true = -1 in Hack (111...); push 0 and negate
                self.writer.write_push("constant", 0)
                self.writer.write_arithmetic("not")
            elif tval in ('false', 'null'):
                # false and null are 0
                self.writer.write_push("constant", 0)
            elif tval == 'this':
                # push current object base address
                self.writer.write_push("pointer", 0)
        # Parenthesized expression: '(' expression ')'
        elif ttype == 'symbol' and tval == '(':
            self.tokenizer.advance()  # consume '('
            self.compileExpression()
            _, closing = self.tokenizer.advance()  # consume ')'
            if closing != ')':
                raise SyntaxError("Expected ')' after expression")
        # Unary operation: - or ~
        elif ttype == 'symbol' and tval in ('-', '~'):
            op = tval
            self.tokenizer.advance()
            self.compileTerm()  # compile the term after unary operator
            if op == '-':
                self.writer.write_arithmetic("neg")
            else:  # '~'
                self.writer.write_arithmetic("not")
        # Identifier: could be varName, array access, or subroutine call
        elif ttype == 'identifier':
            name = tval  # the identifier (variable or subroutine/class name)
            self.tokenizer.advance()  # consume the identifier
            nxt = self.tokenizer.peek()
            # Array access: varName '[' expression ']'
            if nxt and nxt[1] == '[':
                self.tokenizer.advance()  # consume '['
                # Push array base address
                kind = self.symbols.kind_of(name)
                index = self.symbols.index_of(name)
                seg = {"static": "static", "field": "this", "arg": "argument", "var": "local"}[kind]
                self.writer.write_push(seg, index)
                self.compileExpression()
                _, rb = self.tokenizer.advance()  # consume ']'
                if rb != ']':
                    raise SyntaxError("Expected ']' in array term")
                self.writer.write_arithmetic("add")
                # result is base+index, set THAT to this address and push the value at that address
                self.writer.write_pop("pointer", 1)  # THAT = base+index
                self.writer.write_push("that", 0)    # push *THAT (the array element)
            # Subroutine call: could be Name(...) or Name.method(...), where Name might be class or var
            elif nxt and nxt[1] in ('(', '.'):
                self.compileSubroutineCall(name)
            # Simple varName
            else:
                kind = self.symbols.kind_of(name)
                index = self.symbols.index_of(name)
                seg = {"static": "static", "field": "this", "arg": "argument", "var": "local"}[kind]
                self.writer.write_push(seg, index)
        else:
            raise SyntaxError(f"Unexpected token in term: {token}")

    def compileExpressionList(self) -> int:
        # expressionList: (expression (',' expression)*)?
        count = 0
        nxt = self.tokenizer.peek()
        if nxt and nxt[1] != ')':
            # At least one expression
            self.compileExpression()
            count += 1
            nxt = self.tokenizer.peek()
            while nxt and nxt[1] == ',':
                self.tokenizer.advance()  # consume ','
                self.compileExpression()
                count += 1
                nxt = self.tokenizer.peek()
        return count

    def compileSubroutineCall(self, name=None):
        # subroutineCall: (className|varName|subroutineName) ( '.' subroutineName )? '(' expressionList ')'
        # This method is called either with `name` already read (the identifier before a '(' or '.'),
        # or with name=None to read the first identifier inside this method.
        if name is None:
            _, name = self.tokenizer.advance()  # read the identifier (class/var or subroutine name)
        n_args = 0
        nxt = self.tokenizer.peek()
        if nxt and nxt[1] == '.':
            # Method call on an object or function in another class
            self.tokenizer.advance()            # consume '.'
            _, sub_name = self.tokenizer.advance()  # the second part of name (subroutine name)
            kind = self.symbols.kind_of(name)
            if kind:
                # 'name' is a variable (object) on which a method is called
                type_name = self.symbols.type_of(name)   # class of the object
                index = self.symbols.index_of(name)
                seg = {"static": "static", "field": "this", "arg": "argument", "var": "local"}[kind]
                self.writer.write_push(seg, index)      # push object reference as arg0
                n_args = 1
                call_name = f"{type_name}.{sub_name}"
            else:
                # 'name' is a class name (calling a function/constructor)
                call_name = f"{name}.{sub_name}"
            # '('
            _, lp = self.tokenizer.advance()
            if lp != '(':
                raise SyntaxError("Expected '(' after subroutine name")
        else:
            # Subroutine call with no explicit class/var (implies current class method call)
            call_name = f"{self.class_name}.{name}"
            # push 'this' as arg0
            self.writer.write_push("pointer", 0)
            n_args = 1
            _, lp = self.tokenizer.advance()
            if lp != '(':
                raise SyntaxError("Expected '(' after subroutine name")
        # Compile arguments and call
        num_exprs = self.compileExpressionList()
        _, rp = self.tokenizer.advance()
        if rp != ')':
            raise SyntaxError("Expected ')' after subroutine call")
        self.writer.write_call(call_name, n_args + num_exprs)

# Main program entry: compile Jack files to VM
if __name__ == "__main__":
    import os
    import glob
    # Determine input path (file or directory)
    if len(sys.argv) != 2:
        sys.exit("Usage: JackCompiler.py <input.jack or directory>")
    path = sys.argv[1]
    files = []
    if os.path.isdir(path):
        files = glob.glob(os.path.join(path, "*.jack"))
    elif path.endswith(".jack"):
        files = [path]
    else:
        sys.exit("Input must be a .jack file or a directory containing .jack files.")
    for jack_file in files:
        vm_file = jack_file.replace(".jack", ".vm")
        with open(jack_file, 'r') as infile, open(vm_file, 'w') as outfile:
            tokenizer = JackTokenizer(infile)
            writer = VMWriter(outfile)
            compiler = CompilationEngine(tokenizer, writer)
            compiler.compileClass()
