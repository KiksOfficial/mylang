#include "../include/parser.h"
#include <stdlib.h>

Token parser_peek(Parser *p) { return p->tokens.items[p->index]; }

Token parser_advance(Parser *p) { return p->tokens.items[p->index++]; }

int parser_check(Parser *p, TokenType type) {
  if (p->index >= p->tokens.size) {
    return 0;
  }
  return parser_peek(p).type == type;
}

int parser_match(Parser *p, TokenType type) {
  if (parser_check(p, type)) {
    parser_advance(p);
    return 0;
  }
  return 1;
}

ASTNode *create_AST(ASTNode *left, Token value, ASTNode *right) {
  ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));

  if (!node) {
    return NULL;
  }

  node->left = left;
  node->value = value;
  node->right = right;

  return node;
}

void free_ast(ASTNode *node) {
  if (node == NULL)
    return;

  free_ast(node->left);
  free_ast(node->right);

  free(node);
  node = NULL;
}

ASTNode *parse_tokens(TokenArray *tokens) {
  ASTNode *root;
  size_t current_index = 0;
  while (current_index <= tokens->size) {
    root->value = tokens->items[current_index];
  }

  return root;
}
