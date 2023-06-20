#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "../includes/utils.h"
#include "../includes/stats.h"
#include "../includes/structs.h"
#include "../includes/query_manager.h"

void sub_parser (LIST *lists, STATS *stats, HASH *hash, char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) assert(0);

    char *line = NULL;
    size_t len = 0;
    for (int i = 1;getline(&line, &len, fp) != -1; i++) {
        line = strsep(&line, "\n");
        char *str  = line;
        char *num  = strsep(&str, " ");
        int q = atoi(num);
        query_manager(lists, stats, hash, q, i, str);
    }

    free(line);
    fclose(fp);
}
