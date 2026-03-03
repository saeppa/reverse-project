#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: reverse <input>\n");
        return 1;
    }

    const char *in_name = argv[1];
    FILE *in = fopen(in_name, "r");
    if (in == NULL) {
        fprintf(stderr, "error: cannot open file '%s'\n", in_name);
        return 1;
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
                fprintf(stderr, "malloc failed\n");
                free(line);
                for (size_t i = 0; i < count; i++) free(lines[i]);
                free(lines);
                fclose(in);
                return 1;
            }
            lines = tmp;
            capacity = newcap;
        }

        char *copy = strdup(line);
        if (copy == NULL) {
            fprintf(stderr, "malloc failed\n");
            free(line);
            for (size_t i = 0; i < count; i++) free(lines[i]);
            free(lines);
            fclose(in);
            return 1;
        }

        lines[count++] = copy;
    }

    for (size_t i = 0; i < count; i++) {
        fprintf(stdout, "%s", lines[i]);
    }

    free(line);
    fclose(in);
    for (size_t i = 0; i < count; i++) free(lines[i]);
    free(lines);
    return 0;
}
