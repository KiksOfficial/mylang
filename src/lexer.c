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

int lexer() {

  size_t size;
  TokenArray *tokens = malloc(INITIAL_CAPACITY * sizeof(TokenArray));

  tokens->items = malloc(INITIAL_CAPACITY * sizeof(Token));
  tokens->size = 0;
  tokens->capacity = INITIAL_CAPACITY;

  if (tokens->items == NULL) {
    printf("tokens->items = null\n");
    return 1;
  }

  char *source = read_file("test.my", &size);

  if (source == NULL) {
    printf("source = NULL\n");
    free(tokens);
    tokens = NULL;
    return 1;
  }
  char *cursor = source;

  while (*cursor != '\0') {

    if (*cursor == ' ' || *cursor == '\t' || *cursor == '\n' ||
        *cursor == '\r') {
      cursor++;
      continue;
    }

    if (*cursor == '"') {

      check_string(tokens, &cursor);
      continue;
    }

    if (*cursor == 'i') {
      get_i32(tokens, &cursor);
    }

    switch (*cursor) {
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
    }

    cursor++;
  }
  Token token = {.type = TOKEN_EOF, .len = 3, .value = "EOF"};
  push_token(token, tokens);
  printf("tokens\n");
  print_tokens(*tokens);

  return 0;
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
int get_i32(TokenArray *token_array, char **cursor) {
  char *start = *cursor;

  if (strncmp(start, "i32", 3) == 0) {
    *cursor += 3;
    size_t len = *cursor - start;

    Token num = (Token){.type = TOKEN_I32, .len = len, .value = start};
    push_token(num, token_array);

  } else if (strncmp(start, "u32", 3) == 0) {
    *cursor += 3;

    size_t len = *cursor - start;

    Token num = (Token){.type = TOKEN_U32, .len = len, .value = start};
    push_token(num, token_array);
  }

  return 0;
}
