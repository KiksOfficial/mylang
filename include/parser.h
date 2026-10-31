#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stddef.h>

typedef struct ASTNode ASTNode;

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

  Token token_type;
  Token name;
  struct ASTNode *ast_init; // token value

  ASTNode **children;
  size_t children_count;
} ASTNode;

ASTNode *append_nodes(TokenArray *tokens);

#endif
