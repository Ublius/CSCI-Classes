from JackTokenizer import JackTokenizer
from VMWriter import VMWriter
from SymbolTable import SymbolTable

class CompilationEngine:
    def __init__(self, tokenizer: JackTokenizer, vm_writer: VMWriter):
        self.tokenizer = tokenizer
        self.writer = vm_writer
        self.symbols = SymbolTable()
        self.class_name = ""
        self.if_count = 0
        self.while_count = 0

    def compileClass(self):
        token_type, token_val = self.tokenizer.advance()
        if token_val != 'class':
            raise SyntaxError("Expected 'class'")
        _, self.class_name = self.tokenizer.advance()
        _, token_val = self.tokenizer.advance()
        if token_val != '{':
            raise SyntaxError("Expected '{' after class name")
        nxt = self.tokenizer.peek()
        while nxt and nxt[1] in ('static', 'field'):
            self.compileClassVarDec()
            nxt = self.tokenizer.peek()
        nxt = self.tokenizer.peek()
        while nxt and nxt[1] in ('constructor', 'function', 'method'):
            self.compileSubroutine()
            nxt = self.tokenizer.peek()
        _, token_val = self.tokenizer.advance()
        if token_val != '}':
            raise SyntaxError("Expected '}' at end of class")

    def compileClassVarDec(self):
        _, kind = self.tokenizer.advance()
        _, type_name = self.tokenizer.advance()
        _, var_name = self.tokenizer.advance()
        self.symbols.define(var_name, type_name, kind)
        nxt = self.tokenizer.peek()
        while nxt and nxt[1] == ',':
            self.tokenizer.advance()
            _, var_name = self.tokenizer.advance()
            self.symbols.define(var_name, type_name, kind)
            nxt = self.tokenizer.peek()
        _, semicolon = self.tokenizer.advance()
        if semicolon != ';':
            raise SyntaxError("Expected ';' after class variable declaration")

    def compileSubroutine(self):
        self.symbols.start_subroutine()
        self.if_count = 0
        self.while_count = 0
        _, subroutine_kind = self.tokenizer.advance()
        _, return_type = self.tokenizer.advance()
        _, subroutine_name = self.tokenizer.advance()

        if subroutine_kind == 'method':
            self.symbols.define('this', self.class_name, 'arg')

        _, token_val = self.tokenizer.advance()
        if token_val != '(':
            raise SyntaxError("Expected '(' after subroutine name")
        self.compileParameterList()
        _, token_val = self.tokenizer.advance()
        if token_val != ')':
            raise SyntaxError("Expected ')' after parameter list")

        _, token_val = self.tokenizer.advance()
        if token_val != '{':
            raise SyntaxError("Expected '{' to begin subroutine body")

        nxt = self.tokenizer.peek()
        while nxt and nxt[1] == 'var':
            self.compileVarDec()
            nxt = self.tokenizer.peek()

        n_locals = self.symbols.var_count('var')
        self.writer.write_function(f"{self.class_name}.{subroutine_name}", n_locals)

        if subroutine_kind == 'constructor':
            n_fields = self.symbols.var_count('field')
            self.writer.write_push("constant", n_fields)
            self.writer.write_call("Memory.alloc", 1)
            self.writer.write_pop("pointer", 0)

        if subroutine_kind == 'method':
            self.writer.write_push("argument", 0)
            self.writer.write_pop("pointer", 0)

        self.compileStatements()

        _, token_val = self.tokenizer.advance()
        if token_val != '}':
            raise SyntaxError("Expected '}' to close subroutine body")

    def compileParameterList(self):
        nxt = self.tokenizer.peek()
        if nxt and nxt[1] != ')':
            _, type_name = self.tokenizer.advance()
            _, var_name = self.tokenizer.advance()
            self.symbols.define(var_name, type_name, 'arg')
            nxt = self.tokenizer.peek()
            while nxt and nxt[1] == ',':
                self.tokenizer.advance()
                _, type_name = self.tokenizer.advance()
                _, var_name = self.tokenizer.advance()
                self.symbols.define(var_name, type_name, 'arg')
                nxt = self.tokenizer.peek()

    def compileVarDec(self):
        _, var_token = self.tokenizer.advance()
        if var_token != 'var':
            raise SyntaxError("Expected 'var' declaration")
        _, type_name = self.tokenizer.advance()
        _, var_name = self.tokenizer.advance()
        self.symbols.define(var_name, type_name, 'var')
        nxt = self.tokenizer.peek()
        while nxt and nxt[1] == ',':
            self.tokenizer.advance()
            _, var_name = self.tokenizer.advance()
            self.symbols.define(var_name, type_name, 'var')
            nxt = self.tokenizer.peek()
        _, semicolon = self.tokenizer.advance()
        if semicolon != ';':
            raise SyntaxError("Expected ';' after var declaration")

    def compileStatements(self):
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
                break
            nxt = self.tokenizer.peek()

    def compileDo(self):
        _, do_token = self.tokenizer.advance()
        if do_token != 'do':
            raise SyntaxError("Expected 'do'")
        self.compileSubroutineCall()
        self.writer.write_pop("temp", 0)
        _, semicolon = self.tokenizer.advance()
        if semicolon != ';':
            raise SyntaxError("Expected ';' after do statement")

    def compileLet(self):
        _, let_token = self.tokenizer.advance()
        if let_token != 'let':
            raise SyntaxError("Expected 'let'")
        _, var_name = self.tokenizer.advance()

        is_array = False
        nxt = self.tokenizer.peek()

        if nxt and nxt[1] == '[':
            is_array = True
            self.tokenizer.advance()
            kind = self.symbols.kind_of(var_name)
            index = self.symbols.index_of(var_name)
            seg = {"static": "static", "field": "this", "arg": "argument", "var": "local"}[kind]
            self.writer.write_push(seg, index)
            self.compileExpression()
            _, bracket = self.tokenizer.advance()
            if bracket != ']':
                raise SyntaxError("Expected ']' in array index")
            self.writer.write_arithmetic("add")

        _, eq_token = self.tokenizer.advance()
        if eq_token != '=':
            raise SyntaxError("Expected '=' in let statement")

        self.compileExpression()

        _, semicolon = self.tokenizer.advance()
        if semicolon != ';':
            raise SyntaxError("Expected ';' after let statement")

        if is_array:
            self.writer.write_pop("temp", 0)
            self.writer.write_pop("pointer", 1)
            self.writer.write_push("temp", 0)
            self.writer.write_pop("that", 0)
        else:
            kind = self.symbols.kind_of(var_name)
            index = self.symbols.index_of(var_name)
            seg = {"static": "static", "field": "this", "arg": "argument", "var": "local"}[kind]
            self.writer.write_pop(seg, index)

    def compileWhile(self):
        _, while_token = self.tokenizer.advance()
        if while_token != 'while':
            raise SyntaxError("Expected 'while'")

        label_exp = f"WHILE_EXP{self.while_count}"
        label_end = f"WHILE_END{self.while_count}"
        self.while_count += 1

        self.writer.write_label(label_exp)

        _, lp = self.tokenizer.advance()
        if lp != '(':
            raise SyntaxError("Expected '(' after 'while'")

        self.compileExpression()

        _, rp = self.tokenizer.advance()
        if rp != ')':
            raise SyntaxError("Expected ')' after while condition")

        self.writer.write_arithmetic("not")
        self.writer.write_if(label_end)

        _, lb = self.tokenizer.advance()
        if lb != '{':
            raise SyntaxError("Expected '{' after while condition")

        self.compileStatements()

        _, rb = self.tokenizer.advance()
        if rb != '}':
            raise SyntaxError("Expected '}' after while body")

        self.writer.write_goto(label_exp)
        self.writer.write_label(label_end)

    def compileReturn(self):
        _, ret_token = self.tokenizer.advance()
        if ret_token != 'return':
            raise SyntaxError("Expected 'return'")

        nxt = self.tokenizer.peek()
        if nxt and nxt[1] != ';':
            self.compileExpression()
            _, semicolon = self.tokenizer.advance()
            if semicolon != ';':
                raise SyntaxError("Expected ';'")
        else:
            _, semicolon = self.tokenizer.advance()
            if semicolon != ';':
                raise SyntaxError("Expected ';'")
            self.writer.write_push("constant", 0)

        self.writer.write_return()

    def compileIf(self):
        _, if_token = self.tokenizer.advance()
        if if_token != 'if':
            raise SyntaxError("Expected 'if'")

        label_true = f"IF_TRUE{self.if_count}"
        label_false = f"IF_FALSE{self.if_count}"
        label_end = f"IF_END{self.if_count}"
        self.if_count += 1

        _, lp = self.tokenizer.advance()
        if lp != '(':
            raise SyntaxError("Expected '(' after 'if'")

        self.compileExpression()

        _, rp = self.tokenizer.advance()
        if rp != ')':
            raise SyntaxError("Expected ')'")

        self.writer.write_if(label_true)
        self.writer.write_goto(label_false)

        self.writer.write_label(label_true)

        _, lb = self.tokenizer.advance()
        if lb != '{':
            raise SyntaxError("Expected '{'")

        self.compileStatements()

        _, rb = self.tokenizer.advance()
        if rb != '}':
            raise SyntaxError("Expected '}'")

        nxt = self.tokenizer.peek()

        if nxt and nxt[1] == 'else':
            self.writer.write_goto(label_end)

        self.writer.write_label(label_false)

        if nxt and nxt[1] == 'else':
            self.tokenizer.advance()
            _, lb2 = self.tokenizer.advance()
            self.compileStatements()
            _, rb2 = self.tokenizer.advance()
            self.writer.write_label(label_end)

    def compileExpression(self):
        self.compileTerm()

        op_map = {
            '+': 'add',
            '-': 'sub',
            '*': 'Math.multiply',
            '/': 'Math.divide',
            '&': 'and',
            '|': 'or',
            '<': 'lt',
            '>': 'gt',
            '=': 'eq'
        }

        nxt = self.tokenizer.peek()
        while nxt and nxt[1] in op_map:
            _, op = self.tokenizer.advance()
            self.compileTerm()
            if op == '*':
                self.writer.write_call("Math.multiply", 2)
            elif op == '/':
                self.writer.write_call("Math.divide", 2)
            else:
                self.writer.write_arithmetic(op_map[op])
            nxt = self.tokenizer.peek()

    def compileTerm(self):
        token = self.tokenizer.peek()
        if not token:
            raise SyntaxError("Unexpected end")

        ttype, tval = token

        if ttype == 'integerConstant':
            self.tokenizer.advance()
            self.writer.write_push("constant", tval)

        elif ttype == 'stringConstant':
            self.tokenizer.advance()
            self.writer.write_push("constant", len(tval))
            self.writer.write_call("String.new", 1)
            for ch in tval:
                self.writer.write_push("constant", ord(ch))
                self.writer.write_call("String.appendChar", 2)

        elif ttype == 'keyword':
            self.tokenizer.advance()
            if tval == 'true':
                self.writer.write_push("constant", 0)
                self.writer.write_arithmetic("not")
            elif tval in ('false', 'null'):
                self.writer.write_push("constant", 0)
            elif tval == 'this':
                self.writer.write_push("pointer", 0)

        elif ttype == 'symbol' and tval == '(':
            self.tokenizer.advance()
            self.compileExpression()
            self.tokenizer.advance()

        elif ttype == 'symbol' and tval in ('-', '~'):
            op = tval
            self.tokenizer.advance()
            self.compileTerm()
            self.writer.write_arithmetic("neg" if op == '-' else "not")

        elif ttype == 'identifier':
            name = tval
            self.tokenizer.advance()
            nxt = self.tokenizer.peek()

            if nxt and nxt[1] == '[':
                self.tokenizer.advance()
                kind = self.symbols.kind_of(name)
                index = self.symbols.index_of(name)
                seg = {"static": "static", "field": "this", "arg": "argument", "var": "local"}[kind]
                self.writer.write_push(seg, index)
                self.compileExpression()
                self.tokenizer.advance()
                self.writer.write_arithmetic("add")
                self.writer.write_pop("pointer", 1)
                self.writer.write_push("that", 0)

            elif nxt and nxt[1] in ('(', '.'):
                self.compileSubroutineCall(name)

            else:
                kind = self.symbols.kind_of(name)
                index = self.symbols.index_of(name)
                seg = {"static": "static", "field": "this", "arg": "argument", "var": "local"}[kind]
                self.writer.write_push(seg, index)

    def compileExpressionList(self):
        count = 0
        nxt = self.tokenizer.peek()
        if nxt and nxt[1] != ')':
            self.compileExpression()
            count += 1
            nxt = self.tokenizer.peek()
            while nxt and nxt[1] == ',':
                self.tokenizer.advance()
                self.compileExpression()
                count += 1
                nxt = self.tokenizer.peek()
        return count

    def compileSubroutineCall(self, name=None):
        if name is None:
            _, name = self.tokenizer.advance()

        n_args = 0
        nxt = self.tokenizer.peek()

        if nxt and nxt[1] == '.':
            self.tokenizer.advance()
            _, sub_name = self.tokenizer.advance()

            kind = self.symbols.kind_of(name)
            if kind:
                type_name = self.symbols.type_of(name)
                index = self.symbols.index_of(name)
                seg = {"static": "static", "field": "this", "arg": "argument", "var": "local"}[kind]
                self.writer.write_push(seg, index)
                n_args = 1
                call_name = f"{type_name}.{sub_name}"
            else:
                call_name = f"{name}.{sub_name}"

            self.tokenizer.advance()

        else:
            call_name = f"{self.class_name}.{name}"
            self.writer.write_push("pointer", 0)
            n_args = 1
            self.tokenizer.advance()

        num_exprs = self.compileExpressionList()
        self.tokenizer.advance()
        self.writer.write_call(call_name, n_args + num_exprs)