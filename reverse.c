#include <stdio.h>
#include <stdlib.h>

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

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&line, &len, in)) != -1) {
        fprintf(stdout, "%s", line);
    }

    free(line);
    fclose(in);
    return 0;
}
