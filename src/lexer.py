"""
Cook Programming Language - Lexer
--------------------------------
The lexer is responsible for breaking down the source code into tokens.
"""

class Token:
    def __init__(self, token_type, value, line, column):
        self.type = token_type
        self.value = value
        self.line = line
        self.column = column
    
    def __repr__(self):
        return f"Token({self.type}, '{self.value}', line={self.line}, col={self.column})"

class Lexer:
    def __init__(self, source_code):
        self.source_code = source_code
        self.position = 0
        self.line = 1
        self.column = 1
        self.current_char = self.source_code[0] if self.source_code else None
    
    def advance(self):
        """Move to the next character in the source code."""
        self.position += 1
        if self.position >= len(self.source_code):
            self.current_char = None
        else:
            self.current_char = self.source_code[self.position]
            if self.current_char == '\n':
                self.line += 1
                self.column = 1
            else:
                self.column += 1
    
    def tokenize(self):
        """Convert the source code into a list of tokens."""
        tokens = []
        
        # This is a placeholder for the actual tokenization logic
        # We'll implement this as we define the language syntax
        
        return tokens
