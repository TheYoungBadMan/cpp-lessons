# Lexical grammar

<`id`> ::= <`alpha`> <`alphanum`>*; <br />
<`num`> ::= <`integer`> ('.' <`integer`>?)?; <br />
<`integer`> ::= <`digit`>+; <br />
<`op`> ::= '+' | '-' | '*' | '/' | ',' | '(' | ')'; <br />
<`digit`> ::= '\d'; <br />
<`alpha`> ::= '\w' | '_'; <br />
<`alphanum`> ::= <`alpha`> | <`digit`>; <br />


# Syntax grammar

<`expr`> ::= <`term`> (<`sum_op`>  <`term`>)*; <br />
<`term`> ::= <`factor`> (<`mul_op`> <`factor`>)*; <br />
<`factor`> ::= <`unary`> (<`pow_op`> <`factor`>)?; <br />
<`unary`> ::= <`unary_op`>* <`primary`>; <br />
<`primary`> ::= <`num`> | <`var`> | <`group`> | <`func`>; <br />
<`var`> ::= <`id`>; <br />
<`group`> ::= '(' <`expr`> ')'; <br />
<`func`> ::= <`id`> '(' <`arglist`>? ')'; <br />
<`arglist`> ::= <`expr`> (',' <`expr`>)*; <br />
<`sum_op`> ::= '+' | '-'; <br />
<`mul_op`> ::= '*' | '/'; <br />
<`pow_op`> ::= '^'; <br />
<`unary_op`> ::= '+' | '-'; <br />