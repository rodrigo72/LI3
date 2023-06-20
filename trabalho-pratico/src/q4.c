#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "../includes/structs.h"
#include "../includes/stats.h"
#include "../includes/utils.h"

void query_4 (STATS *stats, char *path, int i, char *line) {

    char *file = malloc(sizeof(char) * 25);
    sprintf(file, "command%d_output.txt", i);
    char *filepath = concat(path, file);
    free(file);

    FILE *fp = fopen(filepath, "w+");
    free(filepath);

    double d = 0;
    CITY_STATS *city_stats = g_hash_table_lookup(get_cities_stats_hash(stats), line);

    if (city_stats != NULL) {
        double preco_total = get_city_stats_preco_total(city_stats);
        double numero_de_viagens = (double) get_city_stats_numero_viagens(city_stats);
        d = preco_total / numero_de_viagens;
    }

    if (d > 0) fprintf(fp, "%.3f\n", d);
    fclose(fp);
}