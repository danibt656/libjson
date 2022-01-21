#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "include/json.h"

char *read_file(const char *filename) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        exit(1);
    }

    char *data = calloc(1, sizeof(char));
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1) {
        data = realloc(data, (strlen(data) + strlen(line) + 1) * sizeof(char));
        strcat(data, line);
    }

    fclose(fp);

    if (line)
        free(line);

    return data;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Must specify file to parse.\n");
        exit(1);
    }

    parser_t *parser = new_parser(new_lexer(read_file(argv[1])));

    ast_t *ast = parser_parse(parser);
    
    json_iterator iterator = iterate(ast);
    visitor_visit(ast);
    
    while (iterator.current) {
        char* str = json_string(iterator.current);
        printf("%s\n", str);
        json_iterator_next(&iterator);
    }

    parser_free(parser);
    ast_free(ast);
}