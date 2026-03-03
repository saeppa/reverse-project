/*
* reverse.c – Mini Project 1
* Authors: Sara Eriksson & Sami Salonen
*
* Lähteet:
*
* 1. W. Richard Stevens & Stephen A. Rago (2013).
*    "Advanced Programming in the UNIX Environment", 3rd Edition.
*    Käytetty tiedostonkäsittelyyn, standard I/O -funktioihin
*    (fopen, getline, fprintf) ja muistinhallintaan (malloc, realloc, free).
*
* 2. Remzi H. Arpaci-Dusseau & Andrea C. Arpaci-Dusseau (2018).
*    "Operating Systems: Three Easy Pieces".
*    Käytetty projektin taustakäsitteisiin ja työn vaiheittaiseen toteutukseen.
*
* 3. OSTEP Project Repository:
*    https://github.com/remzi-arpacidusseau/ostep-projects
*    Virallinen tehtävänanto ja projektikuvaus.
*
* 4. Linux man-sivut (man7.org):
*    https://man7.org/linux/man-pages/man3/
*    Käytetty funktioiden kutsumuotojen ja virheenkäsittelyn tarkistamiseen.
*
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_and_die_malloc(char *line, char **lines, size_t count, FILE *in, FILE *out) {
    fprintf(stderr, "malloc failed\n");
    free(line);
    for (size_t i = 0; i < count; i++) free(lines[i]);
    free(lines);
    if (in && in != stdin) fclose(in);
    if (out && out != stdout) fclose(out);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    FILE *in = NULL;
    FILE *out = NULL;
    const char *in_name = NULL;
    const char *out_name = NULL;

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

        if (strcmp(in_name, out_name) == 0) {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }

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
                cleanup_and_die_malloc(line, lines, count, in, out);
            }
            lines = tmp;
            capacity = newcap;
        }

        char *copy = strdup(line);
        if (copy == NULL) {
            cleanup_and_die_malloc(line, lines, count, in, out);
        }

        lines[count++] = copy;
    }

    for (ssize_t i = (ssize_t)count - 1; i >= 0; i--) {
        if (fprintf(out, "%s", lines[i]) < 0) {
            free(line);
            for (size_t k = 0; k < count; k++) free(lines[k]);
            free(lines);
            if (in && in != stdin) fclose(in);
            if (out && out != stdout) fclose(out);
            exit(1);
        }
    }

    free(line);
    if (in && in != stdin) fclose(in);
    if (out && out != stdout) fclose(out);
    for (size_t i = 0; i < count; i++) free(lines[i]);
    free(lines);

    return 0;
}
