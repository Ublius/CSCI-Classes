class SymbolTable:
    NONE = 0
    STATIC = "static"
    FIELD = "this"
    ARG = "argument"
    VAR = "local"

    def __init__(self):
        self.classTable = {}
        self.subTable = {}

        self.classCounts = {self.STATIC: 0, self.FIELD: 0}
        self.subCounts = {self.ARG: 0, self.VAR: 0}

    def startSubroutine(self):
        self.subTable.clear()
        self.subCounts = {self.ARG: 0, self.VAR: 0}

    def _scope(self, kind):
        if kind in (self.STATIC, self.FIELD):
            return self.classTable, self.classCounts
        return self.subTable, self.subCounts

    def define(self, name, identifierType, identifierKind):
        table, counts = self._scope(identifierKind)
        index = counts[identifierKind]

        table[name] = {
            "kind": identifierKind,
            "type": identifierType,
            "index": index
        }

        counts[identifierKind] += 1

    def kindOf(self, name):
        return self.fieldOf(name, "kind")

    def typeOf(self, name):
        return self.fieldOf(name, "type")

    def indexOf(self, name):
        return self.fieldOf(name, "index")

    def fieldOf(self, name, field):
        if name in self.subTable:
            return self.subTable[name][field]
        if name in self.classTable:
            return self.classTable[name][field]
        raise KeyError(f"{name} not found in symbol tables")

    def isDefined(self, name):
        return name in self.subTable or name in self.classTable

    def stringKindOf(self, name):
        kind_map = {
            self.STATIC: "static",
            self.FIELD: "field",
            self.ARG: "arg",
            self.VAR: "var"
        }
        return kind_map.get(self.kindOf(name), "none")
    
    def resolve(self, name):
        if name in self.subTable:
            return self.subTable[name]
        if name in self.classTable:
            return self.classTable[name]
        raise KeyError(f"{name} not found in symbol tables")