#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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

  char *file_buffer = malloc(sizeof(file_size) + 1);

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

void check_string(TokenArray *token_arr, size_t **cursor) {
  (*cursor)++;
  size_t start = **cursor;

  while (**cursor != '\0') {
  }
}

int lexer() {
  printf("lexer\n");

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

  for (size_t i = 0; i < size; i++) {
    char c = source[i];
    if (isspace((unsigned char)c)) {
      continue;
    }

    switch (c) {
    case '{': {
      Token token = {.type = TOKEN_LCURLY, .value = "{"};
      push_token(token, tokens);
      break;
    }
    case '}': {
      Token token = {.type = TOKEN_RCURLY, .value = "}"};
      push_token(token, tokens);
      break;
    }
    case '(': {
      Token token = {.type = TOKEN_LBRACKET, .value = "("};
      push_token(token, tokens);
      break;
    }
    case ')': {
      Token token = {.type = TOKEN_LBRACKET, .value = ")"};
      push_token(token, tokens);
      break;
    }
    case '[': {
      Token token = {.type = TOKEN_LSBRACKET, .value = "["};
      push_token(token, tokens);
      break;
    }
    case ']': {
      Token token = {.type = TOKEN_RSBRACKET, .value = "]"};
      push_token(token, tokens);
      break;
    }

    case '.': {
      Token token = {.type = TOKEN_COMA, .value = ","};
      push_token(token, tokens);
      break;
    }

    case ';': {
      Token token = {.type = TOKEN_SEMICOLON, .value = ";"};
      push_token(token, tokens);
      break;
    }
    case ':': {
      Token token = {.type = TOKEN_COLON, .value = ";"};
      push_token(token, tokens);
      break;
    }
    }
  }
  Token token = {.type = TOKEN_EOF, .value = "EOF"};
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
    printf("Token %zu -> Type: %d, Value: \"%s\"\n", i,
           token_array.items[i].type, token_array.items[i].value);
  }
}
