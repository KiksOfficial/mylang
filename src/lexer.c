#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lexer.h"

#define INITIAL_CAPACITY 64

char *read_file(const char *filename, size_t *size) {
  FILE *fptr = fopen(filename, "r");
  if (fptr == NULL) {
    return NULL;
  }
  // stack overflow not AI
  fseek(fptr, 0, SEEK_END);     // seek to end of file
  long file_size = ftell(fptr); // get current file pointer
  fseek(fptr, 0, SEEK_SET);     // seek back to beginning of file

  char *file_buffer = malloc((size_t)file_size + 1);

  if (file_buffer == NULL) {

    fclose(fptr);
    fptr = NULL;
    return NULL;
  }

  fread(file_buffer, 1, (size_t)file_size, fptr);
  file_buffer[file_size] = '\0';

  fclose(fptr);

  (*size) = file_size;

  return file_buffer;
}

int check_string(TokenArray *token_arr, char **cursor) {
  (*cursor)++;
  char *start = *cursor;

  while (**cursor != '\0' && **cursor != '"') {
    (*cursor)++;
  }

  size_t len = *cursor - start;

  char *str_value = malloc(len + 1);
  if (str_value == NULL) {
    printf("str value = NULL\n");
    return 1;
  }

  memcpy(str_value, start, len);
  str_value[len] = '\0';

  Token some_str =
      (Token){.type = TOKEN_STRING, .len = len, .value = str_value};

  if (**cursor == '"') {
    (*cursor)++;
  }

  push_token(some_str, token_arr);
  return 0;
}

TokenArray *lexer() {

  size_t size;
  TokenArray *tokens = malloc(INITIAL_CAPACITY * sizeof(TokenArray));

  tokens->items = malloc(INITIAL_CAPACITY * sizeof(Token));
  tokens->size = 0;
  tokens->capacity = INITIAL_CAPACITY;

  if (tokens->items == NULL) {
    printf("tokens->items = null\n");
    return NULL;
  }

  char *source = read_file("test.my", &size);

  if (source == NULL) {
    printf("source = NULL\n");
    free(tokens);
    tokens = NULL;
    return NULL;
  }
  char *cursor = source;

  while (*cursor != '\0') {

    if (*cursor == ' ' || *cursor == '\t' || *cursor == '\n' ||
        *cursor == '\r') {
      cursor++;
      continue;
    }

    switch (*cursor) {
    case '0' ... '9':
      get_num(&cursor, tokens);
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
      get_var(tokens, &cursor);
      continue;
    case '"':
      check_string(tokens, &cursor);
      continue;
    case '{': {
      Token token = {.type = TOKEN_LCURLY, .len = 1, .value = "{"};
      push_token(token, tokens);
      break;
    }
    case '}': {
      Token token = {.type = TOKEN_RCURLY, .len = 1, .value = "}"};
      push_token(token, tokens);
      break;
    }
    case '(': {
      Token token = {.type = TOKEN_LBRACKET, .len = 1, .value = "("};
      push_token(token, tokens);
      break;
    }
    case ')': {
      Token token = {.type = TOKEN_RBRACKET, .len = 1, .value = ")"};
      push_token(token, tokens);
      break;
    }
    case '[': {
      Token token = {.type = TOKEN_LSBRACKET, .len = 1, .value = "["};
      push_token(token, tokens);
      break;
    }
    case ']': {
      Token token = {.type = TOKEN_RSBRACKET, .len = 1, .value = "]"};
      push_token(token, tokens);
      break;
    }

    case '.': {
      Token token = {.type = TOKEN_COMA, .len = 1, .value = ","};
      push_token(token, tokens);
      break;
    }

    case ';': {
      Token token = {.type = TOKEN_SEMICOLON, .len = 1, .value = ";"};
      push_token(token, tokens);
      break;
    }
    case ':': {
      Token token = {.type = TOKEN_COLON, .value = ":"};
      push_token(token, tokens);
      break;
    }
    case '=': {
      Token token = {.type = TOKEN_EQ, .len = 1, .value = "="};
      push_token(token, tokens);
      break;
    }
    }

    cursor++;
  }
  Token token = {.type = TOKEN_EOF, .len = 3, .value = "EOF"};
  push_token(token, tokens);
  printf("tokens\n");
  print_tokens(*tokens);

  return tokens;
}

int push_token(Token token, TokenArray *token_array) {
  if (token_array->size >= token_array->capacity) {
    size_t new_capacity =
        token_array->capacity == 0 ? 8 : token_array->capacity * 2;

    Token *tmp =
        realloc(token_array->items, new_capacity * sizeof *token_array->items);

    if (tmp == NULL) {
      return 1;
    }

    token_array->items = tmp;
    token_array->capacity = new_capacity;
  }

  token_array->items[token_array->size] = token;
  token_array->size++;

  return 0;
}

void print_tokens(TokenArray token_array) {
  for (size_t i = 0; i < token_array.size; i++) {
    printf("Token %zu -> Type: %d, Value: \"%.*s\"\n", i,
           token_array.items[i].type, (int)token_array.items[i].len,
           token_array.items[i].value);
  }
}

int get_num(char **cursor, TokenArray *token_array) {
  char *start = *cursor;

  while (isdigit((unsigned char)**cursor)) {
    (*cursor)++;
  }

  size_t len = *cursor - start;
  if (len == 0) {
    return 1;
  }
  char *num_value = malloc(len + 1);

  if (num_value == NULL) {
    return 1;
  }

  memcpy(num_value, start, len);
  num_value[len] = '\0';

  push_token((Token){.type = TOKEN_NUMBER, .value = num_value, .len = len},
             token_array);

  return 0;
}

void get_var(TokenArray *token_array, char **cursor) {
  char *start = *cursor;

  while (isalnum((unsigned char)**cursor) || **cursor == '_') {
    (*cursor)++;
  }

  size_t len = *cursor - start;
  if (len == 0) {
    return;
  }

  TokenType type = TOKEN_IDENT;
  if (len == 3 && memcmp(start, "i32", 3) == 0) {
    type = TOKEN_I32;
  } else if (len == 3 && memcmp(start, "u32", 3) == 0) {
    type = TOKEN_U32;
  }

  push_token((Token){.type = type, .len = len, .value = start}, token_array);
}
