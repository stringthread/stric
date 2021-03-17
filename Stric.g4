grammar Stric;

main
  :(elements+=block EOL?)* EOF
  ;
WHITESPACE
  :' '+ -> skip
  ;
EOL
  :'\r'? '\n'
  ;
fragment DIGIT
  :[0-9]
  ;
fragment NON_ZERO_DIGIT
  :[1-9]
  ;
fragment S_ALPHA
  :[a-z]
  ;
fragment C_ALPHA
  :[A-Z]
  ;
fragment IDENTIFIER_START_CHAR
  :S_ALPHA
  |C_ALPHA
  |'_'
  ;
fragment IDENTIFIER_CHAR
  :DIGIT
  |S_ALPHA
  |C_ALPHA
  |'_'
  ;
EOS
  :';'
  ;
IDENTIFIER
  :IDENTIFIER_START_CHAR IDENTIFIER_CHAR*
  ;
value_bracket_start
  :'('
  ;
value_bracket_end
  :')'
  ;
call_bracket_start
  :'('
  ;
call_bracket_end
  :')'
  ;
block_bracket_start
  :'{'
  ;
block_bracket_end
  :'}'
  ;
ASSIGN
  :'='
  ;
NULL_
  :'null'
  ;
INT
  :'-'? NON_ZERO_DIGIT DIGIT+
  ;
FLOAT
  :'-'? DIGIT+ ('.' DIGIT+)?
  ;
PLUS
  :'+'
  ;
MINUS
  :'-'
  ;
MULT
  :'*'
  ;
DIV
  :'/'
  ;
DIV_INT
  :'//'
  ;
BIT_AND
  :'&'
  ;
BIT_OR
  :'|'
  ;
BIT_EXCLUSIVE_OR
  :'^'
  ;
TYPE_INT
  :'int'
  ;
TYPE_FLOAT
  :'float'
  ;
TRUE
  :'@t'
  ;
FALSE
  :'@f'
  ;
NOT
  :'!'
  |'not '
  ;
BOOL_AND
  :'&&'
  ;
BOOL_OR
  :'||'
  ;
EQUAL
  :'=='
  ;
NOT_EQUAL
  :'!='
  ;
LESS
  :'<'
  ;
GREATER
  :'>'
  ;
LESS_EQUAL
  :'<='
  ;
GREATER_EQUAL
  :'>='
  ;
TYPE_BOOL
  :'bool'
  ;
IF
  :'if'
  ;
ELSE
  :'else'
  ;
WHILE
  :'while'
  ;

unary_operator
  :MINUS
  |NOT
  ;
binary_operator
  :PLUS
  |MINUS
  |MULT
  |DIV
  |DIV_INT
  |BIT_AND
  |BIT_OR
  |BIT_EXCLUSIVE_OR
  |BOOL_AND
  |BOOL_OR
  |EQUAL
  |NOT_EQUAL
  |LESS
  |GREATER
  |LESS_EQUAL
  |GREATER_EQUAL
  ;
literal
  :NULL_
  |INT
  |FLOAT
  |TRUE
  |FALSE
  ;
value_expr
  :value_bracket_start value_expr value_bracket_end
  |unary_operator value_expr
  |value_expr binary_operator value_expr
  |literal
  ;
statement
  :value_expr EOS
  ;
block
  :statement
  ;
