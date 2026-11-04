#include "../include/parser.h"
#include <stddef.h>
#include <stdio.h>
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
    return 1;
  }
  return 0;
}

ASTNode *create_single_node(TokenArray *tokens, size_t *index) {
  ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
  if (node == NULL)
    return NULL;

  Parser p = {.tokens = *tokens, .index = *index};

  if (parser_match(&p, TOKEN_I32)) {
    node->type = NODE_VAR_DECL;

    if (parser_check(&p, TOKEN_IDENT)) {
      node->name = parser_advance(&p);
    } else {
      free(node);
      return NULL;
    }
  }

  *index = p.index;

  return node;
}

int make_AST(TokenArray *tokens) {
  size_t index = 0;
  ASTNode *root = malloc(sizeof(ASTNode));

  ASTNode *outcome = append_nodes(tokens, &index, root);
  return 0;
}

ASTNode *append_nodes(TokenArray *tokens, size_t *index, ASTNode *root) {

  root->children_count = 0;
  root->type = NODE_PROGRAM;
  root->children = NULL;

  while (*index < tokens->size) {
    ASTNode *child = create_single_node(tokens, index);
    if (child == NULL) {
      (*index)++;
      continue;
    }
    root->children_count++;

    ASTNode **tmp =
        realloc(root->children, root->children_count * sizeof(ASTNode *));
    if (tmp == NULL)
      return NULL;
    root->children = tmp;

    root->children[root->children_count - 1] = child;
  }
  return root;
}
