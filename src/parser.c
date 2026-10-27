#include "../include/parser.h"

typedef struct {
  char *value;
  char *name;
  TokenType type;
} ParsedToken;

Token parser_peek(Parser *p) { return p->tokens.items[p->index]; }

Token parser_advance(Parser *p) { return p->tokens.items[p->index++]; }
