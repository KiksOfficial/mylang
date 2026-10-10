#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

typedef enum {
  TOKEN_NUMBER,
  TOKEN_CHAR,

  TOKEN_LCURLY,
  TOKEN_RCURLY,

  TOKEN_LBRACKET,
  TOKEN_RBRACKET,

  TOKEN_LSBRACET,
  TOKEN_RSBRACKET,

  TOKEN_COMA,
  TOKEN_COLON,
  TOKEN_SEMICOLON,

  TOKEN_EOF

} TokenType;

typedef struct {
  TokenType type;
  char *value;
} Token;

typedef struct {
  Token *items;
  size_t capacity;
  size_t size;
} TokenArray;

int parse_file(const char *filename);
int push_token(Token token, TokenArray *token_array);
int lexer();
void print_tokens(TokenArray token_array);

#endif
