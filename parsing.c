#include "mpc.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define mpcp mpc_parser_t

#ifdef WIN32
static char buffer[2048];

char *readline(char *prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char *cpy = malloc(strlen(buffer) + 1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy) - 1] = '\0';
  return cpy;
}

void add_history(char *unused) {}

#else
#include <editline/readline.h>
#include <readline/history.h>
#endif

typedef struct {
  int type;
  long num;
  int err;
} lval;

enum { LVAL_NUM, LVAL_ERR };
enum { LERR_DIV_Z, LERR_BAD_OP, LERR_BAD_NUM };

int my_strcmp(char *s1, char *s2) {
  while (*s1 != '\0' && *s2 != '\0') {
    if (*s1 == *s2) {
      s1++;
      s2++;
    } else {
      return 1;
    }
  }
  return 0;
}

lval lval_num(long x) {
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}

lval lval_err(int x) {
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

lval eval_op(lval x, char *op, lval y) {
  if (x.type == LVAL_ERR) {
    return x;
  }
  if (x.type == LVAL_ERR) {
    return y;
  }
  if (strcmp(op, "+") == 0) {
    return lval_num(x.num + y.num);
  }
  if (strcmp(op, "-") == 0) {
    return lval_num(x.num - y.num);
  }
  if (strcmp(op, "*") == 0) {
    return lval_num(x.num * y.num);
  }
  if (strcmp(op, "/") == 0) {
    return y.num == 0 ? lval_err(LERR_DIV_Z) : lval_num(x.num / y.num);
  }
  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t *t) {

  /* If tagged as number return it directly. */
  if (strstr(t->tag, "number")) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  }

  /* The operator is always second child. */
  char *op = t->children[1]->contents;

  /* We store the third child in `x` */
  lval x = eval(t->children[2]);

  /* Iterate the remaining children and combining. */
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;
}

void lval_print(lval v) {
  switch (v.type) {
  case LVAL_NUM:
    printf("%li", v.num);
    break;
  case LVAL_ERR:
    if (v.err == LERR_DIV_Z) {
      printf("Error: Can Division By 0");
    }
    if (v.err == LERR_BAD_NUM) {
      printf("Error: Invalid Number");
    }
    if (v.err == LERR_BAD_OP) {
      printf("Eror: Invalid Operation");
    }
    break;
  }
}

void lval_println(lval v) {
  lval_print(v);
  putchar('\n');
};

int num_of_nodes(mpc_ast_t *tree) {
  if (tree->children_num == 0) {
    return 1;
  } else if (tree->children_num >= 1) {
    size_t total = 1;

    for (size_t i = 0; i < tree->children_num; i++) {
      total += num_of_nodes(tree->children[i]);
    }
    return total;
  }
  return 0;
}

int main() {

  /* Create Some Parsers */
  mpcp *Number = mpc_new("number");
  mpcp *Symbol = mpc_new("symbol");
  mpcp *Sexpr = mpc_new("sexpr");
  mpcp *Expr = mpc_new("expr");
  mpcp *Lispy = mpc_new("lispy");

  /* Define them with the following Language */
  mpca_lang(MPCA_LANG_DEFAULT, "                                          \
    number : /-?[0-9]+/ ;                    \
    symbol : '+' | '-' | '*' | '/' ;         \
    sexpr  : '(' <expr>* ')' ;               \
    expr   : <number> | <symbol> | <sexpr> ; \
    lispy  : /^/ <expr>* /$/ ;               \
  ",
            Number, Symbol, Sexpr, Expr, Lispy);

  // static char buffer[1024];
  printf("Lisp version 0.0.0.1\n");
  puts("Press Ctrl+c to Exit\n");

  while (1) {

    char *sisend = readline("lispy>");
    if (my_strcmp(":q", sisend) == 0) {
      return 0;
    }

    add_history(sisend);

    mpc_result_t r;
    if (mpc_parse("<stdin>", sisend, Lispy, &r)) {
      lval result = eval(r.output);

      lval_println(result);
      mpc_ast_delete(r.output);

    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    free(sisend);
    sisend = NULL;
  }

  mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Lispy);
}

// static makes this var local to the file
// atoi converts char* -> int
// Takes as input two char* and returns a pointer to the location of the second
// in the first, or 0 if the second is not a sub-string of the first.
