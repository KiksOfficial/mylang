#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
    fptr = NULL return NULL;
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
  Token *tokens = malloc(INITIAL_CAPACITY * sizeof(Token));

  char *source = read_file("test.my", &size);

  if (source == NULL) {

    return 1;
  }

  for (size_t i = 0; i < size; i++) {
    count++;
    if (count >= capacity) {
      capacity *= 2;
      Token *new_tokens = realloc(tokens, capacity * sizeof(Token));
      if (new_tokens == NULL) {
        free(new_tokens);
        new_tokens = NULL;
      };
      tokens = new_tokens;
    }
    switch (source[i]) { case '': }
  }

  return 0;
}

int push_token(Token *token, TokenArray *array) { return 0; }
