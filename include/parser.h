#ifndef PARSER_H
#define PARSER_H

typedef struct {
  int some_data;
} Token;

int parse_file(const char *filename);

#endif
