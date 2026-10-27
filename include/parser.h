#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stddef.h>

typedef struct {
  TokenArray tokens;
  size_t index;
} Parser;

int peek(Parser *p);

#endif
