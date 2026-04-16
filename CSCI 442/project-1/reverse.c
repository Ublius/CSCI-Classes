#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  // Error Handling
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
  }

  const char *input_file = argv[1];
  FILE *input = fopen(input_file, "r");
  if (!input) {
    fprintf(stderr, "error: cannot open file '%s'\n", input_file);
    exit(1);
  }

  const char *output_file = argv[2];
  FILE *output = stdout;
  output = fopen(output_file, "w");
  if (!output) {
    fprintf(stderr, "error: cannot open file '%s'\n", output_file);
    exit(1);
  }

  if (strlen(argv[1]) == strlen(argv[2])) {
    if (strncmp(argv[1], argv[2], strlen(argv[2]) + 1) == 0) {
      fprintf(stderr, "error: input and output file must differ\n");
      exit(1);
    }
  }

  char **lines = NULL;
  size_t num_lines = 0;
  size_t line_capacity = 0;
  char *line = NULL;
  size_t len = 0;

  while (getline(&line, &len, input) != -1) {
    if (num_lines == line_capacity) {
      line_capacity = (line_capacity == 0) ? 10 : line_capacity * 2;
        char **temp = realloc(lines, line_capacity * sizeof(char *));
        if (!temp) {
          free(line);
          for (size_t i = 0; i < num_lines; i++) {
            free(lines[i]);
          }
          free(lines);
          fclose(input);
        }
      lines = temp;
    }

    size_t length = strlen(line) + 1;
    char *copy = malloc(length);
    strncpy(copy, line, length);
    lines[num_lines++] = copy;


    if (!lines[num_lines - 1]) {
      free(line);
      for (size_t i = 0; i < num_lines - 1; i++) {
        free(lines[i]);
      }
      free(lines);
      fclose(input);
    }
  }

  free(line);
  fclose(input);

  if (output_file) {
    output = fopen(output_file, "w");
    if (!output) {
      for (size_t i = 0; i < num_lines; i++) {
        free(lines[i]);
      }
      free(lines);
    }
  }

  // Print lines in reverse order
  for (size_t i = num_lines; i > 0; i--) {
    fprintf(output, "%s", lines[i - 1]);
    free(lines[i - 1]);
  }

  free(lines);
  if (output_file) {
    fclose(output);
  }

  return 0;
}
