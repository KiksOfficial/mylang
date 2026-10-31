#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

typedef enum {
  TOKEN_NUMBER,
  TOKEN_STRING,

  TOKEN_LCURLY,
  TOKEN_RCURLY,

  TOKEN_LBRACKET,
  TOKEN_RBRACKET,

  TOKEN_LSBRACKET,
  TOKEN_RSBRACKET,

  TOKEN_COMA,
  TOKEN_COLON,
  TOKEN_SEMICOLON,

  TOKEN_EQ,
  TOKEN_EQ_EQ,
  TOKEN_LT,
  TOKEN_GT,
  TOKEN_SLASH,
  TOKEN_STAR,
  TOKEN_PLUS,
  TOKEN_MINUS,

  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_AND,
  TOKEN_FUNC,
  TOKEN_RETURN,

  TOKEN_I8,
  TOKEN_I16,
  TOKEN_I32,
  TOKEN_I64,

  TOKEN_U8,
  TOKEN_U16,
  TOKEN_U32,
  TOKEN_U64,

  TOKEN_STR,
  TOKEN_CHAR,

  TOKEN_IDENT,

  TOKEN_BOOL,

  TOKEN_EOF

} TokenType;

typedef struct {
  TokenType type;
  char *value;
  size_t len;
} Token;

typedef struct {
  Token *items;
  size_t capacity;
  size_t size;
} TokenArray;

int parse_file(const char *filename);
int push_token(Token token, TokenArray *token_array);
TokenArray *lexer();
void print_tokens(TokenArray token_array);
void get_var(TokenArray *token_array, char **cursor);
int get_num(char **cursor, TokenArray *token_array);

#endif
