

from typing import List, Tuple

import re

TOKEN_TYPES: List[Tuple[str, re.Pattern]] = [
    ('number', re.compile(r'\d+')),
    ('string', re.compile(r'"[^"]+"')),
    ('keyword', re.compile(r'function|if|NULL|return')),
    ('identifier', re.compile(r'[A-Za-z]\w*')),
    ('operator', re.compile(r'<=|=|!=')),
    ('scope', re.compile(r'(:?!|#)')),
    ('operator', re.compile(r'=|-|\+|\.|\*|/')),
    ('sep', re.compile(r'\(|\)|\{|\}|:|,')),
    ('semicolon', re.compile(r';')),
    ('whitespace', re.compile(r'\s+'))
]

COMMENT_REMOVAL = re.compile(r'//.+\n|/\*.+\*/', flags=re.MULTILINE|re.DOTALL)

class Token:
    def __init__(self, type: str, txt: str, line: int) -> None:
        self.type = type
        self.txt = txt
        self.line = line
    def __str__(self) -> str:
        return f"({self.line}) {self.type.ljust(10)} -> {self.txt}"
    def __repr__(self) -> str:
        return str(self)

# Tokenize the input stream
class Tokenizer:
    def __init__(self, input: str) -> None:
        self.tokens: List[Token] = []

        input = COMMENT_REMOVAL.sub(lambda m: '\n' * m.group(0).count('\n'), input)

        for i,line in enumerate(input.splitlines()):
            
            start_pos = 0
            while start_pos < len(line):
                # Scan a single line for tokens
                best_match_len = 0
                match_type = ''
                for type, regex in TOKEN_TYPES:
                    match = regex.match(line, start_pos)
                    if match is not None:
                        s,e = match.span()
                        _len = e - s
                        if _len > best_match_len:
                            best_match_len = _len
                            match_type = type
                
                # print(line, start_pos, best_match_len)

                if best_match_len == 0:
                    raise ValueError(f"Unknown character at line {i} position {start_pos} ({line[start_pos]})")
                elif match_type != 'whitespace':
                    self.tokens.append(Token(match_type, line[start_pos:start_pos+best_match_len], i))
                
                start_pos += best_match_len
        
        self.tokens.append(Token('eof', '', i))

class ParseTreeNode:
    def __init__(self, token) -> None:
        self.token = token
        self.children: List[ParseTreeNode] = []

class UnexpectedToken(ValueError):
    def __init__(self, t: Token) -> None:
        super().__init__(f'Unexpected token line {t.line}, {t.txt}')

class RecDescParser:
    def __init__(self) -> None:
        self.token_stream: List[Token] = []
        self.token_id = 0

        self.token = self.token_stream[self.token_id]
    def gettoken(self) -> None:
        self.token_id += 1;
        self.token = self.token_stream[self.token_id]
    def parse_BLOCK(self):
        if self.token.type != 'keyword':
            raise UnexpectedToken(self.token)
        if self.token.txt == 'function':
            self.gettoken()
            self.parse_FUNCTION()
        elif self.token.txt == 'method':
            self.gettoken()
            self.parse_METHOD()
        else:
            raise UnexpectedToken(self.token)
        

if __name__ == "__main__":

    with open('test/calculator/commands/1.txt') as f:
        tokens = Tokenizer(f.read())

        print('\n'.join(str(t) for t in tokens.tokens))
