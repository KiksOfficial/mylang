#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stddef.h>

typedef struct {
  TokenArray tokens;
  size_t index;
} Parser;

typedef enum {
  NODE_PROGRAM,
  NODE_VAR_DECL,
  NODE_LITERAL,
  NODE_BINARY_OP
} NodeType;

typedef struct ASTNode {
  NodeType type;

  Token var_type;
  Token init;
  struct ASTNode *init;

  ASTNode **children;
  size_t children_count;
} ASTNode;

ASTNode *parse_tokens(TokenArray *tokens);

#endif
