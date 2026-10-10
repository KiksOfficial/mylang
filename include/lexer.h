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
int lexer();
void print_tokens(TokenArray token_array);

#endif
