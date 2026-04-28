class SymbolTable:
    def __init__(self):
        self.class_table = {}
        self.subroutine_table = {}
        self.index_counter = {'static': 0, 'field': 0, 'arg': 0, 'var': 0}

    def start_subroutine(self):
        self.subroutine_table.clear()
        self.index_counter['arg'] = 0
        self.index_counter['var'] = 0

    def define(self, name, type_name, kind):
        kind = 'var' if kind == 'local' else kind

        if kind in ('static', 'field'):
            idx = self.index_counter[kind]
            self.class_table[name] = (type_name, kind, idx)
            self.index_counter[kind] += 1

        elif kind in ('arg', 'var'):
            idx = self.index_counter[kind]
            self.subroutine_table[name] = (type_name, kind, idx)
            self.index_counter[kind] += 1
        else:
            raise ValueError("Unknown kind")

    def var_count(self, kind):
        kind = 'var' if kind == 'local' else kind
        return self.index_counter.get(kind, 0)

    def kind_of(self, name):
        if name in self.subroutine_table:
            return self.subroutine_table[name][1]
        if name in self.class_table:
            return self.class_table[name][1]
        return None

    def type_of(self, name):
        if name in self.subroutine_table:
            return self.subroutine_table[name][0]
        if name in self.class_table:
            return self.class_table[name][0]
        return None

    def index_of(self, name):
        if name in self.subroutine_table:
            return self.subroutine_table[name][2]
        if name in self.class_table:
            return self.class_table[name][2]
        return None