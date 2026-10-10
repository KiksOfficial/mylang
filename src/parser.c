#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/lexer.h"
#include "../include/parser.h"

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

  char *file_buffer = malloc(sizeof(size) + 1);

  if (file_buffer == NULL) {
    free(fptr);
    fptr = NULL;
    return NULL;
  }

  fread(file_buffer, 1, 1, fptr);
  file_buffer[file_size] = '\0';

  fclose(fptr);

  (*size) = file_size;

  return file_buffer;
}

int parser() {

  size_t size;
  size_t capacity = INITIAL_CAPACITY;
  size_t count = 0;
  TokenArray *tokens = malloc(INITIAL_CAPACITY * sizeof(TokenArray));

  if (tokens->items == NULL) {
    return 1;
  }

  char *source = read_file("test.my", &size);

  if (source == NULL) {
    free(tokens);
    tokens = NULL;
    return 1;
  }

  for (size_t i = 0; i < size; i++) {
    char c = source[i];

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
    }
  }

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
