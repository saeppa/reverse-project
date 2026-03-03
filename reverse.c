#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void die_malloc_cleanup(char *line, char **lines, size_t count) {
    fprintf(stderr, "malloc failed\n");
    free(line);
    for (size_t i = 0; i < count; i++) free(lines[i]);
    free(lines);
    exit(1);
}

int main(int argc, char *argv[]) {
    FILE *in = NULL;
    FILE *out = NULL;
    const char *in_name = NULL;
    const char *out_name = NULL;

    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (argc == 1) {
        in = stdin;
        out = stdout;
    } else if (argc == 2) {
        in_name = argv[1];
        in = fopen(in_name, "r");
        if (in == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", in_name);
            exit(1);
        }
        out = stdout;
    } else {
        in_name = argv[1];
        out_name = argv[2];

        in = fopen(in_name, "r");
        if (in == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", in_name);
            exit(1);
        }

        out = fopen(out_name, "w");
        if (out == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", out_name);
            fclose(in);
            exit(1);
        }
    }

    char **lines = NULL;
    size_t capacity = 0;
    size_t count = 0;

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&line, &len, in)) != -1) {
        if (count == capacity) {
            size_t newcap = (capacity == 0) ? 8 : capacity * 2;
            char **tmp = realloc(lines, newcap * sizeof(*lines));
            if (tmp == NULL) {
                if (in != stdin) fclose(in);
                if (out != stdout && out != NULL) fclose(out);
                die_malloc_cleanup(line, lines, count);
            }
            lines = tmp;
            capacity = newcap;
        }

        char *copy = strdup(line);
        if (copy == NULL) {
            if (in != stdin) fclose(in);
            if (out != stdout && out != NULL) fclose(out);
            die_malloc_cleanup(line, lines, count);
        }

        lines[count++] = copy;
    }

    for (ssize_t i = (ssize_t)count - 1; i >= 0; i--) {
        fprintf(out, "%s", lines[i]);
    }

    free(line);
    if (in != stdin) fclose(in);
    if (out != stdout && out != NULL) fclose(out);
    for (size_t i = 0; i < count; i++) free(lines[i]);
    free(lines);

    return 0;
}
