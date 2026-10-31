#include "include/lexer.h"
#include "include/parser.h"

int main() {
  TokenArray *tokens = lexer();

  ASTNode *graph = append_nodes(tokens);

  return 0;
}
