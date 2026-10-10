#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include <stdlib.h>

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
int parser();
void print_tokens(TokenArray token_array);

#endif
