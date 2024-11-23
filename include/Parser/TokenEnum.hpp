
#ifndef TOKEN
#define TOKEN(name) name,
#endif

#ifndef MAP
#define MAP(name, value) TOKEN(name)
#endif

#ifdef MAPPING
#define ALL
#undef TOKEN
#undef MAP
#define TOKEN(name)
#define MAP(name, value) {value, Token::Type::name},
#endif

#ifdef ALL
#define KEYWORDS
#define LITERALS
#define PUNCTUATION
#define OPERATORS
#define START_END
#endif

#ifdef KEYWORDS

#ifdef START_END
TOKEN(KEYWORDS_START)
#endif

MAP(SELECT, "select")
MAP(INSERT, "insert")
MAP(UPDATE, "update")
MAP(DELETE, "delete")
MAP(CREATE, "create")
MAP(DROP, "drop")
MAP(TABLE, "table")
MAP(UNIQUE, "unique")
MAP(AUTOINCREMENT, "autoincrement")
MAP(KEY, "key")
MAP(NOT_NULL, "not_null")
MAP(INT, "int32")
MAP(BOOL, "bool")
MAP(STRING, "string")
MAP(BYTES, "bytes")
MAP(WHERE, "where")
MAP(FROM, "from")
MAP(IS, "is")

#ifdef START_END
TOKEN(KEYWORDS_END)
#endif

#endif


#ifdef OPERATORS

#ifdef START_END
TOKEN(OPERATORS_START)
#endif

MAP(PLUS, "+")
MAP(MINUS, "-")
MAP(MUL, "*")
MAP(DIV, "/")
MAP(MOD, "%")

MAP(EQUAL, "=")
MAP(NOT_EQUAL, "!=")
MAP(LESS, "<")
MAP(LESS_EQUAL, "<=")
MAP(GREATER, ">")
MAP(GREATER_EQUAL, ">=")

MAP(AND, "&&")
MAP(OR, "||")
MAP(XOR, "^^")
MAP(NOT, "!")

#ifdef START_END
TOKEN(OPERATORS_END)
#endif


#endif


#ifdef PUNCTUATION

#ifdef START_END
TOKEN(PUNCTUATION_START)
#endif

MAP(COMMA, ",")
MAP(DOT, ".")
MAP(COLON, ":")
MAP(SEMICOLON, ";")
MAP(LEFT_PARENTHESIS, "(")
MAP(RIGHT_PARENTHESIS, ")")
MAP(LEFT_BRACKET, "[")
MAP(RIGHT_BRACKET, "]")
MAP(LEFT_BRACE, "{")
MAP(RIGHT_BRACE, "}")
MAP(VLINE, "|")

#ifdef START_END
TOKEN(PUNCTUATION_END)
#endif

#endif


#ifdef LITERALS

#ifdef START_END
TOKEN(LITERALS_START)
#endif

TOKEN(END_OF_FILE)
TOKEN(IDENTIFIER)
TOKEN(NUMBER_LITERAL)
TOKEN(STRING_LITERAL)
TOKEN(BYTES_LITERAL)
MAP(TRUE, "true")
MAP(FALSE, "false")
MAP(NULL_LITERAL, "null")

#ifdef START_END
TOKEN(LITERALS_END)
#endif

#endif

#undef KEYWORDS
#undef LITERALS
#undef PUNCTUATION
#undef OPERATORS
#undef START_END
#undef MAP
#undef TOKEN